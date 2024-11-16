#include <stdio.h>

#include "../lib/c_errors.h"

Error file_404() {
  fopen("nonexistent_file.txt", "r");
  return std_error_new(); 
}
