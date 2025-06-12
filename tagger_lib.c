#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tagger_lib.h"
#include "util_lib.h"

int IsTagActive( RecursionContext *Ctx, const char *TagName )
{
    if( Ctx == NULL || TagName == NULL )
        return -1;

    for( int Index = 0; Index < Ctx->ActiveCount; Index++ )
    {
        if( !strcmp( Ctx->ActiveTags[Index], TagName ) )
            return 1;
    }

    return 0;
}

void RemoveActiveTag( RecursionContext *Ctx, const char *TagName )
{
    if( Ctx == NULL || TagName == NULL )
        return;

    for( int Index = 0; Index < Ctx->ActiveCount; Index++ )
    {
        if( !strcmp( Ctx->ActiveTags[Index], TagName ) )
        {
            free( Ctx->ActiveTags[Index] );

            for( int SubIndex = 0; SubIndex < Ctx->ActiveCount - 1; SubIndex++ )
            {
                Ctx->ActiveTags[SubIndex] = Ctx->ActiveTags[SubIndex + 1];
            }

            Ctx->ActiveCount--;
            Ctx->ActiveTags = (char**)realloc( Ctx->ActiveTags, Ctx->ActiveCount * sizeof(char*) );
            return;
        }
    }
}

char* LookupTag( TagDictionary *Dict, const char *TagName )
{
    if( Dict == NULL || TagName == NULL )
        return NULL;

    for( int Index = 0; Index < Dict->Count; Index++ )
    {
        if( !strcmp( Dict->Tags[Index].Name, TagName ) )
        {
            return Dict->Tags[Index].Replacement;
        }
    }

    return NULL;
}

void AddActiveTag( RecursionContext *Ctx, const char *TagName )
{
    if( Ctx == NULL || TagName == NULL )
        return;

    Ctx->ActiveTags = (char**)realloc( Ctx->ActiveTags, ( Ctx->ActiveCount + 1 ) * sizeof(char*) );

    if( Ctx->ActiveTags == NULL )
    {
        PrintError( "Failed to reallocate memory" );
        return;
    }

    Ctx->ActiveTags[Ctx->ActiveCount] = strdup( TagName );
    if( Ctx->ActiveTags[Ctx->ActiveCount] == NULL )
    {
        PrintError( "Failed to duplicate active tag name" );
        return;
    }
    Ctx->ActiveCount++;
}

char* ResolveTag( const char *TagName, RecursionContext *Ctx, TagDictionary *Dict )
{
    if( TagName == NULL || Ctx == NULL || Dict == NULL )
        return NULL;

    if( Ctx->Depth >= MAX_RECURSION_DEPTH )
        return NULL;

    if( IsTagActive( Ctx, TagName ) )
        return NULL;

    for( int Index = 0; Index < Dict->Count; Index++ )
    {
        if( !strcmp( Dict->Tags[Index].Name, TagName ) )
        {
            Ctx->Depth++;

            AddActiveTag( Ctx, TagName );

            char* ResolvedText = ProcessInput( Dict->Tags[Index].Replacement, Ctx, Dict );
    
            RemoveActiveTag( Ctx, TagName );

            Ctx->Depth--;

            return ResolvedText;
        }
    }

    return NULL;

} /* ResolveTag */

char* ProcessInput( const char *Input, RecursionContext *Ctx, TagDictionary *Dict )
{
    char* Result = AllocateBuffer( BUFFER_SIZE + 1 );

    int Pos = 0;
    int InputLength = strlen( Input );
    while( Pos < InputLength )
    {
        if( Matches( Input, Pos, START_TAG ) )
        {
            char* TagName = ExtractTagName( Input, &Pos, START_TAG, END_TAG ); 

            char* Replacement = ResolveTag( TagName, Ctx, Dict );

            AppendToBuffer( &Result, Replacement );

            free( TagName );
            free( Replacement );
        }
        else
        {
            char* PlainText = ExtractPlainText( Input, &Pos, START_TAG ); 
            AppendToBuffer( &Result, PlainText );
            free( PlainText );
        }
    }

    return Result;
}

