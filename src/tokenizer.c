#include <stdlib.h>
#include <stdbool.h>

#include <btn/tokenizer.h>
#include <btn/vector.h>
#include <btn/cstr.h>

static inline
bool check_delims(char c, const char * delims)
{
    const char * delim = delims;
    while (*delim != '\0') {
        if (c == *delim)
            return true;
        ++delim;
    }
    return false;
}

typedef enum _tok_state
{
    TOK_FINDING,
    TOK_FOUND,
    TOK_DONE
} tok_state;
static
void internal_tokenizer(const char * str, size_t * size_p,
                        char *** elems_p, void * param)
{
    const char * delims = (const char *) param;
    vector(char *) v;
    vector_ctor(&v, sizeof(char *), NULL, NULL);

    size_t start = 0;
    size_t end = 0;
    tok_state state = TOK_FINDING;
    char c;
    char * token;
    while (state != TOK_DONE) {
        switch (state) {
        // Find beginning of new string
        case TOK_FINDING:
            c = str[start];
            if (c == '\0') {
                state = TOK_DONE;
            }
            else if (!check_delims(c, delims)) {
               state = TOK_FOUND;
               end = start;
            } else {
                ++start;
            }
            break;
        case TOK_FOUND:
            c = str[end + 1];
            if (c == '\0') {
                state = TOK_DONE;
                token = stridup(str, start, end);
                vector_push_back(&v, &token);
            } else if (check_delims(c, delims)) {
                state = TOK_FINDING;
                token = stridup(str, start, end);
                start = end + 2; // we know that +1 is a delim, start after
                vector_push_back(&v, &token);
            }
            ++end;
        case TOK_DONE:
            break;
        }
    }

    *elems_p = (char **) vector_to_array(&v);
    *size_p  = vector_size(&v);
    vector_dtor(&v);
}

void tokenizer_ctor(tokenizer * tok, const char * str, const char * delims)
{
    tokenizer_ctor_custom(tok, str, internal_tokenizer, (void *) delims);
}

void tokenizer_ctor_custom(tokenizer * tok, const char * str,
                           void (* func)(const char * str, size_t * size_p,
                                         char *** elems_p, void * param),
                           void * param)
{
    func(str, &tok->size, &tok->elems, param);
    tok->pos = 0;
}

void tokenizer_dtor(tokenizer * tok)
{
    for (size_t i = 0; i < tok->size; ++i) {
        free(tok->elems[i]);
    }
    free(tok->elems);
}

const char * tokenizer_next(tokenizer * tok)
{
    if (!tokenizer_has_tokens(tok)) {
        return NULL;
    }

    const char * out = tok->elems[tok->pos];
    ++tok->pos;
    return out;
}

char ** tokenizer_tokens_copy(const tokenizer * tok)
{
    char ** out = (char **) malloc(sizeof(char *) * tok->size);
    for (size_t i = 0; i < tok->size; ++i) {
        out[i] = strdup(tok->elems[i]);
    }
    return out;
}
