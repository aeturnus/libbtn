#include <gtest/gtest.h>
#include <btn/vector.h>

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
    ASSERT_EQ(100, ctor_count);
    vector_resize(&v, 63);
    ASSERT_EQ(37, dtor_count);
}
