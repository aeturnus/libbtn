#ifndef __BTN_ITERATOR_H__
#define __BTN_ITERATOR_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This provides for the iterator interface
 */

/*
 * Iterators implement this interface by a field called "ops"
 * Iterator structs must not have any other interfaces
 */
typedef struct _iterator_ops
{
    /**
     * next() and prev() will increment/decrement the iterator by n
     */
    void (* next)(void * it, int n);
    void (* prev)(void * it, int n);

    /**
     * Will insert val at the iterator's position
     * @param[in]   val     The value to insert
     */
    bool (* insert)(void * it, const void * val);

    /**
     * Removes the element the iterator is sitting on
     */
    bool (* remove)(void * it);

    /**
     * read will write the element to the destination
     * @param[out]  dst     Where the element will be written to
     * @return  True if success (e.g. not out of bounds)
     */
    bool (* read)(void * it, void * dst);

    /**
     * ptr will return a pointer to the data the iterator is on
     */
    void * (* ptr)(void * it);

    /**
     * Return if the iterator is sitting at the beginning or end
     * If an element is at the end, reading or getting the ptr will fail
     */
    bool (* is_begin)(void * it);
    bool (* is_end)(void * it);
} iterator_ops;

/*
 * Iterable data structures implement this interface
 */
typedef struct _iterable_ops
{
    /**
     * Constructs an iterator sitting at the beginning
     * @param[in]   ds      The datastructure
     * @param[out]  it      The iterator to construct
     */
    void (* begin)(void * ds, void * it);

    /**
     * Constructs an iterator sitting at the end
     * @param[in]   ds      The datastructure
     * @param[out]  it      The iterator to construct
     */
    void (* end)(void * ds, void * it);
} iterable_ops;

// Iterator methods: "it" is a pointer
#define it_next(_it, _n) (_it)->ops->iterator.next(_it, _n)
#define it_prev(_it, _n) (_it)->ops->iterator.prev(_it, _n)
#define it_insert(_it, _val) (_it)->ops->iterator.insert(_it, _val)
#define it_remove(_it) (_it)->ops->iterator.remove(_it)
#define it_read(_it, _dst) (_it)->ops->iterator.read(_it, _dst)
#define it_ptr(_it) (_it)->ops->iterator.ptr(_it)
#define it_is_begin(_it) (_it)->ops->iterator.is_begin(_it)
#define it_is_end(_it) (_it)->ops->iterator.is_end(_it)

// Iterable methods: "ds" and "it" are pointers
#define it_begin(_ds, _it) (_ds)->ops->iterable.begin(_ds, _it)
#define it_end(_ds, _it) (_ds)->ops->iterable.end(_ds, _it)

#ifdef __cplusplus
}
#endif

#endif//__BTN_ITERATOR_H__
