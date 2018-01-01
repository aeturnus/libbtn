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
int atfprintf(FILE * stream, const char * format, ...);

/**
 * Prints to a file, stripping ANSI escape colors if it is not a TTY and
 * enable is false
 */
int aetfprintf(bool enable, FILE * stream, const char * format, ...);

/**
 * Prints to a file, stripping ANSI escape colors if it is not a TTY and
 * enable is false. va_list version
 */
int aetvfprintf(bool enable, FILE * stream, const char * format, va_list ap);

/**
 * Prints to a file, stripping ANSI escape colors if not enabled
 */
int aefprintf(bool enable, FILE * stream, const char * format, ...);

/**
 * va_list version
 */
int aevfprintf(bool enable, FILE * stream, const char * format, va_list ap);

#ifdef __cplusplus
}
#endif

#endif//__BTN_PRINT_H__
