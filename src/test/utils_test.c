#include <stdlib.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "utils.h"

// ####################
// ptr_free
// ####################
Test(ptr_free, _1) {
  char* str = malloc(sizeof(char) * 10);

  cr_assert_eq(str != NULL, true);

  ptr_free((void**) &str);

  cr_assert_eq(str == NULL, true);
}


// ####################
// point_new
// ####################
Test(point_new, _1) {
  Point* point = point_new(0, 1);

  cr_assert_eq(point->x, 0);
  cr_assert_eq(point->y, 1);

  point_free(&point);
}

// ####################
// point_free
// ####################
Test(point_free, _1) {
  Point* point = point_new(5, 1);

  point_free(&point);

  cr_assert_eq(point, NULL);
}

// ####################
// point_to_str
// ####################
Test(point_to_str, _1) {
  Point* point = point_new(3, 2);
  
  char* result = point_to_string(point);
  char* expected = "(3, 2)";

  cr_assert_eq(strcmp(result, expected), 0);

  free(result);
  point_free(&point);
}

// ####################
// point_double
// ####################
Test(point_double, _1) {
  Point* point = point_new(3, 2);
  
  point_double(point);

  cr_assert_eq(point->x, 6);
  cr_assert_eq(point->y, 4);

  point_free(&point);
}
