#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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

void old_main() {
  printf("c_rust\n");
  printf("%lu\n", sizeof("abc\n"));
  char* msg = "abc\n";
  String s = string_new(5, msg);
  printf("%lu %s", s.size, s.value);
  printf("%p %p\n", msg, s.value);

  Error err = oh_snap();
  printf("%s\n", err.message);
}


typedef struct Point {
  int x;
  int y;
} Point;

Result point_new(int x, int y) {
  Point* p;
  p = malloc(sizeof(*p));
  p->x = x;
  p->y = y;
  return result_ok(p);
}

int main() {
  printf("%lu\n", sizeof(Ok));
  Result res = point_new(2, 2);
  Point* p = res.ok.value;
  printf("(%d, %d)\n", p->x, p->y);
  free(p);

  return 0;
}
