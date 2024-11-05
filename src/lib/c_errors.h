#include <errno.h>

#ifndef C_ERRORS_H
#define C_ERRORS_H

typedef struct Error {
  int code;
  char* message;
} Error;


Error* new_error(int code, char* message);
void free_error(Error** error);

typedef struct Result {
  void* ok;
  Error* error;
} Result;

Result result_ok(void* value);
Result result_error(int code, char* message);

#endif
