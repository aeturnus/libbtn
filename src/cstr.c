#include <string.h>
#include <stdlib.h>

#include <btn/cstr.h>

char * strdup(const char * str)
{
    char * out = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    if (out == NULL)
        return NULL;
    strcpy(out, str);
    return out;
}

char * stridup(const char * str, size_t start, size_t end)
{
    if (end < start)
        return NULL;

    size_t len = end - start + 1;
    char * out = (char *) malloc(sizeof(char) * (len + 1));
    strncpy(out, str + start, len);
    out[len] = '\0';
    return out;
}


/**
 * Creates a string given a single character
 * @param[in]   c   The character
 * @return      The requested string. Must be free()'ed later
 */
char * char_to_str(char c)
{
    char * out = (char *) malloc(sizeof(char) * 2);
    out[0] = c;
    out[1] = '\0';
    return out;
}

/**
 * Performs a caseless strcmp
 * @return <0 if s1 < s2, 0 if s1 == s2, >0 if s1 > s2 in lexographic ordering
 */
int strcmp_caseless(const char * s1, const char * s2)
{
    char c1 = *s1; char c2 = *s2;
    while (c1)
    {
        // case matching
        c1 = to_lower(c1);
        c2 = to_lower(c2);

        int comp = (c1-c2);
        if (comp)
            return comp;

        ++s1; ++s2;
        c1 = *s1; c2 = *s2;
    }
    return c1-c2;
}

size_t strfind(const char * str, const char * substr, size_t off)
{
    int idx = off;
    size_t str_len = strlen(str);
    size_t substr_len = strlen(substr);
    while (idx + substr_len <= str_len) {
        if (strncmp(&str[idx], substr, substr_len) == 0)
            return idx;
        ++idx;
    }
    return SIZE_MAX;
}

size_t strcfind(const char * str, char c, size_t off)
{
    size_t idx = off;
    while (str[idx] != '\0') {
        if (c == str[idx])
            return idx;
        ++idx;
    }
    return SIZE_MAX;
}
