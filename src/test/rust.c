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
Test(error_new, _1) {
  Error actual1 = error_new(ERR_CODE_GENERAL, NULL);
  cr_assert_eq(actual1.code, ERR_CODE_GENERAL);
  cr_assert_eq(strcmp(actual1.message, ERR_MSG_BLANK), 0);

  Error actual2 = error_new(ERR_CODE_GENERAL, "error");
  Error expected = {ERR_CODE_GENERAL, "error"};

  cr_assert_eq(actual2.code, expected.code);
  cr_assert_eq(strcmp(actual2.message, expected.message), 0);
}

Test(std_error_new, _1) {
  Error actual = file_404();
  Error expected = {ENOENT, strerror(errno)};

  cr_assert_eq(actual.code, expected.code);
  cr_assert_eq(strcmp(actual.message, expected.message), 0);
}

Test(result_ok, _1) {
  char* msg = "ok";
  Result actual = result_ok(&msg);
  Result expected = (Result) {&msg, {OK_CODE_GENERAL, OK_MSG_GENERAL}};

  cr_assert_eq(result_is_ok(&actual), true);
  cr_assert_eq(result_is_error(&actual), false);
  cr_assert_eq(strcmp(actual.ok, expected.ok), 0);
}

Test(result_ok, _2) {
  Result actual = result_unit();
  Result expected = (Result) {&OK, {OK_CODE_GENERAL, OK_MSG_GENERAL}};

  cr_assert_eq(result_is_ok(&actual), true);
  cr_assert_eq(result_is_error(&actual), false);
  cr_assert_eq(actual.ok, expected.ok);
}

Test(result_error, _1) {
  Error err = error_new(ERR_CODE_GENERAL, "error");
  Result actual = result_error(ERR_CODE_GENERAL, "error");
  Result expected = {NULL, err};

  cr_assert_eq(result_is_ok(&actual), false);
  cr_assert_eq(result_is_error(&actual), true);
  cr_assert_eq(strcmp(actual.error.message, expected.error.message), 0);
}

Test(result_std_error, _1) {
  file_404();
  Result actual = result_std_error();
  Result expected = {NULL, {ENOENT, strerror(errno)}};

  cr_assert_eq(result_is_ok(&actual), false);
  cr_assert_eq(result_is_error(&actual), true);
  cr_assert_eq(strcmp(actual.error.message, expected.error.message), 0);
}

// ####################
// OPTION
// ####################
Test(option_some, _1) {
  char* msg = "ok";
  Option actual = option_some(&msg);
  Option expected = (Option) {&msg, NULL};

  cr_assert_eq(option_is_some(&actual), true);
  cr_assert_eq(option_is_none(&actual), false);
  cr_assert_eq(strcmp((char*) actual.some, (char*) expected.some), 0);
}

Test(option_none, _1) {
  Option actual = option_none();

  cr_assert_eq(option_is_some(&actual), false);
  cr_assert_eq(option_is_none(&actual), true);
}
