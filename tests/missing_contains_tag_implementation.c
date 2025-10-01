/* Missing ContainsTag implementation (util_lib.h:36):
 * compile with gcc reproduce_contains_missing.c tagger_lib.c util_lib.c -o reproduce_contains_missing.
 */

#include "util_lib.h"
#include <stdio.h>

int main(void)
{
    int has_tag = ContainsTag("Hello {{name}}", "{{");
    printf("ContainsTag returned %d\n", has_tag);
    return 0;
}

