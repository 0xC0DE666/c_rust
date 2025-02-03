#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../lib/c_rust.h"

typedef struct String {
  size_t size;
  char* value;
} String;

String string_new(size_t size, char* actual) {
  char value[size] = {};
  memcpy(value, actual, size);
  String str = {};
  str.size = size;
  str.value = value;
  return str;
}

Error oh_snap() {
  return error_new(99, "Oh shit!\n");
}

int main() {
  printf("%lu\n", OK_SIZE_VOID);
  printf("c_rust\n");
  printf("%lu\n", sizeof("abc\n"));
  char* msg = "abc\n";
  String s = string_new(5, msg);
  printf("%lu %s", s.size, s.value);
  printf("%p %p\n", msg, s.value);

  Error err = oh_snap();
  printf("%s\n", err.message);
  return 0;
}
