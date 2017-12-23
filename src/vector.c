#include <stdlib.h>
#include <string.h>

#include <btn/vector.h>

#define MIN_CAP 4

// Get the pointer to the element
static inline
void * element(vector * vec, int idx)
{
    void * out = vec->data + idx * vec->e_size;
    return out;
}

void vector_ctor_size_cap(vector * vec, size_t element_size,
                          size_t initial_size, size_t initial_cap,
                          void (* ctor)(void *), void (* dtor)(void *))
{
    vec->e_size = element_size;
    vec->size   = initial_size;
    if (initial_cap < MIN_CAP)
        initial_cap = MIN_CAP;
    if (initial_cap < initial_size)
        initial_cap = initial_size;
    vec->cap    = initial_cap;
    vec->ctor   = ctor;
    vec->dtor   = dtor;
    vec->data   = (uint8_t *) malloc(vec->e_size * vec->cap);
    if (vec->ctor != NULL) {
        for (size_t i = 0; i < initial_size; ++i) {
            vec->ctor(element(vec, i));
        }
    }
}


void vector_ctor_size(vector * vec, size_t element_size, size_t initial_size,
                      void (* ctor)(void *), void (* dtor)(void *))
{
    vector_ctor_size_cap(vec, element_size, initial_size,
                         initial_size * 2, ctor, dtor);
}

void vector_ctor(vector * vec, size_t element_size,
                 void (* ctor)(void *), void (* dtor)(void *))
{
    vector_ctor_size_cap(vec, element_size, 0, 0, ctor, dtor);
}

void vector_dtor(vector * vec)
{
    if (vec->dtor != NULL) {
        for (int i = vec->size - 1; i >= 0; --i) {
            vec->dtor(element(vec, i));
        }
    }
    free(vec->data);
    vec->data = NULL;
}

static inline
size_t grow(size_t cap)
{
    return cap * 3 / 2;
}

bool vector_resize(vector * vec, size_t new_size)
{
    size_t old_size = vec->size;
    if (new_size < old_size) {
        // size it down: destruct elements
        if (vec->dtor != NULL) {
            for (size_t i = new_size; i < old_size; ++i) {
                vec->dtor(element(vec, i));
            }
        }
    } else if (new_size > old_size) {
        // if no more capacity, reserve more space
        if (new_size > vec->cap) {
            if (!vector_reserve(vec, grow(vec->cap)))
                return false;
        }
        // size up: construct elements
        if (vec->ctor != NULL) {
            for (size_t i = old_size; i < new_size; ++i) {
                vec->ctor(element(vec, i));
            }
        }
    }
    vec->size = new_size;
    return true;
}

bool vector_reserve(vector * vec, size_t new_cap)
{
    if (new_cap <= vec->cap)
        return true;

    uint8_t * new_data = (uint8_t *) realloc(vec->data, vec->e_size * new_cap);
    if (new_data == NULL)
        return false;
    vec->data = new_data;
    vec->cap = new_cap;

    return true;
}

bool vector_push_back(vector * vec, const void * val)
{
    // sizing
    size_t new_size = vec->size + 1;
    if (new_size > vec->cap) {
        if (!vector_reserve(vec, grow(vec->cap)))
            return false;
    }

    // copy over val
    memcpy(element(vec, new_size - 1), val, vec->e_size);
    vec->size = new_size;

    return true;
}

bool vector_pop_back(vector * vec)
{
    if (vec->size == 0)
        return false;

    if (vec->dtor) {
        vec->dtor(element(vec, vec->size - 1));
    }
    vec->size -= 1;

    return true;
}

bool vector_front(vector * vec, void * val)
{
    void * src = vector_frontp(vec);
    if (src == NULL)
        return false;

    memcpy(val, src, vec->e_size);

    return true;
}

bool vector_back(vector * vec, void * val)
{
    void * src = vector_backp(vec);
    if (src == NULL)
        return false;

    memcpy(val, src, vec->e_size);

    return true;
}

bool vector_get(vector * vec, size_t idx, void * val)
{
    void * src = vector_getp(vec, idx);
    if (src == NULL)
        return false;

    memcpy(val, src, vec->e_size);

    return true;
}

bool vector_put(vector * vec, size_t idx, const void * val)
{
    void * dst = vector_getp(vec, idx);
    if (dst == NULL)
        return false;
    memcpy(dst, val, vec->e_size);

    return true;
}

void * vector_frontp(vector * vec)
{
    if (vec->size == 0)
        return NULL;
    return element(vec, 0);
}

void * vector_backp(vector * vec)
{
    if (vec->size == 0)
        return NULL;
    return element(vec, vec->size - 1);
}

void * vector_getp(vector * vec, size_t idx)
{
    if (vec->size == 0 || idx >= vec->size)
        return NULL;

    return element(vec, idx);
}

bool vector_insert(vector * vec, size_t idx, const void * val)
{
    if (vec->size == 0 || idx >= vec->size)
        return false;

    // sizing
    size_t new_size = vec->size + 1;
    if (new_size > vec->cap) {
        if (!vector_reserve(vec, grow(vec->cap)))
            return false;
    }

    // shift over
    for (size_t i = new_size - 1; i > idx; --i) {
        memcpy(element(vec, i), element(vec, i - 1), vec->e_size);
    }
    memcpy(element(vec, idx), val, vec->e_size);
    vec->size = new_size;

    return true;
}

bool vector_erase(vector * vec, size_t idx)
{
    if (vec->size == 0 || idx >= vec->size)
        return false;

    if (vec->dtor != NULL) {
        vec->dtor(element(vec, idx));
    }

    // shift over
    size_t new_size = vec->size - 1;
    for (size_t i = idx; i < new_size - 1; ++i) {
        memcpy(element(vec, i), element(vec, i+1), vec->e_size);
    }

    return true;
}
