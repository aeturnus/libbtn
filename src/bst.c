#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <btn/bst.h>

static void bst_begin(bst * vec, bst_it * it);
static void bst_end(bst * vec, bst_it * it);
static void bst_it_next(bst_it * it, int n);
static void bst_it_prev(bst_it * it, int n);
static bool bst_it_insert(bst_it * it, const void * val);
static bool bst_it_remove(bst_it * it);
static bool bst_it_read(bst_it * it, void * dst);
static void * bst_it_ptr(bst_it * it);
static bool bst_it_is_begin(bst_it * it);
static bool bst_it_is_end(bst_it * it);

static bst_ops ops = {
    .map = {
        .insert = (bool (*)(void *, const void *, const void *)) bst_insert,
        .find   = (bool (*)(void *, const void *, void *)) bst_find,
        .findp  = (void * (*)(void *, const void *)) bst_findp,
        .erase  = (bool (*)(void *, const void *)) bst_erase,
        .size   = (size_t (*)(void *)) bst_size,
        .empty  = (bool (*)(void *)) bst_erase
    },
    .iterable = {
        .begin = (void (*)(void *, void*)) bst_begin,
        .end   = (void (*)(void *, void*)) bst_end
    }
};

static bst_it_ops it_ops = {
    .iterator = {
        .next   = (void (*)(void *, int)) bst_it_next,
        .prev   = (void (*)(void *, int)) bst_it_prev,
        .insert = (bool (*)(void *, const void *)) bst_it_insert,
        .remove = (bool (*)(void *)) bst_it_remove,
        .read   = (bool (*)(void *, void *)) bst_it_read,
        .ptr    = (void * (*)(void *)) bst_it_ptr,
        .is_begin   = (bool (*)(void *)) bst_it_is_begin,
        .is_end     = (bool (*)(void *)) bst_it_is_end
    }
};

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
static void bst_node_tree_delete(bst * tree, bst_node * root);

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
    tree->ops      = &ops;
}

void bst_dtor(bst * tree)
{
    bst_node_tree_delete(tree, tree->root);
    bst_node_delete(tree, tree->root);
}

bool bst_insert(bst * tree, const void * key, const void * val)
{

    // insertion logic
    bst_node * prev = NULL;
    bst_node * curr = tree->root;

    // Case: empty
    if (curr == NULL) {
        bst_node * node = bst_node_new(tree, key, val);
        tree->root = node;
        ++tree->size;
        return true;
    }

    // replication of search logic
    int cmp = 0;
    while (curr != NULL) {
        prev = curr;
        void * node_key = keyp(tree, curr);
        cmp = tree->key_cmp(key, node_key);
        if (cmp == 0) {
            return false;
        } else if (cmp < 0) {
            curr = curr->l;
        } else {
            curr = curr->r;
        }
    }

    // when it gets here, curr is NULL, prev is the parent
    bst_node * node = bst_node_new(tree, key, val);
    if (cmp < 0) {
        node->p = prev;
        prev->l = node;
    } else {
        node->p = prev;
        prev->r = node;
    }
    ++tree->size;

    return true;
}

bool bst_erase(bst * tree, const void * key)
{
    return false;
}

bool bst_find(bst * tree, const void * key, void * val)
{
    void * src = bst_findp(tree, key);
    if (src == NULL)
        return false;
    memcpy(val, src, tree->val_size);
    return true;
}

void * bst_findp(bst * tree, const void * key)
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
    if (node == NULL)
        return;
    if (tree->key_dtor != NULL)
        tree->key_dtor(keyp(tree, node));
    if (tree->val_dtor != NULL)
        tree->val_dtor(valp(tree, node));
    free(node);
}

static
void bst_node_tree_delete(bst * tree, bst_node * root)
{
    if (root == NULL)
        return;
    bst_node_tree_delete(tree, root->l);
    bst_node_delete(tree, root->l);
    bst_node_tree_delete(tree, root->r);
    bst_node_delete(tree, root->r);
}

