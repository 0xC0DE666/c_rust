#include <errno.h>
#include <string.h>

#include "./c_rust.h"

// ####################
// RESULT
// ####################
const Ok OK_UNIT = (Ok) {OK_CODE_GENERAL, OK_MSG_GENERAL};
const Ok OK_NONE = (Ok) {OK_CODE_NONE, OK_MSG_NONE};

Ok new_ok(const size_t size, void* value) {
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
  return (Result) { new_ok(size, value), ERROR_NONE };
}

Result result_unit() {
  return (Result) { OK_UNIT, ERROR_NONE };
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
    result->error.code == ERR_CODE_NONE &&
    strcmp(result->error.message, ERR_MSG_NONE) == 0;
}

bool result_is_unit(const Result* result) {
  return result->ok.size == OK_UNIT.size &&
    strcmp((char*) result->ok.value, OK_MSG_GENERAL) == 0 &&
    result->error.code == ERR_CODE_NONE &&
    strcmp(result->error.message, ERR_MSG_NONE) == 0;
}

bool result_is_error(const Result* result) {
  return result->ok.size == OK_NONE.size &&
    strcmp((char*) result->ok.value, OK_MSG_NONE) == 0 &&
    result->error.code != ERR_CODE_NONE &&
    strcmp(result->error.message, ERR_MSG_NONE) != 0;
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
