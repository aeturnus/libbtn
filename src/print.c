#if defined PLATFORM_WINDOWS
#define DEF_STRIP true
#elif defined PLATFORM_UNIX
#define _POSIX_C_SOURCE 1
#include <unistd.h>
#define DEF_STRIP (!isatty(fileno(stream)))
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

static
void strip_ansi(char * buf, size_t len)
{
    // read and write heads
    size_t r = 0, w = 0;
    while (r <= len) {
        char c = buf[r];
        if (c == '\x1B') {
            // advance the read head until past the m
            while (c != 'm') {
                c = buf[++r];
            }
            ++r;
            continue;   // continue: may have multiple codes adjacent
                        // so need to start another check
        }
        buf[w++] = buf[r++];
    }
}

int afprintf(FILE * stream, const char * format, ...)
{
    bool strip = DEF_STRIP;
    bool contains_ansi = false;
    size_t len = strlen(format);
    int ret = -1;

    // memory allocation only if needed
    for (size_t i = 0; i < len; ++i) {
        if (format[i] == '\x1B') {
            contains_ansi = true;
            break;
        }
    }

    va_list ap;
    va_start(ap, format);
    if (strip && contains_ansi) {
        // get our own working buffer to strip ANSI codes
        char * buf = (char *) malloc(sizeof(char *) * (len + 1));
        strcpy(buf, format);

        // strip the ansi codes
        strip_ansi(buf, len);

        // pass it onto vaprintf
        ret = vfprintf(stream, buf, ap);
    } else {
        // just pass it off without changes
        ret = vfprintf(stream, format, ap);
    }
    va_end(ap);
    return ret;
}
