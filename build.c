#include "CPM.h"

int main(int argc, char** argv)
{
    cpm_log(CPM_LOG_INFO, "Hello,world! %d%s%s%c%f\n", 23, "abcd ", "afeadadw ", 'f', 44.3);
    cpm_log(CPM_LOG_ERROR, "Hello,world! %d%s%s%c%f\n", 23, "abcd ", "afeadadw ", 'f', 44.3);
    cpm_log(CPM_LOG_WARNING, "Hello,world! %d%s%s%c%f\n", 23, "abcd ", "afeadadw ", 'f', 44.3);
    printf("test!\n");
}