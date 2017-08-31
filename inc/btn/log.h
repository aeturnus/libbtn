#ifndef __BTN_LOG_H__
#define __BTN_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

// error printing
#define eprintf(...) fprintf(stderr,"ERROR>> "__VA_ARGS__)

// toggleable compile time debug printing
#ifdef DEBUG
#define dprintf(...) fprintf(stdout,"DEBUG>> "__VA_ARGS__)
#else
#define dprintf(...)
#endif

#ifdef __cplusplus
}
#endif

#endif//__BTN_LOG_H__
