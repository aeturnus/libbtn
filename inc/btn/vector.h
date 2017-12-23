#ifndef __BTN_VECTOR_H__
#define __BTN_VECTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * The vector struct provides a similar API to the C++ STL vector
 */

// So you can specify the type
#define vector(type) vector

typedef struct _vector
{
    // private
    uint8_t * data;
    size_t    e_size;       // how big each element is
    size_t    size;         // how many elements are in use
    size_t    cap;          // actual size of buffer in elements
    void (* ctor)(void *);  // element default constructor function
    void (* dtor)(void *);  // element destructor function
} vector;

/**
 * Construct the vector.
 * @param[in]   element_size    Size of elements to store
 * @param[in]   clean           Destructor function to use if not NULL
 */
void vector_ctor(vector * vec, size_t element_size,
                 void (* ctor)(void *), void (* dtor)(void *));

/**
 * @param[in]   initial_size    Initial size of the vector
 */
void vector_ctor_size(vector * vec, size_t element_size, size_t initial_size,
                      void (* ctor)(void *), void (* dtor)(void *));

/**
 * @param[in]   initial_cap     Initial capacity of the vector
 */
void vector_ctor_size_cap(vector * vec, size_t element_size,
                          size_t initial_size, size_t initial_cap,
                          void (* ctor)(void *), void (* dtor)(void *));

/**
 * Destructor
 */
void vector_dtor(vector * vec);

/**
 * @return The size of the vector
 */
static inline
size_t vector_size(const vector * vec) {return vec->size;}

/**
 * @return The capacity of the vector
 */
static inline
size_t vector_capacity(const vector * vec) {return vec->cap;}

/**
 * Resizes the vector to have a new size.
 */
bool vector_resize(vector * vec, size_t new_size);

/**
 * Set the size of the buffer to be at least the new capacity
 */
bool vector_reserve(vector * vec, size_t new_cap);

/**
 * Inserts an item at the end, resizing if needed
 * @param[in]   val     Item to push
 * @return true on success, false on failure
 */
bool vector_push_back(vector * vec, const void * val);

/**
 * Deletes the last item.
 * @return true on success, false on failure or no more elements
 */
bool vector_pop_back(vector * vec);

/**
 * Gets item at the front of the vector
 * @param[out]  val     Where to store the item retrieved
 * @return true on success, false on failure
 */
bool vector_front(vector * vec, void * val);

/**
 * Gets item at the end of the vector
 * @param[out]  val     Where to store the item retrieved
 * @return true on success, false on failure
 */
bool vector_back(vector * vec, void * val);

/**
 * Retrieves an element
 * @param[in]   idx     Index of element
 * @param[out]  val     Where to copy element to
 * @return true on success, false on failure
 */
bool vector_get(vector * vec, size_t idx, void * val);

/**
 * Pointer equivalent of vector_front
 * @return  Pointer to front element; NULL if size == 0
 */
void * vector_frontp(vector * vec);

/**
 * Pointer equivalent of vector_back
 * @return  Pointer to back element; NULL if size == 0
 */
void * vector_backp(vector * vec);

/**
 * Get a pointer to an element
 * @param[in]   idx     Index of element
 * @return true on success, false on failure
 */
void * vector_getp(vector * vec, size_t idx);

/**
 * Stores an element
 * @param[in]   idx     Index of element
 * @param[in]   val     Item to store
 * @return true on success, false on failure
 */
bool vector_put(vector * vec, size_t idx, const void * val);

/**
 * Provides raw access to underlying buffer
 * @return  Internal buffer
 */
static inline
void * vector_data(vector * vec) {return vec->data;}

/**
 * Inserts element at the index, shifting elements right
 * @param[in]   idx     Index to put the item
 * @param[in]   val     Item to put
 * @return true on success, false on failure
 */
bool vector_insert(vector * vec, size_t idx, const void * val);

/**
 * Deletes element at index, shifting elements left
 * @param[in]   idx     Index of item to delete
 * @return true on success, false on failure
 */
bool vector_erase(vector * vec, size_t idx);

#ifdef __cplusplus
}
#endif

#endif//__VECTOR_H__
