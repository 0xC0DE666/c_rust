VERSION := 0.0.0
NAME := libc_errors
QUALIFIER := $(NAME)-$(VERSION)

CC := gcc
C_FLAGS := -std=c99 -g -Wall -Wextra

OBJ_DIR := ./obj
BIN_DIR := ./bin

all: clean app test libc_errors.so libc_errors.a;

clean:
	rm -f ./src/main.o $(LIB_OBJS) $(TEST_OBJS) $(OBJ_DIR)/* $(BIN_DIR)/*;

#------------------------------
# LIB
#------------------------------

LIB_DIR := ./src/lib
LIB_SRCS = $(wildcard $(LIB_DIR)/*.c)
LIB_HDRS = $(wildcard $(LIB_DIR)/*.h)
LIB_OBJS := $(patsubst %.c, %.o, $(LIB_SRCS))

$(LIB_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.c, %.o, $@) $@;

libc_errors.o: $(LIB_OBJS);
	ld -relocatable -o $(OBJ_DIR)/$@ $(LIB_OBJS);

libc_errors.so: $(LIB_OBJS) $(LIB_HDRS);
	$(CC) $(C_FLAGS) -fPIC -shared -lc -o $(OBJ_DIR)/$@ $(LIB_OBJS);

libc_errors.a: $(LIB_OBJS);
	ar rcs $(OBJ_DIR)/$@ $(LIB_OBJS);

main.o:
	$(CC) $(C_FLAGS) -c -o ./src/main.o ./src/main.c;

app: main.o libc_errors.o;
	$(CC) $(C_FLAGS) -o $(BIN_DIR)/$@ ./src/main.o $(OBJ_DIR)/libc_errors.o;


#------------------------------
# TESTS
#------------------------------

TEST_DIR := ./src/test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_HDRS = $(wildcard $(TEST_DIR)/*.h)
TEST_OBJS := $(patsubst %.c, %.o, $(TEST_SRCS))

$(TEST_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.c, %.o, $@) $@;

test: $(LIB_OBJS) $(TEST_OBJS);
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_DIR)/$@ $(LIB_OBJS) $(TEST_OBJS);


#------------------------------
# RELEASE
#------------------------------

release: C_FLAGS := -std=c99 -O2 -g -DNDDEBUG -Wall -Wextra
release: clean app test libc_errors.so;
