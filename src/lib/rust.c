#include <errno.h>
#include <string.h>

#include "./c_rust.h"

// ####################
// RESULT
// ####################
const Ok OK_VOID = (Ok) {OK_SIZE_VOID, OK_VALUE_VOID};
const Ok OK_NULL = (Ok) {OK_SIZE_NULL, OK_VALUE_NULL};

Ok ok_new(const size_t size, void* value) {
  return (Ok) {size, value};
}

const Error ERROR_NULL = (Error) {ERROR_CODE_NULL, ERROR_MESSAGE_NULL};

Error error_new(const int code, const char* message) {
  return (Error) {code, message != NULL ? message : ERROR_MESSAGE_GENERAL};
}

Error std_error_new() {
  return error_new(errno, strerror(errno));
}

Result result_ok(size_t size, void* value) {
  return (Result) {ok_new(size, value), ERROR_NULL};
}

Result result_void() {
  return (Result) {OK_VOID, ERROR_NULL};
}

Result result_error(const int code, const char* message) {
  return (Result) {OK_NULL, error_new(code, message)};
}

Result result_std_error() {
  return (Result) {OK_NULL, std_error_new()};
}

bool result_is_ok(const Result* result) {
  return result->ok.size > 0 &&
    result->ok.value != NULL &&
    result->error.code == ERROR_NULL.code &&
    result->error.message == ERROR_NULL.message;
}

bool result_is_void(const Result* result) {
  return result->ok.size == OK_VOID.size &&
    strcmp((char*) result->ok.value, (char*) OK_VOID.value) == 0 &&
    result->error.code == ERROR_NULL.code &&
    result->error.message == ERROR_NULL.message;
}

bool result_is_error(const Result* result) {
  return result->ok.size == OK_NULL.size &&
    result->ok.value == OK_NULL.value &&
    result->error.code != ERROR_NULL.code &&
    result->error.message != ERROR_NULL.message;
}

// ####################
// OPTION
// ####################
Some some_new(size_t size, void* value) {
  return (Some) {size, value};
}

const Some SOME_NULL = {SOME_SIZE_NULL, SOME_VALUE_NULL};

const None NONE = {};
const None NONE_NULL = {};

Option option_some(size_t size, void* value) {
  return (Option) {some_new(size, value), NONE_NULL};
}

Option option_none() {
  return (Option) {SOME_NULL, NONE};
}

bool option_is_some(const Option* option) {
  return option->some.size > 0 && option->some.value != NULL;
}

bool option_is_none(const Option* option) {
  return option->some.size == SOME_NULL.size && option->some.value == SOME_NULL.value;
}
