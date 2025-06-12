#include "util_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

void PrintProgressBar( double Fraction )
{
    int Completed = (int)( Fraction * PROGRESS_BAR_WIDTH );

    int Index;

    printf("\r[");

    for( Index = 0; Index < Completed; ++Index )
    {
        putchar('=');
    }

    if( Completed < PROGRESS_BAR_WIDTH )
    {
        putchar('>');
        for( Index = Completed + 1; Index < PROGRESS_BAR_WIDTH; ++Index )
        {
            putchar(' ');
        }
    }
    else
    {
        putchar('=');
    }

    printf( "] %3.0f%%", Fraction * 100.0 );
    fflush( stdout );
}

void PrintError( const char* Message )
{
    perror( Message );
}

void StandardError( const char *Format, ... )
{
    va_list Args;
    va_start( Args, Format );

    vfprintf( stderr, Format, Args );

    va_end( Args );
}

char* AllocateBuffer( size_t BufferSize )
{
    char* Buffer = (char*)malloc( BufferSize * sizeof(char) );

    if( Buffer == NULL )
    {
        PrintError( "Failed to allocate memory to string buffer" );
        return NULL;
    }

    for( size_t Index = 0; Index < BufferSize; Index++ )
        Buffer[Index] = '\0';

    return Buffer;
}

void AppendToBuffer( char **Buffer, const char *Text )
{
    if( Text == NULL ) return;

    size_t CurrentLength = *Buffer ? strlen( *Buffer ) : 0;
    size_t TextLength = strlen( Text );

    char* NewBuffer = (char*) realloc( *Buffer, ( CurrentLength + TextLength + 1 ) * sizeof(char) );
    if( NewBuffer == NULL )
    {
        PrintError( "Failed to reallocate memory" );
        return;
    }

    *Buffer = NewBuffer;
    strcpy( *Buffer + CurrentLength, Text );
}

int Matches( const char *Input, int Pos, const char *Pattern )
{
    if( Input == NULL || Pos < 0 || Pattern == NULL )
        return -1;

    size_t InputLength = strlen( Input );
    if( Pos >= InputLength ) 
        return -1;

    size_t PatternLength = strlen( Pattern );

    if( Pos + PatternLength > InputLength )
        return 0;

    for( size_t Index = 0; Index < PatternLength; Index++ )
    {
        if( Input[Pos + Index] != Pattern[Index] )
            return 0;
    }

    return 1;
}

char* ExtractTagName( const char *Input, int *Pos, const char *TagStart, const char *TagEnd )
{
    if( Input == NULL || Pos == NULL || TagStart == NULL || TagEnd == NULL )
        return NULL;

    const char* StartPos = strstr( Input + *Pos, TagStart );
    if( StartPos == NULL )
        return NULL;

    StartPos += strlen( TagStart );

    const char* EndPos = strstr( StartPos, TagEnd );
    if( EndPos == NULL )
        return NULL;

    size_t TagLength = EndPos - StartPos;

    char* TagName = AllocateBuffer( TagLength + 1 );
    if( TagName == NULL )
        return NULL;

    strncpy( TagName, StartPos, TagLength );
    TagName[TagLength] = '\0';

    *Pos = EndPos - Input + strlen( TagEnd );

    return TagName;
}

char* ExtractPlainText( const char* Input, int* Pos, const char* TagStart )
{
    if( Input == NULL || Pos == NULL || TagStart == NULL )
        return NULL;

    const char* NextTagStart = strstr( Input + *Pos, TagStart );

    const char* EndPos = NextTagStart ? NextTagStart : Input + strlen( Input );

    size_t PlainTextLength = EndPos - ( Input + *Pos );

    char* PlainText = AllocateBuffer( PlainTextLength + 1 );
    if( PlainText == NULL )
        return NULL;

    strncpy( PlainText, Input + *Pos, PlainTextLength );
    PlainText[PlainTextLength] = '\0';

    *Pos = EndPos - Input;

    return PlainText;
}

