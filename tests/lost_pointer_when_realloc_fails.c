/* Lost pointer when realloc fails (tagger_lib.c:68):
 * compile with gcc reproduce_realloc_loss.c tagger_lib.c util_lib.c -o reproduce_realloc_loss.
 * */

#include "tagger_lib.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *realloc(void *ptr, size_t size)
{
    (void)ptr; (void)size;
    errno = ENOMEM;
    return NULL;  // force AddActiveTag to hit the failure path
}

int main(void)
{
    RecursionContext ctx = {0};
    ctx.ActiveCount = 1;
    ctx.ActiveTags = malloc(sizeof(char *));
    ctx.ActiveTags[0] = strdup("only-entry");

    char **original_array = ctx.ActiveTags;

    AddActiveTag(&ctx, "new-entry");

    printf("Ctx.ActiveTags is now %p (expected original array)\n", (void *)ctx.ActiveTags);
    printf("Original array still allocated at %p but leaked by caller\n", (void *)original_array);

    free(original_array[0]);
    free(original_array);
    return 0;
}
