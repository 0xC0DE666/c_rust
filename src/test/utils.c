#include <stdio.h>

#include "../lib/c_rust.h"

Error file_404() {
  fopen("nonexistent_file.txt", "r");
  return std_error_new(); 
}
