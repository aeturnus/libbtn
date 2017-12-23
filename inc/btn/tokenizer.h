#ifndef __BTN_TOKENIZER_H__
#define __BTN_TOKENIZER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include <btn/vector.h>

/**
 * The tokenizer is similar to Java's StringTokenizer
 */

typedef struct _tokenizer
{
    // private
    size_t pos;     // current token
    size_t size;    // number of elements
    char ** elems;  // the elements themselves
} tokenizer;

/**
 * Normal tokenizer constructor
 * @param[in]   str     The string to tokenize
 * @param[in]   delims  String containing delimiters
 */
void tokenizer_ctor(tokenizer * tok, const char * str, const char * delims);

/**
 * Custom tokenizer constructor, allowing caller to provide their own
 * tokenizing function
 * @param[in]   str     The string to tokenize
 * @param[in]   func    Tokenizing function
 *                      str    : string to pass in
 *                      size_p : report back the number of elements
 *                      elems_p: report back an array of strings
 *                      param  : some parameter (e.g. delimiters)
 * @param[in]   param   Parameter to pass to the custom tokenizer
 */
void tokenizer_ctor_custom(tokenizer * tok, const char * str,
                           void (* func)(const char * str, size_t * size_p,
                                         char *** elems_p, void * param),
                           void * param);
void tokenizer_dtor(tokenizer * tok);

/**
 * Returns next token and advances the tokenizer to the next element
 * Token memory is managed by the tokenizer
 * @return  The token: owned by the tokenizer
 */
const char * tokenizer_next(tokenizer * tok);

/**
 * @return  The remaining tokens in the tokenizer
 */
static inline
size_t tokenizer_count_tokens(const tokenizer * tok)
{
    return tok->size - tok->pos;
}

/**
 * @return  The total number of tokens
 */
static inline
size_t tokenizer_num_tokens(const tokenizer * tok) {return tok->size;}

/**
 * @return  True if there are remaining tokens
 */
static inline
bool tokenizer_has_tokens(const tokenizer * tok) {return tok->pos < tok->size;}

/**
 * Resets the tokenizer so you can go through the tokens again
 */
static inline
void tokenizer_reset(tokenizer * tok) {tok->pos = 0;}

/**
 * @return  The array of tokens managed by the tokenizer
 */
static inline
const char * const * tokenizer_tokens(const tokenizer * tok)
{
    return (const char * const *) tok->elems;
}

/**
 * @return  The an array of tokens: caller is responsible for the array and
 *          the strings
 */
char ** tokenizer_tokens_copy(const tokenizer * tok);

#ifdef __cplusplus
}
#endif

#endif//__BTN_TOKENIZER_H__
