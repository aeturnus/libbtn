#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <btn/bst.h>

struct _bst_node
{
    rb_color  c;
    bst_node * p;
    bst_node * l;
    bst_node * r;
    uint8_t kv[];   // flexible array member to hold the key and value
                    // [key, val]
};

static inline void * keyp(bst * tree, bst_node * node);
static inline void * valp(bst * tree, bst_node * node);

void bst_ctor(bst * tree,
                  size_t key_size, size_t val_size,
                  void (* key_dtor)(void *), void (* val_dtor)(void *),
                  int (* key_cmp)(const void * k1, const void * k2))
{
    tree->root = NULL;
    tree->size = 0;
    tree->key_size = key_size;
    tree->val_size = val_size;
    tree->key_dtor = key_dtor;
    tree->val_dtor = val_dtor;
    tree->key_cmp  = key_cmp;
}

bool bst_insert(bst * tree, const void * key, const void * val)
{
    void * find = bst_getp(tree, key);
    if (find != NULL)
        return false;

    bst_node * node = bst_node_new(tree, key, val);

    // insertion logic

    return true;
}

bool bst_erase(bst * tree, const void * key)
{
}

bool bst_get(bst * tree, const void * key, void * val)
{
    void * src = bst_getp(tree, key);
    memcpy(val, src, tree->val_size);
}

void * bst_getp(bst * tree, const void * key)
{
    bst_node * curr = tree->root;
    while (curr != NULL) {
        void * node_key = keyp(tree, curr);
        int cmp = tree->key_cmp(key, node_key);
        if (cmp == 0) {
            return valp(tree, curr);
        } else if (cmp < 0) {
            curr = curr->l;
        } else {
            curr = curr->r;
        }
    }

    // if it gets to this point, haven't found it
    return NULL;
}

// node stuff down

static inline
void * keyp(bst * tree, bst_node * node)
{
    // offset 0
    return node->kv;
}

static inline
void * valp(bst * tree, bst_node * node)
{
    // offset by key
    return node->kv + tree->key_size;
}

rb_color bst_node_color(bst * tree, bst_node * node) { return node->c; }
bst_node * bst_node_p(bst * tree, bst_node * node) { return node->p; }
bst_node * bst_node_l(bst * tree, bst_node * node) { return node->l; }
bst_node * bst_node_r(bst * tree, bst_node * node) { return node->r; }
void bst_node_get_key(bst * tree, bst_node * node, void * key)
{
    void * src = keyp(tree, node);
    memcpy(key, src, tree->key_size);
}

void bst_node_get_val(bst * tree, bst_node * node, void * val)
{
    void * src = valp(tree, node);
    memcpy(val, src, tree->val_size);
}

void * bst_node_getp_key(bst * tree, bst_node * node)
{
    return keyp(tree, node);
}

void * bst_node_getp_val(bst * tree, bst_node * node)
{
    return valp(tree, node);
}

void bst_node_set_key(bst * tree, bst_node * node, const void * key)
{
    void * dst = keyp(tree, node);
    memcpy(dst, key, tree->key_size);
}

void bst_node_set_val(bst * tree, bst_node * node, const void * val)
{
    void * dst = valp(tree, node);
    memcpy(dst, val, tree->val_size);
}

bst_node * bst_node_new(bst * tree, const void * key, const void * val)
{
    size_t size = sizeof(bst_node) + tree->key_size + tree->val_size;
    bst_node * node = (bst_node *) malloc(size);
    bst_node_set_key(tree, node, key);
    bst_node_set_val(tree, node, val);
    node->p = NULL;
    node->l = NULL;
    node->r = NULL;
    node->c = RB_BLK;
    return node;
}

void bst_node_delete(bst * tree, bst_node * node)
{
    if (tree->key_dtor != NULL)
        tree->key_dtor(keyp(tree, node));
    if (tree->val_dtor != NULL)
        tree->val_dtor(valp(tree, node));
    free(node);
}
