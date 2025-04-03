#include "tagger_lib.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *Input = "Hello, {{name}}! Welcome to {{site}}.";

    TagDictionary Dict;

    Dict.Count = 2;
    Dict.Tags = malloc( Dict.Count * sizeof(Tag) );

    Dict.Tags[0].Name = "name";
    Dict.Tags[0].Replacement = "Qasim";

    Dict.Tags[1].Name = "site";
    Dict.Tags[1].Replacement = "example.com";

    RecursionContext Ctx;
    Ctx.Depth = 0;
    Ctx.ActiveTags = NULL;
    Ctx.ActiveCount = 0;

    char *Result = ProcessInput( Input, &Ctx, &Dict );
    printf( "%s\n", Result );

    free( Result );

    return 0;
}
