#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_rust.h"
#include "./utils.h"

// ####################
// RESULT
// ####################
Test(result_ok, _1) {
  char* value = "ok";
  size_t size = sizeof(*value);
  Result actual = result_ok(size, value);
  Result expected = {ok_new(size, value), ERROR_NULL};

  cr_assert(result_is_ok(&actual));
  cr_assert_eq(actual.ok.size, size);
  cr_assert_eq(strcmp((char*) actual.ok.value, (char*) expected.ok.value), 0);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(actual.error.message, expected.error.message);
}

Test(result_void, _1) {
  Result actual = result_void();
  Result expected = {OK_VOID, ERROR_NULL};

  cr_assert(result_is_void(&actual));
  cr_assert_eq(actual.ok.size, expected.ok.size);
  cr_assert_eq(strcmp((char*) actual.ok.value, (char*) expected.ok.value), 0);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(actual.error.message, expected.error.message);
}

Test(result_error, _1) {
  Result actual = result_error(ERROR_CODE_GENERAL, ERROR_MESSAGE_GENERAL);
  Result expected = {OK_NULL, error_new(ERROR_CODE_GENERAL, ERROR_MESSAGE_GENERAL)};

  cr_assert(result_is_error(&actual));
  cr_assert_eq(actual.ok.size, expected.ok.size);
  cr_assert_eq(actual.ok.value, expected.ok.value);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(strcmp(actual.error.message, expected.error.message), 0);
}

Test(result_std_error, _1) {
  file_404();
  Result actual = result_std_error();
  Result expected = {OK_NULL, std_error_new()};

  cr_assert(result_is_error(&actual));
  cr_assert_eq(actual.ok.size, expected.ok.size);
  cr_assert_eq(actual.ok.value, expected.ok.value);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(strcmp(actual.error.message, expected.error.message), 0);
}

// ####################
// OPTION
// ####################
Test(option_some, _1) {
  char* msg = "ok";
  Option actual = option_some(sizeof(*msg), &msg);
  Option expected = {some_new(sizeof(*msg), &msg), NONE_NULL};

  cr_assert(option_is_some(&actual));
  cr_assert(option_is_none(&actual) == false);
  cr_assert_eq(strcmp((char*) actual.some.value, (char*) expected.some.value), 0);
}

Test(option_none, _1) {
  Option actual = option_none();

  cr_assert(option_is_some(&actual) == false);
  cr_assert(option_is_none(&actual));
}
