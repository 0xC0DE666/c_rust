#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_rust.h"
#include "./utils.h"

// ####################
// error_new
// ####################
Test(error_new, _1) {
  Error actual = error_new(ERR_CODE_GENERAL, NULL);
  cr_assert_eq(actual.code, ERR_CODE_GENERAL);
  cr_assert_eq(strcmp(actual.message, ERR_MSG_BLANK), 0);

  actual = error_new(ERR_CODE_GENERAL, "error");
  Error expected = {ERR_CODE_GENERAL, "error"};

  cr_assert_eq(actual.code, expected.code);
  cr_assert_eq(strcmp(actual.message, expected.message), 0);
}

// ####################
// std_error_new
// ####################
Test(std_error_new, _1) {
  Error actual = file_404();
  Error expected = {ENOENT, strerror(errno)};

  cr_assert_eq(actual.code, expected.code);
  cr_assert_eq(strcmp(actual.message, expected.message), 0);
}

// ####################
// result_ok
// ####################
Test(result_ok, _1) {
  char* msg = "ok";
  Result actual = result_new_ok(&msg);
  Result expected = (Result) {&msg, {OK_CODE_GENERAL, OK_MSG_GENERAL}};

  cr_assert_eq(actual.error.code, 0);
  cr_assert_eq(strcmp(actual.error.message, OK_MSG_GENERAL), 0);
  cr_assert_eq(actual.ok != NULL, true);
  cr_assert_eq(strcmp(actual.ok, expected.ok), 0);
  cr_assert_eq(result_is_ok(&actual), true);
  cr_assert_eq(result_is_error(&actual), false);
}

// ####################
// result_error
// ####################
Test(result_error, _1) {
  Error err = error_new(ERR_CODE_GENERAL, "error");
  Result actual = result_new_error(ERR_CODE_GENERAL, "error");
  Result expected = {NULL, err};

  cr_assert_eq(actual.ok, NULL);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(strcmp(actual.error.message, expected.error.message), 0);
  cr_assert_eq(result_is_ok(&actual), false);
  cr_assert_eq(result_is_error(&actual), true);
}

// ####################
// result_std_error
// ####################
Test(result_std_error, _1) {
  file_404();
  Result actual = result_std_error();
  Result expected = {NULL, {ENOENT, strerror(errno)}};

  cr_assert_eq(actual.ok, NULL);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(strcmp(actual.error.message, expected.error.message), 0);
  cr_assert_eq(result_is_ok(&actual), false);
  cr_assert_eq(result_is_error(&actual), true);
}
