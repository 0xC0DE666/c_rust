#ifndef RUSTY_C_H
#define RUSTY_C_H

#include <stdbool.h>

#define ERR_CODE_GENERAL -1
#define ERR_MSG_BLANK "No error message provided."
#define ERR_MSG_NULL_POINTER(fn_name, var_name) "[ERROR] Null pointer detected in function '" #fn_name "'. Argument '" #var_name "' is null."

#define OK_CODE_GENERAL 0
#define OK_MSG_GENERAL "Operation succeeded."

// ####################
// RESULT
// ####################
typedef struct {} Ok;
static const Ok OK;

typedef struct Error {
  const int code;
  const char* message;
} Error;

static const Error NO_ERROR;

Error error_new(const int code, const char* message);
Error std_error_new();

typedef struct Result {
  void* ok;
  const Error error;
} Result;

Result result_ok(void* value);
Result result_unit();
Result result_error(const int code, const char* message);
Result result_std_error();
bool result_is_ok(const Result* result);
bool result_is_error(const Result* result);

// ####################
// OPTION
// ####################

typedef struct {} None;

static const None NONE;

typedef struct Option {
  void* some;
  const None* none;
} Option;

Option option_some(void* value);
Option option_none();
bool option_is_some(const Option* option);
bool option_is_none(const Option* option);

#endif
