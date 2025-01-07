VERSION := 0.1.0
NAME := libc_rust

CC := gcc
C_FLAGS := -std=gnu99 -g -Wall -Wextra

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
APP_HDRS := $(shell find $(APP_SRC_DIR) -type f -name "*.h")
APP_SRCS := $(shell find $(APP_SRC_DIR) -type f -name "*.c")
APP_OBJS := $(patsubst $(APP_SRC_DIR)/%.c, $(APP_OBJ_DIR)/%.o, $(APP_SRCS))

$(APP_OBJ_DIR)/%.o: $(APP_SRC_DIR)/%.c | $(APP_OBJ_DIR)
	$(CC) $(C_FLAGS) -c $< -o $@

app: $(APP_OBJS) $(RELEASE_O);
	$(CC) $(C_FLAGS) -o $(BIN_DIR)/$@ $(APP_OBJS) $(RELEASE_O);

#------------------------------
# LIB
#------------------------------

C_SO_FLAGS := -shared -fPIC -lc
LIB_SRC_DIR := $(SRC_DIR)/lib
LIB_OBJ_DIR := $(OBJ_DIR)/lib
LIB_HDRS := $(shell find $(LIB_SRC_DIR) -type f -name "*.h")
LIB_SRCS := $(shell find $(LIB_SRC_DIR) -type f -name "*.c")
LIB_OBJS := $(patsubst $(LIB_SRC_DIR)/%.c, $(LIB_OBJ_DIR)/%.o, $(LIB_SRCS))

$(LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.c | $(LIB_OBJ_DIR)
	$(CC) $(C_FLAGS) -c $< -o $@

# VERSIONED
$(call GET_VERSIONED_NAME,o): $(LIB_OBJS) $(DEPS_OBJS);
	ld -relocatable -o $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

$(call GET_VERSIONED_NAME,a): $(LIB_OBJS) $(DEPS_OBJS);
	ar rcs $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

$(call GET_VERSIONED_NAME,so): $(LIB_OBJS) $(DEPS_OBJS);
	$(CC) $(C_FLAGS) $(C_SO_FLAGS) -o $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

# UNVERSIONED
$(NAME).o: $(LIB_OBJS) $(DEPS_OBJS);
	ld -relocatable -o $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

$(NAME).a: $(LIB_OBJS) $(DEPS_OBJS);
	ar rcs $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

$(NAME).so: $(LIB_OBJS) $(DEPS_OBJS);
	$(CC) $(C_FLAGS) $(C_SO_FLAGS) -o $(RELEASE_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

#------------------------------
# TESTS
#------------------------------

TEST_SRC_DIR := $(SRC_DIR)/test
TEST_OBJ_DIR := $(OBJ_DIR)/test
TEST_HDRS := $(shell find $(TEST_SRC_DIR) -type f -name "*.h")
TEST_SRCS := $(shell find $(TEST_SRC_DIR) -type f -name "*.c")
TEST_OBJS := $(patsubst $(TEST_SRC_DIR)/%.c, $(TEST_OBJ_DIR)/%.o, $(TEST_SRCS))

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.c | $(TEST_OBJ_DIR)
	$(CC) $(C_FLAGS) -c $< -o $@

test: $(TEST_OBJS) $(RELEASE_O);
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_DIR)/$@ $(TEST_OBJS) $(RELEASE_O);

#------------------------------
# RELEASE
#------------------------------

release: C_FLAGS := -std=gnu99 -O2 -g -DNDDEBUG -Wall -Wextra -pthread
release: clean $(VERSIONED_RELEASE_ASSETS) $(UNVERSIONED_RELEASE_ASSETS) app test;
	cp $(LIB_HDRS) $(RELEASE_DIR);
	echo $(VERSION) > $(RELEASE_DIR)/version.txt;
	tar -czvf $(BUILD_DIR)/$(call GET_VERSIONED_NAME,tar.gz) -C $(RELEASE_DIR) .;

.PHONY: exe_app exe_test;

exe_app: clean $(NAME).o app;
	./build/bin/app;

exe_test: clean $(NAME).o test;
	./build/bin/test;

clean:
	rm -f $(APP_OBJS) $(LIB_OBJS) $(TEST_OBJS) $(RELEASE_DIR)/* $(BIN_DIR)/* $(BUILD_DIR)/$(call GET_VERSIONED_NAME,tar.gz);
