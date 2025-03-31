#include <stdio.h>
#include <string.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../lib/c_rust.h"

typedef struct Point {
  int x;
  int y;
} Point;

Result point_new(int x, int y) {
  Point* p;
  p = malloc(sizeof(*p));
  if (p == NULL) {
    return result_error(ERROR_CODE_GENERAL, "Failed to allocate Point");
  }
  p->x = x;
  p->y = y;
  return result_ok(p);
}

// Test Point creation with valid values
Test(point_new, valid_values) {
  Result res = point_new(2, 3);
  cr_assert(result_is_ok(&res));
  
  Point* p = res.ok.value;
  cr_assert_eq(p->x, 2);
  cr_assert_eq(p->y, 3);
  
  free(p);
}

// Test error propagation pattern
Test(error_propagation, example) {
  Result res1 = point_new(2, 3);
  cr_assert(result_is_ok(&res1));
  
  Point* p = res1.ok.value;
  Result res2 = point_new(p->x * 2, p->y * 2);
  free(p);
  
  cr_assert(result_is_ok(&res2));
  p = res2.ok.value;
  cr_assert_eq(p->x, 4);
  cr_assert_eq(p->y, 6);
  free(p);
} 
