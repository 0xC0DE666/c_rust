#ifndef RUSTY_C_H
#define RUSTY_C_H

#include <stdbool.h>

#define ERR_CODE_GENERAL -1
#define ERR_MSG_BLANK "No error message provided."
#define ERR_MSG_NULL_POINTER(fn_name, var_name) "[ERROR] Null pointer detected in function '" #fn_name "'. Argument '" #var_name "' is null."

#define OK_CODE_GENERAL 0
#define OK_MSG_GENERAL "Operation succeeded."

// ####################
// ERROR
// ####################
typedef struct Error {
  int code;
  char* message;
} Error;

extern const Error NO_ERROR;

Error error_new(int code, char* message);
Error std_error_new();

// ####################
// RESULT
// ####################
typedef struct Result {
  void* ok;
  Error error;
} Result;

Result result_ok(void* value);
Result result_error(int code, char* message);
Result result_std_error();
bool result_is_ok(const Result* result);
bool result_is_error(const Result* result);

// ####################
// OPTION
// ####################
typedef struct Option {
  void* some;
  void* none;
} Option;

Option option_some(void* value);
Option option_none();
bool option_is_some(const Option* option);
bool option_is_none(const Option* option);

#endif
