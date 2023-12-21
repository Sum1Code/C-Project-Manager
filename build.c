#define CPM_SUPPRESS_LINKER_WARNING
#include "CPM.h"

int main(int argc, char **argv) {
  CPM_REBUILD_SELF(argc, argv);
  cpm_log(CPM_LOG_INFO, "direxists: %d\n", dir_ops(DIR_DELETE, "./build"));
}
