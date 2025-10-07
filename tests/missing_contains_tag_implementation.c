/* Missing ContainsTag implementation (util_lib.h:36) */

#include "util_lib.h"
#include <stdio.h>

int main(void)
{
    int has_tag = ContainsTag("Hello {{name}}", "{{");
    printf("ContainsTag returned %d\n", has_tag);
    return 0;
}