// iteration funcs

static
bst_node * bst_lowest(bst * tree)
{
    bst_node * prev = NULL;
    bst_node * curr = tree->root;
    while (curr != NULL) {
        prev = curr;
        curr = curr->l;
    }
    return prev;
}

static
bst_node * bst_highest(bst * tree)
{
    bst_node * prev = NULL;
    bst_node * curr = tree->root;
    while (curr != NULL) {
        curr = curr->r;
    }
    return prev;
}

static
int bst_node_cmp(bst * tree, bst_node * n1, bst_node * n2)
{
    /*
    if (n1 != NULL && n2 == NULL)
        return -1;

    if (n1 == NULL && n2 == NULL)
        return 0;

    if (n1 == NULL && n2 != NULL)
        return 1;
    */

    void * k1  = keyp(tree, n1);
    void * k2  = keyp(tree, n2);

    return tree->key_cmp(k1, k2);
}

// return successor: returns NULL if no more
static
bst_node * bst_node_succ(bst * tree, bst_node * node)
{
    if (node == NULL)
        return NULL;

    // have a right child? traverse that child going left
    if (node->r != NULL) {
        bst_node * prev = node->r;
        bst_node * curr = node->r->l;
        while (curr != NULL) {
            prev = curr;
            curr = curr->l;
        }
        return prev;
    }

    // backtrack to find an ancestor that's greater
    bst_node * curr = node;
    while (curr->p != NULL) {
        int cmp = bst_node_cmp(tree, curr, curr->p);
        if (cmp < 0)
            return curr->p;
        else
            curr = curr->p;
    }
    return NULL;
}

// if provided with a NULL node, it's assumed to be the 'end' node
// return predecessor: returns NULL if no more
static
bst_node * bst_node_pred(bst * tree, bst_node * node)
{
    if (node == NULL) {
        return bst_highest(tree);
    }

    // have a left child? traverse that child going right
    if (node->l != NULL) {
        bst_node * prev = node->l;
        bst_node * curr = node->l->r;
        while (curr != NULL) {
            prev = curr;
            curr = curr->r;
        }
        return prev;
    }

    // backtrack to find an ancestor that's greater
    bst_node * curr = node;
    while (curr->p != NULL) {
        int cmp = bst_node_cmp(tree, curr, curr->p);
        if (cmp > 0)
            return curr->p;
        else
            curr = curr->p;
    }
    return NULL;
}

static void
bst_begin(bst * tree, bst_it * it)
{
    it->tree = tree;
    it->curr = bst_lowest(tree);
    it->ops  = &it_ops;
}

static void bst_end(bst * tree, bst_it * it)
{
    it->tree = tree;
    it->curr = NULL;
    it->ops  = &it_ops;
}

static void bst_it_next(bst_it * it, int n)
{
    if (n < 0) {
        bst_it_prev(it, -n);
    }

    for (; n > 0 && it->curr != NULL; --n) {
        it->curr = bst_node_succ(it->tree, it->curr);
    }
}

static void bst_it_prev(bst_it * it, int n)
{
    if (n < 0) {
        bst_it_next(it, -n);
    }

    bst_node * prev = NULL;
    for (; n > 0 && it->curr != prev; --n) {
        prev = it->curr;
        it->curr = bst_node_pred(it->tree, it->curr);
    }
}

static bool bst_it_insert(bst_it * it, const void * val)
{
    return false;
}

static bool bst_it_remove(bst_it * it)
{
    return false;
}

static bool bst_it_read(bst_it * it, void * dst)
{
    memcpy(dst, &it->curr, sizeof(bst_node *));
    return true;
}

static void * bst_it_ptr(bst_it * it)
{
    return &(it->curr);
}

static bool bst_it_is_begin(bst_it * it)
{
    return (it->curr == bst_lowest(it->tree));
}

static bool bst_it_is_end(bst_it * it)
{
    return (it->curr == NULL);
}
