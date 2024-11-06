VERSION := 0.0.0
NAME := libc_errors
QUALIFIER := $(NAME)-$(VERSION)

CC := gcc
C_FLAGS := -std=c99 -g -Wall -Wextra

BUILD_DIR := ./build
BIN_DIR := $(BUILD_DIR)/bin
DIST_DIR := $(BUILD_DIR)/dist
OBJ_DIR := $(BUILD_DIR)/obj
SRC_DIR := ./src
# DIST_OBJS := $(wildcard $(DIST_DIR)/*.o)
DIST_OBJS := $(DIST_DIR)/$(NAME).o

all: clean $(NAME).o $(NAME).so $(NAME).a app test;

DEPS_DIR := $(SRC_DIR)/deps
DEPS_OBJS := $(wildcard $(DEPS_DIR)/*.o)

#------------------------------
# APP
#------------------------------

APP_SRC_DIR := $(SRC_DIR)/app
APP_OBJ_DIR := $(OBJ_DIR)/app
APP_HDRS = $(wildcard $(APP_SRC_DIR)/*.h)
APP_SRCS := $(wildcard $(APP_SRC_DIR)/*.c)
APP_OBJS := $(patsubst $(APP_SRC_DIR)/%.c, $(APP_OBJ_DIR)/%.o, $(APP_SRCS))

$(APP_OBJ_DIR)/%.o: $(APP_SRC_DIR)/%.c | $(APP_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

app: $(APP_OBJS) $(DIST_OBJS);
	$(CC) $(C_FLAGS) -o $(BIN_DIR)/$@ $(APP_OBJS) $(DIST_OBJS);

#------------------------------
# LIB
#------------------------------

LIB_SRC_DIR := $(SRC_DIR)/lib
LIB_OBJ_DIR := $(OBJ_DIR)/lib
LIB_HDRS = $(wildcard $(LIB_SRC_DIR)/*.h)
LIB_SRCS := $(wildcard $(LIB_SRC_DIR)/*.c)
LIB_OBJS := $(patsubst $(LIB_SRC_DIR)/%.c, $(LIB_OBJ_DIR)/%.o, $(LIB_SRCS))

$(LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.c | $(LIB_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME).o: $(LIB_OBJS) $(DEPS_OBJS);
	ld -relocatable -o $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

$(NAME).a: $(LIB_OBJS) $(DEPS_OBJS);
	ar rcs $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

$(NAME).so: $(LIB_OBJS) $(DEPS_OBJS);
	$(CC) $(C_FLAGS) -fPIC -shared -lc -o $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

#------------------------------
# TESTS
#------------------------------

TEST_SRC_DIR := $(SRC_DIR)/test
TEST_OBJ_DIR := $(OBJ_DIR)/test
TEST_HDRS = $(wildcard $(TEST_SRC_DIR)/*.h)
TEST_SRCS := $(wildcard $(TEST_SRC_DIR)/*.c)
TEST_OBJS := $(patsubst $(TEST_SRC_DIR)/%.c, $(TEST_OBJ_DIR)/%.o, $(TEST_SRCS))

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.c | $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_OBJS) $(DIST_OBJS);
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_DIR)/$@ $(TEST_OBJS) $(DIST_OBJS);

#------------------------------
# RELEASE
#------------------------------

release: C_FLAGS := -std=c99 -O2 -g -DNDDEBUG -Wall -Wextra
release: clean $(NAME).o $(NAME).so $(NAME).a app test;
	cp $(LIB_HDRS) $(DIST_DIR);
	tar -czvf $(BUILD_DIR)/$(NAME).tar.gz $(DIST_DIR);

clean:
	rm -f $(APP_OBJS) $(LIB_OBJS) $(TEST_OBJS) $(DIST_DIR)/* $(BIN_DIR)/* $(BUILD_DIR)/$(NAME).tar.gz;
