#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../util_lib.h"
#include "../tagger_lib.h"

void TestSimpleReplacemenet( void )
{
    TagDictionary Dict;
    Dict.Count = 2;
    Dict.Tags = malloc( sizeof( Tag ) * Dict.Count );
    if( Dict.Tags == NULL )
    {
        Error( "Failed to allocate TagDictionary" );
        exit( EXIT_FAILURE );
    }
    Dict.Tags[0].Name = "name";
    Dict.Tags[0].Replacement = "Alice";
    Dict.Tags[1].Name = "site";
    Dict.Tags[1].Replacement = "Example.com";

    RecursionContext Ctx;
    Ctx.Depth = 0;
    Ctx.ActiveTags = NULL;
    Ctx.ActiveCount = 0;

    const char *Input = "Hello, {{name}}! Welcome to {{site}}.";
    char *Output = ProcessInput( Input, &Ctx, &Dict );
    printf( "Test 1 Output: %s\n", Output );

    assert( strcmp( Output, "Hello, Alice! Welcome to Example.com." ) == 0 );

    free( Output );
    free( Dict.Tags );
}

void TestRecursiveReplacement( void )
{
    TagDictionary Dict;
    Dict.Count = 2;
    Dict.Tags = malloc( sizeof( Tag ) * Dict.Count );
    if ( Dict.Tags == NULL ) {
        Error( "Failed to allocate TagDictionary" );
        exit( EXIT_FAILURE );
    }
    Dict.Tags[0].Name = "name";
    Dict.Tags[0].Replacement = "Bob";
    Dict.Tags[1].Name = "greeting";
    Dict.Tags[1].Replacement = "Hi, {{name}}";

    RecursionContext Ctx;
    Ctx.Depth = 0;
    Ctx.ActiveTags = NULL;
    Ctx.ActiveCount = 0;

    const char *Input = "{{greeting}}, welcome!";
    char *Output = ProcessInput( Input, &Ctx, &Dict );
    printf( "Test 2 Output: %s\n", Output );

    assert( strcmp( Output, "Hi, Bob, welcome!" ) == 0 );

    free( Output );
    free( Dict.Tags );
}

int main( void )
{
    TestSimpleReplacemenet();
    TestRecursiveReplacement();
    printf( "All tests passed!\n" );
    return 0;
}

