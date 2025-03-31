#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../lib/c_rust.h"

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

// ####################
// OPTION
// ####################
Test(option_some, valid_value) {
  int value = 42;
  Option opt = option_some(&value);
  cr_assert(option_is_some(&opt));
  cr_assert_eq(*(int*)opt.some, 42);
}

Test(option_some, empty_string) {
  char* empty = "";
  Option opt = option_some(empty);
  cr_assert(option_is_some(&opt));
  cr_assert_str_eq((char*)opt.some, "");
}

Test(option_none, basic) {
  Option opt = option_none();
  cr_assert(option_is_none(&opt));
  cr_assert_eq(opt.some, NULL);
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