#ifndef C_LOGGER_H
#define C_LOGGER_H

// Define ANSI color codes
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

typedef enum LogLevel {
  ERROR = 0, WARN = 1, INFO = 2, DEBUG = 3, VERBOSE = 4
} LogLevel;

void write_log(LogLevel, const char*);

#endif
