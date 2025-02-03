#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/c_rust.h"

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
