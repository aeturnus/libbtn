#include <cstdio>

#include <gtest/gtest.h>
#include <btn/print.h>
#include <btn/ansi.h>

TEST(print, afprintf)
{
    const char * print  = ANSI_F_RED "Red "
                          ANSI_F_WHT "White "
                          ANSI_F_BLU "Blue"
                          ANSI_RESET;
    const char * expect = "Red White Blue";

    FILE * file = NULL;
    file = fopen("temp.txt", "w");
    afprintf(file, print);
    fclose(file);

    static char buffer[128];
    file = fopen("temp.txt", "r");
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, 0);
    size_t read = fread(buffer, sizeof(char), len, file);
    fclose(file);

    ASSERT_STREQ(expect, buffer) << "ftell: " << len << std::endl << "read " << read << " bytes";
}
