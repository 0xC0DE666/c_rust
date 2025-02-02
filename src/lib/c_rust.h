#ifndef RUSTY_C_H
#define RUSTY_C_H

#include <stdbool.h>

// TODO: Maybe rename NONE to VOID/NULL?
#define ERR_CODE_GENERAL -1
#define ERR_CODE_NONE 0

#define ERR_MSG_GENERAL "An unexpected error occurd."
#define ERR_MSG_BLANK "No error message provided."
#define ERR_MSG_NONE ""
#define ERR_MSG_NULL_POINTER(fn_name, var_name) "[ERROR] Null pointer detected in function '" #fn_name "'. Argument '" #var_name "' is null."

#define OK_CODE_GENERAL 0
#define OK_CODE_VOID 0
#define OK_CODE_NONE 0

#define OK_MSG_GENERAL "Operation succeeded."
#define OK_MSG_VOID ""
#define OK_MSG_NONE ""

// ####################
// RESULT
// ####################
typedef struct Ok {
  size_t size;
  void* value;
} Ok;
extern const Ok OK_VOID;
extern const Ok OK_NONE;

Ok ok_new(const size_t size, void* value);

typedef struct Error {
  const int code;
  const char* message;
} Error;

extern const Error ERROR_NONE;

Error error_new(const int code, const char* message);
Error std_error_new();

typedef struct Result {
  const Ok ok;
  const Error error;
} Result;

Result result_ok(size_t size, void* value);
Result result_void();
Result result_error(const int code, const char* message);
Result result_std_error();
bool result_is_ok(const Result* result);
bool result_is_void(const Result* result);
bool result_is_error(const Result* result);

// ####################
// OPTION
// ####################

typedef struct {} None;

extern const None NONE;

typedef struct Option {
  void* some;
  const None* none;
} Option;

Option option_some(void* value);
Option option_none();
bool option_is_some(const Option* option);
bool option_is_none(const Option* option);

#endif
