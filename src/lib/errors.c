#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "./c_errors.h"

Error* error_new(int code, char* message) {
  Error* err = malloc(sizeof(Error));
  *err = (Error){code, message};
  return err;
}

Error* std_error_new() {
  return error_new(errno, strerror(errno));
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

Result result_std_error() {
  return (Result) { NULL, std_error_new() };
}
