#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/c_rust.h"

/**
 * @brief Represents a 2D point with x and y coordinates
 */
typedef struct Point {
  int x;
  int y;
} Point;

/**
 * @brief Creates a new Point with the given coordinates
 * @param x The x coordinate
 * @param y The y coordinate
 * @return Result containing the new Point or an error if allocation fails
 */
Result point_new(int x, int y) {
  Point* p = malloc(sizeof(*p));
  if (p == NULL) {
    return result_error(ERROR_CODE_GENERAL, "Failed to allocate Point");
  }
  p->x = x;
  p->y = y;
  return result_ok(p);
}

/**
 * @brief Prints a Point to stdout
 * @param p The Point to print
 * @return Result indicating success or failure
 */
Result point_print(const Point* p) {
  if (p == NULL) {
    return result_error(ERROR_CODE_GENERAL, ERROR_MESSAGE_NULL_POINTER(point_print, p));
  }
  printf("(%d, %d)\n", p->x, p->y);
  return result_void();
}

int main() {
  // Example of error handling with Result type
  Result res = point_new(2, 2);
  if (result_is_error(&res)) {
    fprintf(stderr, "Error: %s\n", res.error.message);
    return 1;
  }
  
  Point* p = res.ok.value;
  Result print_res = point_print(p);
  if (result_is_error(&print_res)) {
    fprintf(stderr, "Error: %s\n", print_res.error.message);
    free(p);
    return 1;
  }
  
  free(p);
  return 0;
}
