#ifndef __BTN_PRINT_H__
#define __BTN_PRINT_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdbool.h>

/**
 * Prints to a file, stripping ANSI escape colors if it is not a TTY
 */
int afprintf(FILE * stream, const char * format, ...);

/**
 * Prints to a file, stripping ANSI escape colors if enabled
 */
int eafprintf(bool enable, FILE * stream, const char * format, ...);

/**
 * va_list version
 */
int eavfprintf(bool enable, FILE * stream, const char * format, va_list ap);

#ifdef __cplusplus
}
#endif

#endif//__BTN_PRINT_H__
