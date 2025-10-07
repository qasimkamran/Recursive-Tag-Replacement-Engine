/* Progress bar scaling error (benchmark.c:66, util_lib.c:11) */

#include "util_lib.h"
#include <stdio.h>

int main(void)
{
    printf("Benchmark-style call with 100.0 (expectation: 10000%% displayed)\n");
    PrintProgressBar(100.0);
    printf("\n");
    return 0;
}

