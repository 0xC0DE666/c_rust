#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../lib/c_rust.h"
#include "./utils.h"

// ####################
// RESULT
// ####################
Test(result_ok, _1) {
  char* value = "ok";
  Result actual = result_ok(value);
  Result expected = {ok_new(value), ERROR_NONE};

  cr_assert(result_is_ok(&actual));
  cr_assert(not(result_is_void(&actual)));
  cr_assert(not(result_is_error(&actual)));
  cr_assert_eq(strcmp((char*) actual.ok.value, (char*) expected.ok.value), 0);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(actual.error.message, expected.error.message);
}

Test(result_void, _1) {
  Result actual = result_void();
  Result expected = {OK_VOID, ERROR_NONE};

  cr_assert(result_is_ok(&actual));
  cr_assert(result_is_void(&actual));
  cr_assert(not(result_is_error(&actual)));
  cr_assert_eq(strcmp((char*) actual.ok.value, (char*) expected.ok.value), 0);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(actual.error.message, expected.error.message);
}

Test(result_error, _1) {
  Result actual = result_error(ERROR_CODE_GENERAL, ERROR_MESSAGE_GENERAL);
  Result expected = {OK_NONE, error_new(ERROR_CODE_GENERAL, ERROR_MESSAGE_GENERAL)};

  cr_assert(not(result_is_ok(&actual)));
  cr_assert(not(result_is_void(&actual)));
  cr_assert(result_is_error(&actual));
  cr_assert_eq(actual.ok.value, expected.ok.value);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(strcmp(actual.error.message, expected.error.message), 0);
}

Test(result_std_error, _1) {
  file_404();
  Result actual = result_std_error();
  Result expected = {OK_NONE, std_error_new()};

  cr_assert(not(result_is_ok(&actual)));
  cr_assert(not(result_is_void(&actual)));
  cr_assert(result_is_error(&actual));
  cr_assert_eq(actual.ok.value, expected.ok.value);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(strcmp(actual.error.message, expected.error.message), 0);
}

// Test error_new with NULL message
Test(error_new, null_message) {
  Error err = error_new(ERROR_CODE_GENERAL, NULL);
  cr_assert_eq(err.code, ERROR_CODE_GENERAL);
  cr_assert_str_eq(err.message, ERROR_MESSAGE_GENERAL);
}

// Test result_is_void with various inputs
Test(result_is_void, edge_cases) {
  // Test with NULL value
  Result res1 = {ok_new(NULL), ERROR_NONE};
  cr_assert(not(result_is_void(&res1)));
  
  // Test with empty string
  Result res2 = {ok_new(""), ERROR_NONE};
  cr_assert(result_is_void(&res2));
  
  // Test with non-empty string
  Result res3 = {ok_new("test"), ERROR_NONE};
  cr_assert(not(result_is_void(&res3)));
  
  // Test with error
  Result res4 = result_error(ERROR_CODE_GENERAL, "test");
  cr_assert(not(result_is_void(&res4)));
}

// Test ok_new with NULL value
Test(ok_new, null_value) {
  Ok ok = ok_new(NULL);
  cr_assert_eq(ok.value, NULL);
}

// ####################
// OPTION
// ####################
Test(option_some, _1) {
  char* msg = "ok";
  Option actual = option_some(&msg);
  Option expected = {some_new(&msg), NONE_NONE};

  cr_assert(option_is_some(&actual));
  cr_assert(not(option_is_none(&actual)));
  cr_assert_eq(strcmp((char*) actual.some.value, (char*) expected.some.value), 0);
}

Test(option_none, _1) {
  Option actual = option_none();

  cr_assert(not(option_is_some(&actual)));
  cr_assert(option_is_none(&actual));
}

// Test some_new with NULL value
Test(some_new, null_value) {
  Some some = some_new(NULL);
  cr_assert_eq(some.value, NULL);
}
