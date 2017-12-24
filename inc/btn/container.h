#ifndef __BTN_CONTAINER_H__
#define __BTN_CONTAINER_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This file contains helpful functions and macros for container types
 */

/**
 * A shim function that allows you to free malloc'ed pointers in a libbtn
 * container
 * E.g. you put malloc'ed strings into a vector and want the vector
 * to automatically free them. The dtor function you pass on constructing the
 * vector takes the address of the element in the vector: an address to the
 * internal data buffer used by the vector. The actual string pointer is the
 * element itself. This function will dereference that element pointer and
 * pass that dereferenced data pointer to free.
 */
void btn_free_shim(void * arg);

/**
 * This macros are for casting constructors and destructors to
 * appease the C++ compilers
 */
#define ctor_cast(func) ((void (*)(void *)) func)
#define dtor_cast(func) ((void (*)(void *)) func)

#ifdef __cplusplus
}
#endif

#endif//__CONTAINER_H__
