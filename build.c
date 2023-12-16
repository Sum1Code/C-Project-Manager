#include "CPM.h"

int main(int argc, char** argv)
{
    CPM_REBUILD_SELF(argc, argv);
    cpm_build_now();
    cpm_build_async();
}