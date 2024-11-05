#include <stdlib.h>
#include <stdbool.h>

#include "./c_errors.h"

Error* new_error(int code, char* message) {
  Error* err = malloc(sizeof(Error));
  err->code = code;
  err->message = message;
  return err;
}

void free_error(Error** error) {
  free(*error);
  *error = NULL;
}

Result result_ok(void* value) {
  return (Result) { value, NULL };
}

Result result_error(int code, char* message) {
  return (Result) { NULL, new_error(code, message) };
}
