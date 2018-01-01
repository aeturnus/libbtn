#ifndef __BTN_CSTRING_H__
#define __BTN_CSTRING_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <string.h>
#include <stdint.h>

/**
 * Helpful functions involving C strings
 */

/**
 * Creates a string given the position of the first and last character
 * in a source string
 * @param[in]   str     The source string
 * @param[in]   start   The index of the starting character
 * @param[in]   end     The index of the ending character
 * @return      The requested string. Must be free()'ed later
 */
char * stridup(const char * str, size_t start, size_t end);

#ifndef __cplusplus
char * strdup(const char * str);
#endif

/**
 * Creates a string given a single character
 * @param[in]   c   The character
 * @return      The requested string. Must be free()'ed later
 */
char * char_to_str(char c);

/**
 * Performs a caseless strcmp
 * @return <0 if s1 < s2, 0 if s1 == s2, >0 if s1 > s2 in lexographic ordering
 */
int strcmp_caseless(const char * s1, const char * s2);

/**
 * Finds first occurance of substring
 * @param   str     The string to search in
 * @param   substr  The string to look for
 * @param   off     Offset to start from
 * @return  Index of pattern; SIZE_MAX if not found
 */
size_t strfind(const char * str, const char * substr, size_t off);

/**
 * Finds first occurance of character
 * @param   str     The string to search in
 * @param   c       The char to look for
 * @param   off     Offset to start from
 * @return  Index of pattern; SIZE_MAX if not found
 */
size_t strcfind(const char * str, char c, size_t off);

/**
 * Turns uppercase character into lowercase
 */
static inline
char to_lower(char c)
{
    if ('A' <= c && c <= 'Z')
        c += ('a' - 'A');
    return c;
}

/**
 * Turns lowercase character into uppercase
 */
static inline
char to_upper(char c)
{
    if ('a' <= c && c <= 'z')
        c -= ('a' - 'A');
    return c;
}

#ifdef __cplusplus
}
#endif

#endif//__CSTRING_H__
