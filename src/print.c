#if defined PLATFORM_WINDOWS
#define DEF_ENABLE false // strip by default: command prompt can't handle colors
#elif defined PLATFORM_UNIX
#define _POSIX_C_SOURCE 1
#include <unistd.h>
#define DEF_ENABLE (isatty(fileno(stream)))
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

int aevfprintf(bool enable, FILE * stream, const char * format, va_list ap)
{
    int ret = -1;
    size_t len = strlen(format);
    // memory allocation only if needed
    bool contains_ansi = false;
    for (size_t i = 0; i < len; ++i) {
        if (format[i] == '\x1B') {
            contains_ansi = true;
            break;
        }
    }

    if (!enable && contains_ansi) {
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

    return ret;
}

int atfprintf(FILE * stream, const char * format, ...)
{
    bool enable = DEF_ENABLE;

    va_list ap;
    va_start(ap, format);
    int ret = aevfprintf(enable, stream, format, ap);
    va_end(ap);
    return ret;
}

int aetfprintf(bool in_enable, FILE * stream, const char * format, ...)
{
    bool enable = DEF_ENABLE && in_enable;

    va_list ap;
    va_start(ap, format);
    int ret = aevfprintf(enable, stream, format, ap);
    va_end(ap);
    return ret;
}

int aefprintf(bool enable, FILE * stream, const char * format, ...)
{
    va_list ap;
    va_start(ap, format);
    int ret = aevfprintf(enable, stream, format, ap);
    va_end(ap);
    return ret;
}
