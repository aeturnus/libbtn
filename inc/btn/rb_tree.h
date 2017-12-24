#ifndef __BTN_RB_TREE_H__
#define __BTN_RB_TREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define rb_tree(key_type, val_type) rb_tree

struct _rb_node;
typedef struct _rb_node rb_node;

typedef struct _rb_tree
{
    // private
    rb_node * root;

    size_t size;        // number of meaningful nodes

    size_t key_size;    // size of the key type
    size_t val_size;    // size of the value type
    void (* key_dtor)(void *);  // key destructor function
    void (* val_dtor)(void *);  // value destructor function

    int (* key_cmp)(void * k1, void * k2);  // key comparator
} rb_tree;

/**
 * Constructs a red-black tree
 * @param[in]   key_size    How large (bytes) is the key type (use sizeof())
 * @param[in]   val_size    How large (bytes) is the value type (use sizeof())
 * @param[in]   key_dtor    Destructor function for the key
 * @param[in]   val_dtor    Destructor function for the value
 * @param[in]   key_cmp     Comparator function for keys
 *                          Returns <0 if k1 < k2, 0 if k1 == k2, >0 if k1 > k2
 */
void rb_tree_ctor(rb_tree * tree,
                  size_t key_size, size_t val_size,
                  void (* key_dtor)(void *), void (* val_dtor)(void *),
                  int (* key_cmp)(void * k1, void * k2));

// red-black node stuff below

typedef enum _rb_color
{
    RB_BLK,
    RB_RED,
} rb_color;

/**
 * @return  Node's color
 */
rb_color rb_node_color(rb_tree * tree, rb_node * node);

/**
 * @return  left or right children or parent
 */
rb_node * rb_node_l(rb_tree * tree, rb_node * node);
rb_node * rb_node_r(rb_tree * tree, rb_node * node);
rb_node * rb_node_p(rb_tree * tree, rb_node * node);

/**
 * Reads the key/value into key or val
 */
void rb_node_get_key(rb_tree * tree, rb_node * node, void * key);
void rb_node_get_val(rb_tree * tree, rb_node * node, void * val);

/**
 * @return  Pointers to the key/value
 */
void * rb_node_getp_key(rb_tree * tree, rb_node * node);
void * rb_node_getp_val(rb_tree * tree, rb_node * node);

// Write access functions
rb_node * rb_node_new(rb_tree * tree, void * key, void * val);
void rb_node_delete(rb_tree * tree, rb_node * node);
void rb_node_set_key(rb_tree * tree, rb_node * node, void * key);
void rb_node_set_val(rb_tree * tree, rb_node * node, void * val);

#ifdef __cplusplus
}
#endif

#endif//__BTN_RB_TREE_H__
