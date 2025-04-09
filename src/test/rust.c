#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../lib/c_rust.h"
#include "./utils.h"

// ####################
// ERROR
// ####################
Test(error_new, valid_input) {
  Error err = error_new(-1, "test error");
  cr_assert_eq(err.code, -1);
  cr_assert_str_eq(err.message, "test error");
}

Test(error_new, empty_message) {
  Error err = error_new(-1, "");
  cr_assert_eq(err.code, -1);
  cr_assert_str_eq(err.message, "");
}

Test(std_error_new, basic) {
  errno = ENOENT;  // Set a specific error
  Error err = std_error_new();
  cr_assert_eq(err.code, ENOENT);
  cr_assert_str_eq(err.message, strerror(ENOENT));
}

// ####################
// RESULT
// ####################
Test(result_ok, _1) {
  char* value = "ok";
  Result actual = result_ok(value);
  Result expected = {{value}, {0, NULL}};

  cr_assert(result_is_ok(&actual));
  cr_assert(not(result_is_void(&actual)));
  cr_assert(not(result_is_error(&actual)));
  cr_assert_eq(strcmp((char*) actual.ok.value, (char*) expected.ok.value), 0);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(actual.error.message, expected.error.message);
}

Test(result_void, _1) {
  Result actual = result_void();
  Result expected = {{"()"}, {0, NULL}};

  cr_assert(result_is_ok(&actual));
  cr_assert(result_is_void(&actual));
  cr_assert(not(result_is_error(&actual)));
  cr_assert_eq(strcmp((char*) actual.ok.value, (char*) expected.ok.value), 0);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(actual.error.message, expected.error.message);
}

Test(result_error, _1) {
  Result actual = result_error(ERROR_CODE_GENERAL, ERROR_MESSAGE_GENERAL);
  Result expected = {{NULL}, error_new(ERROR_CODE_GENERAL, ERROR_MESSAGE_GENERAL)};

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
  Result expected = {{NULL}, std_error_new()};

  cr_assert(not(result_is_ok(&actual)));
  cr_assert(not(result_is_void(&actual)));
  cr_assert(result_is_error(&actual));
  cr_assert_eq(actual.ok.value, expected.ok.value);
  cr_assert_eq(actual.error.code, expected.error.code);
  cr_assert_eq(strcmp(actual.error.message, expected.error.message), 0);
}

Test(result_ok, valid_value) {
  int value = 42;
  Result res = result_ok(&value);
  cr_assert(result_is_ok(&res));
  cr_assert_eq(*(int*)res.ok.value, 42);
}

Test(result_ok, empty_string) {
  char* empty = "";
  Result res = result_ok(empty);
  cr_assert(result_is_ok(&res));
  cr_assert_str_eq((char*)res.ok.value, "");
}

Test(result_void, basic) {
  Result res = result_void();
  cr_assert(result_is_ok(&res));
  cr_assert(result_is_void(&res));
  cr_assert_str_eq((char*)res.ok.value, "()");
}

Test(result_error, valid_input) {
  Result res = result_error(-1, "test error");
  cr_assert(result_is_error(&res));
  cr_assert_eq(res.error.code, -1);
  cr_assert_str_eq(res.error.message, "test error");
}

Test(result_std_error, basic) {
  errno = ENOENT;  // Set a specific error
  Result res = result_std_error();
  cr_assert(result_is_error(&res));
  cr_assert_eq(res.error.code, ENOENT);
  cr_assert_str_eq(res.error.message, strerror(ENOENT));
}

Test(result_is_ok, valid_result) {
  int value = 42;
  Result res = result_ok(&value);
  cr_assert(result_is_ok(&res));
}

Test(result_is_ok, error_result) {
  Result res = result_error(-1, "test error");
  cr_assert(not(result_is_ok(&res)));
}

Test(result_is_ok, void_result) {
  Result res = result_void();
  cr_assert(result_is_ok(&res));
}

Test(result_is_void, valid_void) {
  Result res = result_void();
  cr_assert(result_is_void(&res));
}

Test(result_is_void, non_void) {
  int value = 42;
  Result res = result_ok(&value);
  cr_assert(not(result_is_void(&res)));
}

Test(result_is_void, error_result) {
  Result res = result_error(-1, "test error");
  cr_assert(not(result_is_void(&res)));
}

Test(result_is_error, valid_error) {
  Result res = result_error(-1, "test error");
  cr_assert(result_is_error(&res));
}

Test(result_is_error, ok_result) {
  int value = 42;
  Result res = result_ok(&value);
  cr_assert(not(result_is_error(&res)));
}

Test(result_is_error, void_result) {
  Result res = result_void();
  cr_assert(not(result_is_error(&res)));
}

Test(result_is_void, edge_cases) {
  Result res1 = {{NULL}, {0, NULL}};
  cr_assert(not(result_is_void(&res1)));
  
  Result res2 = {{""}, {0, NULL}};
  cr_assert(not(result_is_void(&res2)));
  
  Result res3 = {{"test"}, {0, NULL}};
  cr_assert(not(result_is_void(&res3)));
  
  Result res4 = result_error(ERROR_CODE_GENERAL, "test");
  cr_assert(not(result_is_void(&res4)));
}

// ####################
// OPTION
// ####################
Test(option_some, _1) {
  char* msg = "ok";
  Option actual = option_some(&msg);
  Option expected = {&msg};

  cr_assert(option_is_some(&actual));
  cr_assert(not(option_is_none(&actual)));
  cr_assert_eq(strcmp((char*) actual.value, (char*) expected.value), 0);
}

Test(option_none, _1) {
  Option actual = option_none();

  cr_assert(not(option_is_some(&actual)));
  cr_assert(option_is_none(&actual));
}

Test(option_some, valid_value) {
  int value = 42;
  Option opt = option_some(&value);
  cr_assert(option_is_some(&opt));
  cr_assert_eq(*(int*)opt.value, 42);
}

Test(option_some, empty_string) {
  char* empty = "";
  Option opt = option_some(empty);
  cr_assert(option_is_some(&opt));
  cr_assert_str_eq((char*)opt.value, "");
}

Test(option_none, basic) {
  Option opt = option_none();
  cr_assert(option_is_none(&opt));
  cr_assert_eq(opt.value, NULL);
}

Test(option_is_some, some_value) {
  int value = 42;
  Option opt = option_some(&value);
  cr_assert(option_is_some(&opt));
}

Test(option_is_some, none_value) {
  Option opt = option_none();
  cr_assert(not(option_is_some(&opt)));
}

Test(option_is_none, none_value) {
  Option opt = option_none();
  cr_assert(option_is_none(&opt));
}

Test(option_is_none, some_value) {
  int value = 42;
  Option opt = option_some(&value);
  cr_assert(not(option_is_none(&opt)));
} 
