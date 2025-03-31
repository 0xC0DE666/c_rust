#include <errno.h>
#include <string.h>

#include "./c_rust.h"

// ####################
// RESULT
// ####################
const Ok OK_VOID = (Ok) {OK_VALUE_VOID};
const Ok OK_NONE = (Ok) {OK_VALUE_NONE};

Ok ok_new(void* value) {
  return (Ok) {value};
}

const Error ERROR_NONE = (Error) {ERROR_CODE_NONE, ERROR_MESSAGE_NONE};

Error error_new(const int code, const char* message) {
  return (Error) {code, message != NULL ? message : ERROR_MESSAGE_GENERAL};
}

Error std_error_new() {
  return error_new(errno, strerror(errno));
}

Result result_ok(void* value) {
  return (Result) {ok_new(value), ERROR_NONE};
}

Result result_void() {
  return (Result) {OK_VOID, ERROR_NONE};
}

Result result_error(const int code, const char* message) {
  return (Result) {OK_NONE, error_new(code, message)};
}

Result result_std_error() {
  return (Result) {OK_NONE, std_error_new()};
}

bool result_is_ok(const Result* result) {
  return result->ok.value != OK_NONE.value &&
    result->error.code == ERROR_NONE.code &&
    result->error.message == ERROR_NONE.message;
}

bool result_is_void(const Result* result) {
  return result->ok.value != OK_NONE.value &&
    strcmp((char*) result->ok.value, (char*) OK_VOID.value) == 0 &&
    result->error.code == ERROR_NONE.code &&
    result->error.message == ERROR_NONE.message;
}

bool result_is_error(const Result* result) {
  return result->ok.value == OK_NONE.value &&
    result->error.code != ERROR_NONE.code &&
    result->error.message != ERROR_NONE.message;
}

// ####################
// OPTION
// ####################
Some some_new(void* value) {
  return (Some) {value};
}

const Some SOME_NONE = {SOME_VALUE_NONE};

const None NONE = {};
const None NONE_NONE = {};

Option option_some(void* value) {
  return (Option) {some_new(value), NONE_NONE};
}

Option option_none() {
  return (Option) {SOME_NONE, NONE};
}

bool option_is_some(const Option* option) {
  return option->some.value != SOME_NONE.value;
}

bool option_is_none(const Option* option) {
  return option->some.value == SOME_NONE.value;
}
