#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <btn/rb_tree.h>

struct _rb_node
{
    rb_color  c;
    rb_node * p;
    rb_node * l;
    rb_node * r;
    uint8_t kv[];   // flexible array member to hold the key and value
                    // [key, val]
};

void rb_tree_ctor(rb_tree * tree,
                  size_t key_size, size_t val_size,
                  void (* key_dtor)(void *), void (* val_dtor)(void *),
                  int (* key_cmp)(void * k1, void * k2))
{
    tree->root = NULL;
    tree->size = 0;
    tree->key_size = key_size;
    tree->val_size = val_size;
    tree->key_dtor = key_dtor;
    tree->val_dtor = val_dtor;
    tree->key_cmp  = key_cmp;
}

static inline
void * keyp(rb_tree * tree, rb_node * node)
{
    // offset 0
    return node->kv;
}

static inline
void * valp(rb_tree * tree, rb_node * node)
{
    // offset by key
    return node->kv + tree->key_size;
}

rb_color rb_node_color(rb_tree * tree, rb_node * node) { return node->c; }
rb_node * rb_node_p(rb_tree * tree, rb_node * node) { return node->p; }
rb_node * rb_node_l(rb_tree * tree, rb_node * node) { return node->l; }
rb_node * rb_node_r(rb_tree * tree, rb_node * node) { return node->r; }
void rb_node_get_key(rb_tree * tree, rb_node * node, void * key)
{
    void * src = keyp(tree, node);
    memcpy(key, src, tree->key_size);
}

void rb_node_get_val(rb_tree * tree, rb_node * node, void * val)
{
    void * src = valp(tree, node);
    memcpy(val, src, tree->val_size);
}

void * rb_node_getp_key(rb_tree * tree, rb_node * node)
{
    return keyp(tree, node);
}

void * rb_node_getp_val(rb_tree * tree, rb_node * node)
{
    return valp(tree, node);
}

void rb_node_set_key(rb_tree * tree, rb_node * node, void * key)
{
    void * dst = keyp(tree, node);
    memcpy(dst, key, tree->key_size);
}

void rb_node_set_val(rb_tree * tree, rb_node * node, void * val)
{
    void * dst = valp(tree, node);
    memcpy(dst, val, tree->val_size);
}

rb_node * rb_node_new(rb_tree * tree, void * key, void * val)
{
    size_t size = sizeof(rb_node) + tree->key_size + tree->val_size;
    rb_node * node = (rb_node *) malloc(size);
    rb_node_set_key(tree, node, key);
    rb_node_set_val(tree, node, val);
    node->p = NULL;
    node->l = NULL;
    node->r = NULL;
    node->c = RB_BLK;
    return node;
}

void rb_node_delete(rb_tree * tree, rb_node * node)
{
    if (tree->key_dtor != NULL)
        tree->key_dtor(keyp(tree, node));
    if (tree->val_dtor != NULL)
        tree->val_dtor(valp(tree, node));
    free(node);
}
