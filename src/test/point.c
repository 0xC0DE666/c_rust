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


// Test error_new with NULL message
Test(error_new, null_message) {
  Error err = error_new(ERROR_CODE_GENERAL, NULL);
  cr_assert_eq(err.code, ERROR_CODE_GENERAL);
  cr_assert_str_eq(err.message, ERROR_MESSAGE_GENERAL);
}

// Test result_is_void with various inputs
Test(result_is_void, edge_cases) {
  // Test with NULL value
  Result res1 = {ok_new(NULL), ERROR_NULL};
  cr_assert(not(result_is_void(&res1)));
  
  // Test with empty string
  Result res2 = {ok_new(""), ERROR_NULL};
  cr_assert(result_is_void(&res2));
  
  // Test with non-empty string
  Result res3 = {ok_new("test"), ERROR_NULL};
  cr_assert(not(result_is_void(&res3)));
  
  // Test with error
  Result res4 = result_error(ERROR_CODE_GENERAL, "test");
  cr_assert(not(result_is_void(&res4)));
}

// Test ok_new with NULL value
Test(ok_new, null_value) {
  Ok ok = ok_new(NULL);
  cr_assert_eq(ok.value, NULL);
}

// Test some_new with NULL value
Test(some_new, null_value) {
  Some some = some_new(NULL);
  cr_assert_eq(some.value, NULL);
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