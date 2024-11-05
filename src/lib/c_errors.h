#include <stdbool.h>
#include <pthread.h>

#ifndef C_ERRORS_H
#define C_ERRORS_H

//####################
// COMMON
//####################
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

#define ERR_MALLOC_FAILED "malloc failed"

#define ERR_RWLOCK_INIT_FAILED "rwlock init failed"
#define ERR_RWLOCK_DESTROY_FAILED "rwlock destroy failed"
#define ERR_RDLOCK_FAILED "rdlock failed"
#define ERR_WRLOCK_FAILED "wrlock failed"
#define ERR_RWLOCK_UNLOCK_FAILED "rwlock unlock failed"

#endif
