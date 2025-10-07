/* Lost pointer when realloc fails (tagger_lib.c:68) */

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
    RecursionContext Ctx = {0};
    Ctx.ActiveCount = 1;
    Ctx.ActiveTags = malloc(sizeof(char *));
    Ctx.ActiveTags[0] = strdup("only-entry");

    char **OriginalArray = Ctx.ActiveTags;

    AddActiveTag( &Ctx, "new-entry" );

    printf( "Ctx.ActiveTags is now %p (expected original array)\n", (void *)Ctx.ActiveTags );
    printf( "Original array still allocated at %p but leaked by caller\n", (void *)OriginalArray );

    free( OriginalArray[0] );
    free( OriginalArray );
    return 0;
}
