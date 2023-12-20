#define CPM_SUPPRESS_LINKER_WARNING
#include "CPM.h"

int main(int argc, char** argv)
{
    CPM_REBUILD_SELF(argc, argv);
    // BuildProperties_t bp;
    // cpm_configure_compiler(&bp, "clang", "fmts/test.c", NULL,  "./fmts", "test", NULL);
    // cpm_configure_linker(&bp, NULL, NULL, NULL, NULL);
    // cpm_build(&bp);
    // cpm_link(&bp);
    // cpm_buildprop_cleanup(&bp);

    // String_t str = dir_glob("fmts/raylib/src", "*.c");
    // cpm_log(CPM_LOG_INFO, "%s\n", str.m_inner_ptr);
    // string_free(&str);
    cpm_log(CPM_LOG_INFO, "%d\n", __LINE__);
}