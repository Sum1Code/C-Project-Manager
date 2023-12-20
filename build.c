#define CPM_SUPPRESS_LINKER_WARNING
#include "CPM.h"

int main(int argc, char** argv)
{
    CPM_REBUILD_SELF(argc, argv);
    dir_ops(DIR_CHECK);
}