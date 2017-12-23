#include <stdio.h>

#include <gtest/gtest.h>

#include <btn/tokenizer.h>
#include <btn/cstr.h>

#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))
TEST(tokenizer, single_delimiter)
{
    const char * test_string = "The cat in the hat spoke back";
    const char * expected[] = {"The", "cat", "in", "the", "hat", "spoke", "back"};
    tokenizer tok;
    tokenizer_ctor(&tok, test_string, " ");
    int len = ARRAY_LEN(expected);
    ASSERT_EQ(len, tokenizer_num_tokens(&tok));
    ASSERT_EQ(len, tokenizer_count_tokens(&tok));

    int i = 0;
    while (tokenizer_has_tokens(&tok)) {
        const char * token = tokenizer_next(&tok);
        ASSERT_STREQ(expected[i], token);
        ++i;
        ASSERT_EQ(len - i, tokenizer_count_tokens(&tok));
    }
}

TEST(tokenizer, multi_delimiter)
{
    const char * test_string = "The;cat.in-the$hat;spoke,back";
    const char * expected[] = {"The", "cat", "in", "the", "hat", "spoke", "back"};
    tokenizer tok;
    tokenizer_ctor(&tok, test_string, ".,;-$");
    int len = ARRAY_LEN(expected);
    ASSERT_EQ(len, tokenizer_num_tokens(&tok));
    ASSERT_EQ(len, tokenizer_count_tokens(&tok));

    int i = 0;
    while (tokenizer_has_tokens(&tok)) {
        const char * token = tokenizer_next(&tok);
        ASSERT_STREQ(expected[i], token);
        ++i;
        ASSERT_EQ(len - i, tokenizer_count_tokens(&tok));
    }
}

// appends number to str from [0, param)
void custom_tok (const char * str, size_t * size_p,
                 char *** elems_p, void * param)
{
    int count = *(int *)param;
    char * buffer = (char *) malloc(sizeof(char) * (strlen(str) + 4));
    char ** arr = (char **) malloc(sizeof(char *) * count);
    for (int i = 0; i < count; ++i) {
        sprintf(buffer, "%s%d", str, i);
        arr[i] = strdup(buffer);
    }
    *size_p  = count;
    *elems_p = arr;
}


TEST(tokenizer, custom_tokenizer)
{
    const char * test_string = "num";
    const char * expected[] = {"num0", "num1", "num2", "num3", "num4", "num5"};
    int len = ARRAY_LEN(expected);
    tokenizer tok;
    tokenizer_ctor_custom(&tok, test_string, custom_tok, &len);
    ASSERT_EQ(len, tokenizer_num_tokens(&tok));
    ASSERT_EQ(len, tokenizer_count_tokens(&tok));

    int i = 0;
    while (tokenizer_has_tokens(&tok)) {
        const char * token = tokenizer_next(&tok);
        ASSERT_STREQ(expected[i], token);
        ++i;
        ASSERT_EQ(len - i, tokenizer_count_tokens(&tok));
    }
}
