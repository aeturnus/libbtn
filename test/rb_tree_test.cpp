#include <gtest/gtest.h>

#include <btn/rb_tree.h>
#include <btn/container.h>
#include <btn/cstr.h>

static
int strcmp_shim(void * a1, void * a2)
{
    char * s1 = (char *) a1;
    char * s2 = (char *) a2;
    return strcmp(s1, s2);
}

TEST(rb_node, access)
{
    rb_tree(char *, int) tree;
    rb_tree_ctor(&tree, sizeof(char *), sizeof(int),
                 btn_free_shim, NULL, strcmp_shim);

    const char * expect_string = "John Doe";
    int expect_val = 25;

    char * copy_string = strdup(expect_string);
    rb_node * node = rb_node_new(&tree, &copy_string, &expect_val);

    char * key;
    int    val;
    rb_node_get_key(&tree, node, &key);
    rb_node_get_val(&tree, node, &val);
    ASSERT_STREQ(expect_string, key);
    ASSERT_EQ(expect_val, val);
    rb_node_delete(&tree, node);
}
