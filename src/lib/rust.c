#include <errno.h>
#include <string.h>

#include "./c_rust.h"

// ####################
// RESULT
// ####################
const Ok OK_VOID = (Ok) {OK_CODE_VOID, OK_MSG_VOID};
const Ok OK_NONE = (Ok) {OK_CODE_NONE, OK_MSG_NONE};

Ok ok_new(const size_t size, void* value) {
  return (Ok) {size, value};
}

const Error ERROR_NONE = (Error) {ERR_CODE_NONE, ERR_MSG_NONE};

Error error_new(const int code, const char* message) {
  return (Error) {code, message != NULL ? message : ERR_MSG_BLANK};
}

Error std_error_new() {
  return error_new(errno, strerror(errno));
}

Result result_ok(size_t size, void* value) {
  return (Result) { ok_new(size, value), ERROR_NONE };
}

Result result_void() {
  return (Result) { OK_VOID, ERROR_NONE };
}

Result result_error(const int code, const char* message) {
  return (Result) { OK_NONE, error_new(code, message) };
}

Result result_std_error() {
  return (Result) { OK_NONE, std_error_new() };
}

bool result_is_ok(const Result* result) {
  return result->ok.size > 0 &&
    result->ok.value != NULL &&
    result->error.code == ERROR_NONE.code &&
    strcmp(result->error.message, ERROR_NONE.message) == 0;
}

bool result_is_void(const Result* result) {
  return result->ok.size == OK_VOID.size &&
    strcmp((char*) result->ok.value, (char*) OK_VOID.value) == 0 &&
    result->error.code == ERROR_NONE.code &&
    strcmp(result->error.message, ERROR_NONE.message) == 0;
}

bool result_is_error(const Result* result) {
  return result->ok.size == OK_NONE.size &&
    strcmp((char*) result->ok.value, (char*) OK_NONE.value) == 0 &&
    result->error.code != ERROR_NONE.code &&
    strcmp(result->error.message, ERROR_NONE.message) != 0;
}

// ####################
// OPTION
// ####################
const None NONE = {};

Option option_some(void* value) {
  return (Option) { value, NULL };
}

Option option_none() {
  return (Option) { NULL, &NONE };
}

bool option_is_some(const Option* option) {
  return option->some != NULL && option->none == NULL;
}

bool option_is_none(const Option* option) {
  return option->some == NULL && option->none == &NONE;
}
