#include "CPM.h"

int main(int argc, char** argv)
{
    cpm_log(CPM_LOG_INFO, "Hello,world!%c%s%s%d%f\n", 'a', "bsfad", "ccsa", 123, 12321.44);
}