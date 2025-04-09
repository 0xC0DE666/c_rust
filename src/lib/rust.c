#include <assert.h>
#include <errno.h>
#include <string.h>

#include "./c_rust.h"

// ####################
// RESULT
// ####################
Error error_new(const int code, const char* message) {
  assert(code != 0 && message != NULL);
  return (Error) {code, message};
}

Error std_error_new() {
  return error_new(errno, strerror(errno));
}

Result result_ok(void* value) {
  assert(value != NULL);
  return (Result) {{value}, {0, NULL}};
}

Result result_void() {
  return (Result) {{"()"}, {0, NULL}};
}

Result result_error(const int code, const char* message) {
  return (Result) {{NULL}, error_new(code, message)};
}

Result result_std_error() {
  return (Result) {{NULL}, std_error_new()};
}

bool result_is_ok(const Result* result) {
  assert(result != NULL);
  return result->ok.value != NULL &&
    result->error.code == 0 &&
    result->error.message == NULL;
}

bool result_is_void(const Result* result) {
  assert(result != NULL);
  return result->ok.value != NULL &&
    strcmp((char*) result->ok.value, "()") == 0 &&
    result->error.code == 0 &&
    result->error.message == NULL;
}

bool result_is_error(const Result* result) {
  assert(result != NULL);
  return result->ok.value == NULL &&
    result->error.code != 0 &&
    result->error.message != NULL;
}

// ####################
// OPTION
// ####################
Option option_some(void* value) {
	assert(value != NULL);
  return (Option) {value};
}

Option option_none() {
  return (Option) {NULL};
}

bool option_is_some(const Option* option) {
	assert(option != NULL);
  return option->value != NULL;
}

bool option_is_none(const Option* option) {
	assert(option != NULL);
  return option->value == NULL;
}
