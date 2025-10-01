/* Active-tag removal shift bug (tagger_lib.c:35):
 * run gcc reproduce_remove_active.c tagger_lib.c util_lib.c -o active_tag_removal_shift.
 * */

#include "tagger_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    RecursionContext Ctx = {0};
    Ctx.ActiveCount = 2;
    Ctx.ActiveTags = malloc( Ctx.ActiveCount * sizeof(char *) );
    Ctx.ActiveTags[0] = strdup( "outer" );
    Ctx.ActiveTags[1] = strdup( "inner" );

    char *FirstSlot = Ctx.ActiveTags[0];
    char *SecondSlot = Ctx.ActiveTags[1];

    RemoveActiveTag( &Ctx, "inner" );

    printf( "ActiveCount = %d (expected 1)\n", Ctx.ActiveCount);
    printf( "ActiveTags[0] == outer? %s\n", Ctx.ActiveTags[0] == FirstSlot ? "yes" : "no -> overwritten" );
    printf( "ActiveTags[0] == former inner? %s\n", Ctx.ActiveTags[0] == SecondSlot ? "yes (dangling)" : "no" );

    free( FirstSlot );
    free( Ctx.ActiveTags );
    return 0;
  }
