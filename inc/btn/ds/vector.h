#ifndef __BTN_VECTOR_H__
#define __BTN_VECTOR_H__

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define vector(type) vector

struct vector_str;
typedef struct vector_str vector;

typedef struct vector_str
{
    // private members: do not write to
    char * _buffer;
    size_t _element_size;
    size_t _length;
    size_t _capacity;
    void (* _clean)(void *);
} vector;


/**
 * Basic constructor. Initial capacity is implementation defined
 * @param   element_size    Size of the elements to store
 * @param   clean           Function that will clean up the element. NULL will
 *                          result in a NOP
 */
void vector_ctor( vector * thiz, size_t element_size, void (* clean)(void *) );

/**
 * Constructor with initial capacity
 * @param   element_size        Size of the elements to store
 * @param   initial_capacity    Initial capacity of which the vector will
 *                              contain at least
 */
void vector_ctor_cap( vector * thiz, size_t element_size, size_t initial_capacity, void (* clean)(void *) );

/**
 * Destructor
 */
void vector_dtor( vector * thiz );

/*
 * Dynamic memory versions
 */
vector * vector_new( size_t element_size, void (* clean)(void *) );
vector * vector_new_cap( size_t element_size, size_t initial_capacity, void (* clean)(void *) );
void vector_delete( vector * thiz );

/**
 * Returns the length
 */
static inline size_t vector_length( vector * thiz ) { return thiz->_length; }
static inline size_t vector_capacity( vector * thiz ) { return thiz->_capacity; }

/**
 * Reallocates so the capacity is at least the provided capacity
 * @param   capacity    The requested capacity
 * @return  1 if success
 */
int vector_realloc( vector * thiz, size_t capacity );

/**
 * Pushes element to the end of the vector
 * @src Element to copy to the end of the vector
 * @return  1 if success
 */
int vector_push( vector * thiz, const void * src );

/**
 * Removes element
 * @dst Storage spot that the last element will be copied to. Removes last
 *      element from the vector
 * @return  1 if success, 0 if no more elements
 */
int vector_pop( vector * thiz, void * dst );

/**
 * Gets an item from the vector, writing it to dst
 * @index   Index of element
 * @dst     Area to write the element to
 * @return  1 if success, 0 if out of bounds
 */
int vector_get( vector * thiz, int index, void * dst );

/**
 * Puts an item into the vector at a given index, shifting the other elements
 * to the end.
 * If the index is greater than length, behavior is undefined.
 * @index   Index where the element should be
 * @src     The element itself
 * @return  1 if success, 0 if out of bounds
 */
int vector_insert( vector * thiz, int index, const void * src );

/**
 * Removes the element at a given index, shifting elements after it left
 * @index   Index of element to remove
 * @dst     If not NULL, will copy the element to this
 * @return  1 if success, 0 if out of bounds
 */
int vector_remove( vector * thiz, int index, void * dst );

#ifdef __cplusplus
}
#endif

#endif//__BTN_VECTOR_H__
