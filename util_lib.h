#include <stddef.h>

#define BUFFER_SIZE 32
#define PROGRESS_BAR_WIDTH 50

void PrintProgressBar( double Fraction );

char* AllocateBuffer( size_t BufferSize );

int Matches( const char* Input, int Pos, const char* Pattern );

char* ExtractTagName( const char* Input, int* Pos, const char* TagStart, const char* TagEnd );

char* ExtractPlainText( const char* Input, int* Pos, const char* TagStart );

int ContainsTag( const char* Text, const char* TagStart );

void AppendToBuffer( char** Buffer, const char* Text );

void PrintError( const char* Messsage );

void StandardError( const char *Format, ... );

