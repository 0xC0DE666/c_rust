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
Test(ok_new, _1) {
  char* value = "ok";
  Ok actual = ok_new(sizeof(*value), value);
  Ok expected = {sizeof(*value), value};

  cr_assert_eq(actual.size, expected.size);
  cr_assert_eq(strcmp((char*) actual.value, (char*) expected.value), 0);
}

Test(error_new, _1) {
  Error actual1 = error_new(ERROR_CODE_GENERAL, NULL);
  Error expected1 = {ERROR_CODE_GENERAL, ERROR_MESSAGE_GENERAL};

  cr_assert_eq(actual1.code, expected1.code);
  cr_assert_eq(strcmp(actual1.message, expected1.message), 0);

  Error actual2 = error_new(ERROR_CODE_GENERAL, ERROR_MESSAGE_GENERAL);
  Error expected2 = {ERROR_CODE_GENERAL, ERROR_MESSAGE_GENERAL};

  cr_assert_eq(actual2.code, expected2.code);
  cr_assert_eq(strcmp(actual2.message, expected2.message), 0);
}

Test(std_error_new, _1) {
  Error actual = file_404();
  Error expected = {ENOENT, strerror(errno)};

  cr_assert_eq(actual.code, expected.code);
  cr_assert_eq(strcmp(actual.message, expected.message), 0);
}

Test(result_ok, _1) {
  char* value = "ok";
  size_t size = sizeof(*value);
  Result actual = result_ok(size, value);
  Result expected = (Result) {ok_new(size, value), ERROR_NONE};

  cr_assert(result_is_ok(&actual));
  cr_assert_eq(actual.ok.size, size);
  cr_assert_eq(strcmp((char*) actual.ok.value, (char*) expected.ok.value), 0);
  cr_assert_eq(actual.error.code, ERROR_NONE.code);
  cr_assert_eq(strcmp(actual.error.message, ERROR_NONE.message), 0);
}

Test(result_void, _1) {
  Result actual = result_void();

  cr_assert(result_is_void(&actual));
  cr_assert_eq(actual.ok.size, OK_VOID.size);
  cr_assert_eq(strcmp((char*) actual.ok.value, (char*) OK_VOID.value), 0);
  cr_assert_eq(actual.error.code, ERROR_NONE.code);
  cr_assert_eq(strcmp(actual.error.message, ERROR_NONE.message), 0);
}

Test(result_error, _1) {
  Result actual = result_error(ERROR_CODE_GENERAL, ERROR_MESSAGE_GENERAL);
  Result expected = {OK_NONE, error_new(ERROR_CODE_GENERAL, ERROR_MESSAGE_GENERAL)};

  cr_assert(result_is_error(&actual));
  cr_assert_eq(actual.ok.size, OK_NONE.size);
  cr_assert_eq(actual.ok.value, OK_NONE.value);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(strcmp(actual.error.message, expected.error.message), 0);
}

Test(result_std_error, _1) {
  file_404();
  Result actual = result_std_error();
  Result expected = {OK_NONE, {ENOENT, strerror(errno)}};

  cr_assert(result_is_error(&actual));
  cr_assert_eq(actual.ok.size, OK_NONE.size);
  cr_assert_eq(actual.ok.value, OK_NONE.value);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(strcmp(actual.error.message, expected.error.message), 0);
}

// ####################
// OPTION
// ####################
Test(option_some, _1) {
  char* msg = "ok";
  Option actual = option_some(sizeof(*msg), &msg);
  Option expected = {some_new(sizeof(*msg), &msg), NONE_SOME};

  cr_assert(option_is_some(&actual));
  cr_assert(option_is_none(&actual) == false);
  cr_assert_eq(strcmp((char*) actual.some.value, (char*) expected.some.value), 0);
}

Test(option_none, _1) {
  Option actual = option_none();

  cr_assert(option_is_some(&actual) == false);
  cr_assert(option_is_none(&actual));
}
