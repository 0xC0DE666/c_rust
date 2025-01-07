#include <errno.h>
#include <string.h>

#include "./c_rust.h"

// ####################
// ERROR
// ####################
const Error NO_ERROR = (Error) {OK_CODE_GENERAL, ""};

Error error_new(int code, char* message) {
  return (Error) {code, message != NULL ? message : ERR_MSG_BLANK};
}

Error std_error_new() {
  return error_new(errno, strerror(errno));
}

// ####################
// RESULT
// ####################
Result result_ok(void* value) {
  return (Result) { value, NO_ERROR };
}

Result result_error(int code, char* message) {
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

// ####################
// OPTION
// ####################
Option option_some(void* value) {
  return (Option) { value, NULL };
}

Option option_none() {
  return (Option) { NULL, NULL };
}

bool option_is_some(const Option* option) {
  return option->some != NULL && option->none == NULL;
}

bool option_is_none(const Option* option) {
  return option->some == NULL && option->none == NULL;
}
