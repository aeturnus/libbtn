#include <gtest/gtest.h>
#include <btn/cstr.h>

TEST(cstr, strdup)
{
    const char * string = "Hello world!";
    char * copy = strdup(string);
    ASSERT_STREQ(string, copy);
    free(copy);
}

TEST(cstr, stridup)
{
    const char * string = "Hello world!";
    char * copy = stridup(string, 3, 8);
    ASSERT_STREQ("lo wor", copy);
    free(copy);
}

TEST(cstr, char_to_str)
{
    char * str = char_to_str('g');
    ASSERT_STREQ("g", str);
    free(str);
}

TEST(cstr, strcmp_caseless)
{
    ASSERT_TRUE(strcmp_caseless("HELLO", "hello") == 0);
    ASSERT_TRUE(strcmp_caseless("hElLO", "hElLo") == 0);
    ASSERT_TRUE(strcmp_caseless("bat", "CAT") < 0);
    ASSERT_TRUE(strcmp_caseless("Cat", "bAT") > 0);
}

TEST(cstr, strfind)
{
    const char * string = "The cat in the hat spoke back";
    ASSERT_EQ(4, strfind(string, "cat", 0));
    ASSERT_EQ(15, strfind(string, "hat", 0));
    ASSERT_EQ(25, strfind(string, "back", 0));
    ASSERT_EQ(SIZE_MAX, strfind(string, "dog", 0));
}

TEST(cstr, strcfind)
{
    const char * string = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 26; ++i) {
        ASSERT_EQ(i, strcfind(string, 'a' + i, 0));
    }
}

TEST(cstr, to_upper)
{
    ASSERT_EQ('A', to_upper('a'));
    ASSERT_EQ('A', to_upper('A'));
    ASSERT_EQ('Z', to_upper('z'));
    ASSERT_EQ('Z', to_upper('Z'));
    ASSERT_EQ('M', to_upper('m'));
    ASSERT_EQ('M', to_upper('M'));
    ASSERT_EQ('$', to_upper('$'));
}

TEST(cstr, to_lower)
{
    ASSERT_EQ('a', to_lower('a'));
    ASSERT_EQ('a', to_lower('A'));
    ASSERT_EQ('z', to_lower('z'));
    ASSERT_EQ('z', to_lower('Z'));
    ASSERT_EQ('m', to_lower('m'));
    ASSERT_EQ('m', to_lower('M'));
    ASSERT_EQ('$', to_lower('$'));
}
