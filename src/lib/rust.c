#include <errno.h>
#include <string.h>

#include "./c_rust.h"

Error error_new(int code, char* message) {
  return (Error) {code, message != NULL ? message : ERR_MSG_BLANK};
}

Error std_error_new() {
  return error_new(errno, strerror(errno));
}

Result result_new_ok(void* value) {
  return (Result) { value, error_new(OK_CODE_GENERAL, OK_MSG_GENERAL) };
}

Result result_new_error(int code, char* message) {
  return (Result) { NULL, error_new(code, message) };
}

Result result_std_error() {
  return (Result) { NULL, std_error_new() };
}

bool result_is_ok(const Result* result) {
  return result->ok != NULL && result->error.code == OK_CODE_GENERAL;
}

bool result_is_error(const Result* result) {
  return result->ok == NULL && result->error.code != OK_CODE_GENERAL;
}
