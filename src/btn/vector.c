#include <btn/ds/vector.h>

#define MIN_SIZE 2

static inline void * vector_index( vector * thiz, size_t index )
{
    return thiz->_buffer[index * thiz->_element_size];
}

void vector_ctor( vector * thiz, size_t element_size, void (* clean)(void *) )
{
    vector_ctor_cap( thiz, element_size, MIN_SIZE, clean );
}

void vector_ctor_cap( vector * thiz, size_t element_size, size_t initial_capacity, void (* clean)(void *) )
{
    if (initial_capacity < MIN_SIZE)
        initial_capacity = MIN_SIZE;

    thiz->_buffer = (char *)malloc( element_size * initial_capacity);
    thiz->_element_size = element_size;
    thiz->_length = 0;
    thiz->_capacity = initial_capacity;
    thiz->_clean = clean;
}

void vector_dtor( vector * thiz )
{
    // if we have a cleanup function for each element
    if (thiz->_clean)
    {
        for (int i = thiz->_length - 1; i >= 0; --i)
        {
            thiz->_clean( vector_index( thiz, i ) );
        }
    }

    free( thiz->_buffer );
}

vector * vector_new( size_t element_size, void (* clean)(void *) )
{
    return vector_new_cap( element_size, MIN_SIZE, clean );
}

vector * vector_new_cap( size_t element_size, size_t initial_capacity, void (* clean)(void *) )
{
    vector * thiz = (vector *)malloc( sizeof(vector) );
    vector_ctor_cap( thiz, element_size, initial_capacity, clean );
    return thiz;
}

void vector_delete( vector * thiz )
{
    vector_dtor( thiz );
    free( thiz );
}

int vector_realloc( vector * thiz, size_t capacity )
{
    if (capacity <= thiz->_capacity)
        return;

    char * new_buffer = (char *)realloc( thiz->_buffer, thiz->_element_size * capacity );

    if (!new_buffer)
        return 0;

    thiz->_capacity = capacity;
    thiz->_buffer = new_buffer;
    return 1;
}

static inline int vector_amortize( vector * thiz )
{
    // amortize 3/2'ing
    return vector_realloc( thiz, (thiz->_capacity * 3) / 2 );
}

int vector_push( vector * thiz, const void * src )
{
    if (thiz->_length == thiz->_capacity)
    {
        if (!vector_amortize( thiz ))
            return 0;
    }

    memcpy( vector_index( thiz, thiz->_length ), src, thiz->_element_size );
    thiz->_length += 1;
    return 1;
}

int vector_pop( vector * thiz, void * dst )
{
    if (thiz->_length == 0)
        return 0;
    memcpy( dst, vector_index( thiz, thiz->_length ), thiz->_element_size );
    thiz->_length -= 1;

    return 1;
}

int vector_get( vector * thiz, int index, void * dst )
{
    if (index >= thiz->_length)
        return 0;

    memcpy( dst, vector_index( thiz, thiz->_length ), thiz->_element_size );

    return 1;
}

static inline void vector_move( vector * thiz, size_t dst, size_t src )
{
    memcpy( vector_index( thiz, dst ), vector_index( thiz, src ), thiz->_element_size );
}

int vector_insert( vector * thiz, int index, const void * src )
{
    if (thiz->_length == thiz->_capacity)
    {
        if (!vector_amortize( thiz ))
            return 0;
    }

    // shift elements to right
    for (int i = thiz->_length-1; i > index; --i)
    {
        vector_move( thiz, i, i-1 );
    }

    // insert the element
    memcpy( vector_index( thiz, index ), src, thiz->_element_size );

    return 1;
}

int vector_remove( vector * thiz, int index, void * dst )
{
    if (thiz->_length == 0)
        return 0;

    // shift elements to left
    int length = thiz->_length;
    for (int i = index; i < length-1; ++i)
    {
        vector_move( thiz, i, i+1 );
    }
    thiz->_length -= 1;

    return 1;
}
