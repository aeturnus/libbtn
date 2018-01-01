#ifndef __BTN_PRINT_H__
#define __BTN_PRINT_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>

/**
 * Prints to a file, stripping ANSI escape colors if it is not a TTY
 */
int afprintf(FILE * stream, const char * format, ...);

#ifdef __cplusplus
}
#endif

#endif//__BTN_PRINT_H__
