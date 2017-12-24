#ifndef __BTN_BST_H__
#define __BTN_BST_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <btn/map.h>

#define bst(key_type, val_type) bst

typedef struct _bst_ops
{
    map_ops map;
} bst_ops;

struct _bst_node;
typedef struct _bst_node bst_node;

typedef struct _bst
{
    // private
    bst_node * root;

    size_t size;        // number of meaningful nodes

    size_t key_size;    // size of the key type
    size_t val_size;    // size of the value type
    void (* key_dtor)(void *);  // key destructor function
    void (* val_dtor)(void *);  // value destructor function

    int (* key_cmp)(const void * k1, const void * k2);  // key comparator

    // vtable
    bst_ops * ops;
} bst;

/**
 * Constructs a binary search tree
 * @param[in]   key_size    How large (bytes) is the key type (use sizeof())
 * @param[in]   val_size    How large (bytes) is the value type (use sizeof())
 * @param[in]   key_dtor    Destructor function for the key
 * @param[in]   val_dtor    Destructor function for the value
 * @param[in]   key_cmp     Comparator function for keys
 *                          Returns <0 if k1 < k2, 0 if k1 == k2, >0 if k1 > k2
 */
void bst_ctor(bst * tree,
              size_t key_size, size_t val_size,
              void (* key_dtor)(void *), void (* val_dtor)(void *),
              int (* key_cmp)(const void * k1, const void * k2));

void bst_dtor(bst * tree);

/**
 * Inserts a key-value pair into the tree
 * @param[in]   key     Pointer to key
 * @param[in]   val     Pointer to value
 * @return  True if successful
 */
bool bst_insert(bst * tree, const void * key, const void * val);

/**
 * Erases a key-value pair by the key
 * @param[in]   key     Pointer to key
 * @return  True if successful, false if key not found
 */
bool bst_erase(bst * tree, const void * key);

/**
 * Retrieves value for given key
 * @param[in]   key     Pointer to key
 * @param[in]   val     Pointer to write value to
 */
bool bst_find(bst * tree, const void * key, void * val);

/**
 * Retrieves value for given key
 * @param[in]   key     Pointer to key
 * @return  Pointer to the value
 */
void * bst_findp(bst * tree, const void * key);

static inline
size_t bst_size(bst * tree) { return tree->size; }

static inline
bool bst_empty(bst * tree) { return tree->size == 0; }

// red-black node stuff below
// users should not touch these: these are here for testing purposes

typedef enum _rb_color
{
    RB_BLK,
    RB_RED,
} rb_color;

/**
 * @return  Node's color
 */
rb_color bst_node_color(bst * tree, bst_node * node);

/**
 * @return  left or right children or parent
 */
bst_node * bst_node_l(bst * tree, bst_node * node);
bst_node * bst_node_r(bst * tree, bst_node * node);
bst_node * bst_node_p(bst * tree, bst_node * node);

/**
 * Reads the key/value into key or val
 */
void bst_node_get_key(bst * tree, bst_node * node, void * key);
void bst_node_get_val(bst * tree, bst_node * node, void * val);

/**
 * @return  Pointers to the key/value
 */
void * bst_node_getp_key(bst * tree, bst_node * node);
void * bst_node_getp_val(bst * tree, bst_node * node);

// Write access functions
bst_node * bst_node_new(bst * tree, const void * key, const void * val);
void bst_node_delete(bst * tree, bst_node * node);
void bst_node_set_key(bst * tree, bst_node * node, const void * key);
void bst_node_set_val(bst * tree, bst_node * node, const void * val);

#ifdef __cplusplus
}
#endif

#endif//__BTN_BST_H__
