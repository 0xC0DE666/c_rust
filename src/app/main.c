#include <stdio.h>

#include "../lib/c_rust.h"

int main() {
  printf("c_errors\n");
  printf("%ld\n", sizeof(Error) * 8);
  printf("%ld\n", sizeof(void*) * 8);
  return 0;
}
