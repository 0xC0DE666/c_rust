#ifndef C_ERRORS_H
#define C_ERRORS_H

#define ERR_MSG_NULL_POINTER(fn_name, var_name) "[ERROR] Null pointer detected in function '" #fn_name "'. Argument '" #var_name "' is null."

#define ERR_CODE_GENERAL -1
#define SUC_CODE_GENERAL 0

typedef struct Error {
  int code;
  char* message;
} Error;


Error* error_new(int code, char* message);
Error* std_error_new();
void error_free(Error** error);

typedef struct Result {
  void* ok;
  Error* error;
} Result;

Result result_ok(void* value);
Result result_error(int code, char* message);
Result result_std_error();

#endif
