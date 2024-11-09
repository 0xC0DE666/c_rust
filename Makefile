NAME := libc_errors
VERSION := 0.0.0

CC := gcc
C_FLAGS := -std=c99 -g -Wall -Wextra

define GET_VERSIONED_NAME
$(NAME).$(1).$(VERSION)
endef

SRC_DIR := ./src
DEPS_ROOT_DIR := $(SRC_DIR)/deps
DEPS_OBJS := $(shell find $(DEPS_ROOT_DIR) -type f -name "*.o")

BUILD_DIR := ./build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
RELEASE_DIR := $(BUILD_DIR)/release
RELEASE_O := $(RELEASE_DIR)/$(NAME).o
VERSIONED_RELEASE_ASSETS := $(call GET_VERSIONED_NAME,o) $(call GET_VERSIONED_NAME,a) $(call GET_VERSIONED_NAME,so)
UNVERSIONED_RELEASE_ASSETS := $(NAME).o $(NAME).a $(NAME).so

all: clean $(UNVERSIONED_RELEASE_ASSETS) app test;

#------------------------------
# APP
#------------------------------

APP_SRC_DIR := $(SRC_DIR)/app
APP_OBJ_DIR := $(OBJ_DIR)/app
APP_HDRS := $(wildcard $(APP_SRC_DIR)/*.h)
APP_SRCS := $(wildcard $(APP_SRC_DIR)/*.c)
APP_OBJS := $(patsubst $(APP_SRC_DIR)/%.c, $(APP_OBJ_DIR)/%.o, $(APP_SRCS))

$(APP_OBJ_DIR)/%.o: $(APP_SRC_DIR)/%.c | $(APP_OBJ_DIR)
	$(CC) $(C_FLAGS) -c $< -o $@

app: $(APP_OBJS) $(RELEASE_O);
	$(CC) $(C_FLAGS) -o $(BIN_DIR)/$@ $(APP_OBJS) $(RELEASE_O);

#------------------------------
# LIB
#------------------------------

LIB_SRC_DIR := $(SRC_DIR)/lib
LIB_OBJ_DIR := $(OBJ_DIR)/lib
LIB_HDRS := $(wildcard $(LIB_SRC_DIR)/*.h)
LIB_SRCS := $(wildcard $(LIB_SRC_DIR)/*.c)
LIB_OBJS := $(patsubst $(LIB_SRC_DIR)/%.c, $(LIB_OBJ_DIR)/%.o, $(LIB_SRCS))

$(LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.c | $(LIB_OBJ_DIR)
	$(CC) $(C_FLAGS) -c $< -o $@

# VERSIONED
$(call GET_VERSIONED_NAME,o): $(LIB_OBJS) $(DEPS_OBJS);
	ld -relocatable -o $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

$(call GET_VERSIONED_NAME,a): $(LIB_OBJS) $(DEPS_OBJS);
	ar rcs $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

$(call GET_VERSIONED_NAME,so): $(LIB_OBJS) $(DEPS_OBJS);
	$(CC) $(C_FLAGS) -fPIC -shared -lc -o $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

# UNVERSIONED
$(NAME).o: $(LIB_OBJS) $(DEPS_OBJS);
	ld -relocatable -o $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

$(NAME).a: $(LIB_OBJS) $(DEPS_OBJS);
	ar rcs $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

$(NAME).so: $(LIB_OBJS) $(DEPS_OBJS);
	$(CC) $(C_FLAGS) -fPIC -shared -lc -o $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

#------------------------------
# TESTS
#------------------------------

TEST_SRC_DIR := $(SRC_DIR)/test
TEST_OBJ_DIR := $(OBJ_DIR)/test
TEST_HDRS := $(wildcard $(TEST_SRC_DIR)/*.h)
TEST_SRCS := $(wildcard $(TEST_SRC_DIR)/*.c)
TEST_OBJS := $(patsubst $(TEST_SRC_DIR)/%.c, $(TEST_OBJ_DIR)/%.o, $(TEST_SRCS))

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.c | $(TEST_OBJ_DIR)
	$(CC) $(C_FLAGS) -c $< -o $@

test: $(TEST_OBJS) $(RELEASE_O);
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_DIR)/$@ $(TEST_OBJS) $(RELEASE_O);

#------------------------------
# RELEASE
#------------------------------

release: C_FLAGS := -std=c99 -O2 -g -DNDDEBUG -Wall -Wextra
release: clean $(VERSIONED_RELEASE_ASSETS) $(UNVERSIONED_RELEASE_ASSETS) app test;
	cp $(LIB_HDRS) $(RELEASE_DIR);
	echo $(VERSION) > $(RELEASE_DIR)/version.txt;
	tar -czvf $(BUILD_DIR)/$(call GET_VERSIONED_NAME,tar.gz) -C $(RELEASE_DIR) .;

clean:
	rm -f $(APP_OBJS) $(LIB_OBJS) $(TEST_OBJS) $(RELEASE_DIR)/* $(BIN_DIR)/* $(BUILD_DIR)/$(call GET_VERSIONED_NAME,tar.gz);
