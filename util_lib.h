#include <stddef.h>

#define BUFFER_SIZE 32
#define PROGRESS_BAR_WIDTH 50

typedef struct
{
    char *Data;
    size_t Length;
    size_t Capacity;
} Buffer;

void PrintProgressBar( double Fraction );

void BufferInit( Buffer* Buf );

void BufferFree( Buffer* Buf );

int BufferReserve( Buffer* Buf, size_t Capacity );

char* AllocateBuffer( size_t BufferSize );

int Matches( const char* Input, int Pos, const char* Pattern );

char* ExtractTagName( const char* Input, int* Pos, const char* TagStart, const char* TagEnd );

char* ExtractPlainText( const char* Input, int* Pos, const char* TagStart );

int ContainsTag( const char* Text, const char* TagStart );

void AppendToBuffer( Buffer* Buf, const char* Text );

void PrintError( const char* Messsage );

void StandardError( const char *Format, ... );

