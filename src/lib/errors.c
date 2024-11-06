#include <stdlib.h>
#include <stdbool.h>

#include "./c_errors.h"

Error* error_new(int code, char* message) {
  Error* err = malloc(sizeof(Error));
  err->code = code;
  err->message = message;
  return err;
}

void error_free(Error** error) {
  free(*error);
  *error = NULL;
}

Result result_ok(void* value) {
  return (Result) { value, NULL };
}

Result result_error(int code, char* message) {
  return (Result) { NULL, error_new(code, message) };
}
