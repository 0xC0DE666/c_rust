#ifndef RUSTY_C_H
#define RUSTY_C_H

#include <stdbool.h>

#define ERR_CODE_GENERAL -1
#define ERR_MSG_BLANK "No error message provided."
#define ERR_MSG_NULL_POINTER(fn_name, var_name) "[ERROR] Null pointer detected in function '" #fn_name "'. Argument '" #var_name "' is null."

#define OK_CODE_GENERAL 0
#define OK_MSG_GENERAL "Operation succeeded."

typedef struct Error {
  int code;
  char* message;
} Error;

Error error_new(int code, char* message);
Error std_error_new();

typedef struct Result {
  void* ok;
  Error error;
} Result;

Result result_new_ok(void* value);
Result result_new_error(int code, char* message);
Result result_std_error();
bool result_is_ok(const Result* result);
bool result_is_error(const Result* result);

#endif
