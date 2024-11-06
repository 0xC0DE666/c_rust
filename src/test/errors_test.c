#include <errno.h>
#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_errors.h"
#include "./utils.h"

// ####################
// error_new
// ####################
Test(error_new, _1) {
  Error* actual = error_new(1, "error");
  Error expected = {1, "error"};

  cr_assert_eq(actual != NULL, true);
  cr_assert_eq(actual->code, expected.code);
  cr_assert_eq(strcmp(actual->message, expected.message), 0);

  error_free(&actual);
}

// ####################
// std_error_new
// ####################
Test(std_error_new, _1) {
  Error* actual = file_404();
  Error expected = {ENOENT, strerror(errno)};

  cr_assert_eq(actual != NULL, true);
  cr_assert_eq(actual->code, expected.code);
  cr_assert_eq(strcmp(actual->message, expected.message), 0);

  error_free(&actual);
}

// ####################
// error_free
// ####################
Test(error_free, _1) {
  Error* actual = error_new(1, "error");

  error_free(&actual);

  cr_assert_eq(actual, NULL);
}

// ####################
// result_ok
// ####################
Test(result_ok, _1) {
  char* msg = "ok";
  Result actual = result_ok(&msg);
  Result expected = {&msg, NULL};

  cr_assert_eq(actual.error, NULL);
  cr_assert_eq(actual.ok != NULL, true);
  cr_assert_eq(strcmp(actual.ok, expected.ok), 0);
  cr_assert_eq(actual.error, NULL);
}

// ####################
// result_error
// ####################
Test(result_error, _1) {
  Error* err = error_new(1, "error");
  Result actual = result_error(1, "error");
  Result expected = {NULL, err};

  cr_assert_eq(actual.ok, NULL);
  cr_assert_eq(actual.error != NULL, true);
  cr_assert_eq(actual.error->code, expected.error->code);
  cr_assert_eq(strcmp(actual.error->message, expected.error->message), 0);

  error_free(&err);
}
