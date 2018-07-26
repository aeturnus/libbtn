#include <gtest/gtest.h>

#include <btn/bst.h>
#include <btn/map.h>
#include <btn/container.h>
#include <btn/cstr.h>

static
int strcmp_shim(const void * a1, const void * a2)
{
    char * s1 = (char *) a1;
    char * s2 = (char *) a2;
    return strcmp(s1, s2);
}

TEST(bst_node, access)
{
    bst(char *, int) tree;
    bst_ctor(&tree, sizeof(char *), sizeof(int),
                 btn_free_shim, NULL, strcmp_shim);

    const char * expect_string = "John Doe";
    int expect_val = 25;

    char * copy_string = strdup(expect_string);
    bst_node * node = bst_node_new(&tree, &copy_string, &expect_val);

    char * key;
    int    val;
    bst_node_get_key(&tree, node, &key);
    bst_node_get_val(&tree, node, &val);
    ASSERT_STREQ(expect_string, key);
    ASSERT_EQ(expect_val, val);
    bst_node_delete(&tree, node);

    bst_dtor(&tree);
}

typedef struct _str_int
{
    const char * key;
    int val;
} str_int;

#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))
TEST(bst, insert_find)
{
    bst(char *, int) tree;
    bst_ctor(&tree, sizeof(char *), sizeof(int),
             NULL, NULL, strcmp_shim);

    static const str_int pairs[] = {
        {"John Doe", 25},
        {"Jane Doe", 82},
        {"Austin Smith", 33},
        {"Hannah Reed", 94},
        {"Victor Bovik", 32},
    };

    int len = ARRAY_LEN(pairs);
    for (int i = 0; i < len; ++i) {
        bst_insert(&tree, &pairs[i].key, &pairs[i].val);
    }

    ASSERT_EQ(len, bst_size(&tree));


    for (int i = 0; i < len; ++i) {
        int    val;
        ASSERT_TRUE(bst_find(&tree, &pairs[i].key, &val));
        ASSERT_EQ(pairs[i].val, val);
    }

    bst_dtor(&tree);
}

TEST(bst_map, insert_find)
{
    bst(char *, int) tree;
    bst_ctor(&tree, sizeof(char *), sizeof(int),
             NULL, NULL, strcmp_shim);

    static const str_int pairs[] = {
        {"John Doe", 25},
        {"Jane Doe", 82},
        {"Austin Smith", 33},
        {"Hannah Reed", 94},
        {"Victor Bovik", 32},
    };

    int len = ARRAY_LEN(pairs);
    for (int i = 0; i < len; ++i) {
        map_insert(&tree, &pairs[i].key, &pairs[i].val);
    }

    ASSERT_EQ(len, map_size(&tree));


    for (int i = 0; i < len; ++i) {
        int    val;
        ASSERT_TRUE(map_find(&tree, &pairs[i].key, &val));
        ASSERT_EQ(pairs[i].val, val);
    }

    bst_dtor(&tree);
}

int intcmp_shim(const void * a1, const void * a2)
{
    const int * i1 = (const int *) a1;
    const int * i2 = (const int *) a2;
    return (*i1) - (*i2);
}

typedef struct _int_int
{
    int key;
    int val;
} int_int;

TEST(bst_iterator, traverse)
{
    bst(int, int) tree;
    bst_ctor(&tree, sizeof(int), sizeof(int),
             NULL, NULL, intcmp_shim);

    static const int_int pairs[] = {
        {10, ~10},

        {5, ~5},
        {2, ~2},
        {1, ~1},
        {3, ~3},
        {4, ~4},
        {7, ~7},
        {6, ~6},
        {9, ~9},
        {8, ~8},

        {15,~15},
        {12,~12},
        {11,~11},
        {14,~14},
        {13,~13},
        {19,~19},
        {20,~20},
        {16,~16},
        {17,~17},
        {18,~18},
    };

    int len = ARRAY_LEN(pairs);
    for (int i = 0; i < len; ++i) {
        bst_insert(&tree, &pairs[i].key, &pairs[i].val);
    }

    bst_it it;
    it_begin(&tree, &it);

    int count = 0;
    int arr[ARRAY_LEN(pairs)];
    while (!it_is_end(&it)) {
        bst_node * node;
        it_read(&it, &node);
        int key;
        int val;
        bst_node_get_key(&tree, node, &key);
        bst_node_get_val(&tree, node, &val);
        ASSERT_EQ(val, ~key);

        arr[count] = key;
        ++count;
        it_next(&it, 1);
    }

    ASSERT_EQ(len, count);

    // make sure we read them in order
    for (int i = 0; i < ARRAY_LEN(pairs); ++i) {
        ASSERT_EQ(i + 1, arr[i]);
    }

    bst_dtor(&tree);
}
