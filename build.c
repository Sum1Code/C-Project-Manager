#define GCC_VERSION 9
#include "CPM.h"

int main(int argc, char** argv)
{
    BuildProperties_t bp;
    cpm_configure_compiler(&bp, "clang", "test/test.c", NULL,  NULL, "test", NULL);
    cpm_configure_linker(&bp, NULL, NULL, NULL, NULL);
    cpm_build(&bp);
    dir_ops(DIR_CHECK, "build");
}