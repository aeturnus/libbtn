#include <gtest/gtest.h>
#include <btn/vector.h>
#include <btn/cstr.h>

int ctor_count = 0;
int dtor_count = 0;
void count_reset(void)
{
    ctor_count = 0;
    dtor_count = 0;
}
void ctor_up(void * arg)
{
    ctor_count += 1;
}
void dtor_up(void * arg)
{
    dtor_count += 1;
}

TEST(vector, ctor_dtor)
{
    vector(int) v;

    count_reset();
    vector_ctor(&v, sizeof(int), ctor_up, dtor_up);
    vector_dtor(&v);
    ASSERT_EQ(ctor_count, dtor_count);

    count_reset();
    vector_ctor_size(&v, sizeof(int), 17, ctor_up, dtor_up);
    vector_dtor(&v);
    ASSERT_EQ(ctor_count, dtor_count);

    count_reset();
    vector_ctor_size_cap(&v, sizeof(int), 17, 34, ctor_up, dtor_up);
    vector_dtor(&v);
    ASSERT_EQ(ctor_count, dtor_count);
}

TEST(vector, accessor)
{
    vector(int) v;

    count_reset();
    vector_ctor(&v, sizeof(int), ctor_up, dtor_up);

    for (int i = 0; i < 10; ++i) {
        vector_push_back(&v, &i);
    }
    ASSERT_EQ(10, vector_size(&v));
    for (int i = 0; i < 10; ++i) {
        int val;
        vector_get(&v, i, &val);
        ASSERT_EQ(i, val);
    }

    vector_dtor(&v);
}

TEST(vector, resize)
{
    vector(int) v;

    vector_ctor(&v, sizeof(int), ctor_up, dtor_up);
    count_reset();
    vector_resize(&v, 100);
    ASSERT_EQ(100, vector_size(&v));
    ASSERT_EQ(100, ctor_count);
    vector_resize(&v, 63);
    ASSERT_EQ(63, vector_size(&v));
    ASSERT_EQ(37, dtor_count);
}

TEST(vector, insert)
{
    vector(int) v;

    vector_ctor(&v, sizeof(int), ctor_up, dtor_up);
    count_reset();
    for (int i = 0; i < 10; ++i) {
        vector_push_back(&v, &i);
    }
    int in_val = 23;
    vector_insert(&v, 5, &in_val);

    ASSERT_EQ(11, vector_size(&v));

    for (int i = 0; i < 11; ++i) {
        int val;
        vector_get(&v, i, &val);
        if (i < 5)
            ASSERT_EQ(i, val) << "at index " << i;
        else if (i == 5)
            ASSERT_EQ(in_val, val) << "at index " << i;
        else
            ASSERT_EQ(i-1, val) << "at index " << i;
    }
}

TEST(vector, erase)
{
    vector(int) v;

    vector_ctor(&v, sizeof(int), ctor_up, dtor_up);
    count_reset();
    for (int i = 0; i < 10; ++i) {
        vector_push_back(&v, &i);
    }
    vector_erase(&v, 5);

    ASSERT_EQ(9, vector_size(&v));

    for (int i = 0; i < 9; ++i) {
        int val;
        vector_get(&v, i, &val);
        if (i < 5)
            ASSERT_EQ(i, val) << "at index " << i;
        else
            ASSERT_EQ(i+1, val) << "at index " << i;
    }
}

TEST(vector, to_array)
{
    vector(int) v;

    count_reset();
    vector_ctor(&v, sizeof(int), ctor_up, dtor_up);

    for (int i = 0; i < 10; ++i) {
        vector_push_back(&v, &i);
    }
    for (int i = 0; i < 10; ++i) {
        int val;
        vector_get(&v, i, &val);
    }
    int * arr = (int *) vector_to_array(&v);

    vector_dtor(&v);

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(i, arr[i]);
    }
}

int free_count = 0;
void string_free(char ** str_ptr)
{
    ++free_count;
    char * str = * str_ptr;
    free(str);
}
#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))
TEST(vector, pointer_types)
{
    const char * strings[] = {
        "Hello world!",
        "How do you do!",
        "Za warudo",
        "aaaabbbbccc",
        "ddeeefff",
        "ggghhiiijjkkll"
    };

    vector(char *) v;
    vector_ctor(&v, sizeof(char *), NULL, dtor_cast(string_free));
    int len = ARRAY_LEN(strings);
    for (int i = 0; i < len; ++i) {
        char * str = strdup(strings[i]);
        vector_push_back(&v, &str);
    }
    ASSERT_EQ(len, vector_size(&v));
    for (int i = 0; i < len; ++i) {
        char * str;
        vector_get(&v, i, &str);
        ASSERT_STREQ(strings[i], str);
    }
    vector_dtor(&v);
    ASSERT_EQ(len, free_count);
}
