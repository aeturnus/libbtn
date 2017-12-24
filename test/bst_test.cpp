#include <gtest/gtest.h>

#include <btn/bst.h>
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
}
