VERSION := 0.0.0
NAME := libc_errors
QUALIFIER := $(NAME)-$(VERSION)

CC := gcc
C_FLAGS := -std=c99 -g -Wall -Wextra

BIN_DIR := ./build/bin
DIST_DIR := ./build/dist
# DIST_OBJS := $(wildcard $(DIST_DIR)/*.o)
DIST_OBJS := $(DIST_DIR)/libc_errors.o

all: clean libc_errors.o libc_errors.so libc_errors.a app test;

DEPS_DIR := ./src/deps
DEPS_OBJS := $(wildcard $(DEPS_DIR)/*.o)

#------------------------------
# APP
#------------------------------

APP_DIR := ./src/app
APP_HDRS = $(wildcard $(APP_DIR)/*.h)
APP_SRCS := $(wildcard $(APP_DIR)/*.c)
APP_OBJS := $(patsubst %.c, %.o, $(APP_SRCS))

$(APP_OBJS):
	$(CC) $(C_FLAGS) -c -o $@ $(patsubst %.o, %.c, $@);

app: $(APP_OBJS) $(DIST_OBJS);
	$(CC) $(C_FLAGS) -o $(BIN_DIR)/$@ $(APP_OBJS) $(DIST_OBJS);

#------------------------------
# LIB
#------------------------------

LIB_DIR := ./src/lib
LIB_HDRS = $(wildcard $(LIB_DIR)/*.h)
LIB_SRCS = $(wildcard $(LIB_DIR)/*.c)
LIB_OBJS := $(patsubst %.c, %.o, $(LIB_SRCS))

$(LIB_OBJS):
	$(CC) $(C_FLAGS) -c -o $@ $(patsubst %.o, %.c, $@);

libc_errors.o: $(LIB_OBJS) $(DEPS_OBJS);
	cp $(LIB_HDRS) $(DIST_DIR)
	ld -relocatable -o $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

libc_errors.a: $(LIB_OBJS) $(DEPS_OBJS);
	ar rcs $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

libc_errors.so: $(LIB_OBJS) $(DEPS_OBJS);
	$(CC) $(C_FLAGS) -fPIC -shared -lc -o $(DIST_DIR)/$@ $(LIB_OBJS) $(DEPS_OBJS);

#------------------------------
# TESTS
#------------------------------

TEST_DIR := ./src/test
TEST_HDRS = $(wildcard $(TEST_DIR)/*.h)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS := $(patsubst %.c, %.o, $(TEST_SRCS))

$(TEST_OBJS):
	$(CC) $(C_FLAGS) -c -o $@ $(patsubst %.o, %.c, $@);

test: $(TEST_OBJS) $(DIST_OBJS);
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_DIR)/$@ $(TEST_OBJS) $(DIST_OBJS);

#------------------------------
# RELEASE
#------------------------------

release: C_FLAGS := -std=c99 -O2 -g -DNDDEBUG -Wall -Wextra
release: clean libc_errors.o libc_errors.so libc_errors.a app test;

clean:
	rm -f $(APP_OBJS) $(LIB_OBJS) $(TEST_OBJS) $(DIST_DIR)/* $(BIN_DIR)/*;
