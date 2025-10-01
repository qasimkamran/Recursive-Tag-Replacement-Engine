/* Progress bar scaling error (benchmark.c:66, util_lib.c:11):
 * run gcc reproduce_progress_bar.c util_lib.c -o reproduce_progress_bar.
 */

#include "util_lib.h"
#include <stdio.h>

int main(void)
{
    printf("Benchmark-style call with 100.0 (expectation: 10000%% displayed)\n");
    PrintProgressBar(100.0);
    printf("\n");
    return 0;
}

