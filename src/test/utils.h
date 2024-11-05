#ifndef UTILS_H
#define UTILS_H

  void ptr_free(void** ptr);

  typedef struct {
    int x;
    int y;
  } Point;


  Point* point_new(int x, int y);
  void point_free(Point** const point);
  char* point_to_string(Point* const point);
  void point_double(Point* const point);

  void print_string(char* str);
  void* nothing(void* v);

#endif
