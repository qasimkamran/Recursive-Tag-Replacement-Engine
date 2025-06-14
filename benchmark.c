#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "util_lib.h"

char* GetDiagnosticString_AppendToBuffer( long N, int L )
{
    char* Text = malloc( L + 1 );
    memset( Text, 'x', L );
    Text[L] = '\0';

    struct timespec T0, T1;
    clock_gettime( CLOCK_MONOTONIC, &T0 );

    Buffer Buf;
    BufferInit( &Buf );
    for( long Index = 0; Index < N; Index++ )
    {
        AppendToBuffer( &Buf, Text );
    }

    clock_gettime( CLOCK_MONOTONIC, &T1 );
    double Elapsed = ( T1.tv_sec - T0.tv_sec ) + ( T1.tv_nsec - T0.tv_nsec ) / 1e9;

    char* OutputString = AllocateBuffer( BUFFER_SIZE + 1 );
    snprintf( OutputString, BUFFER_SIZE + 1, "%ld,%d,%.6f\n", N, L, Elapsed );

    BufferFree( &Buf );
    free( Text );

    return OutputString;
}

int BENCHMARK_AppendToBuffer( long N_Max, int L_Max )
{
    FILE *OutputFile = fopen( "benchmarks/BENCHMARK_AppendToBuffer.csv", "w" );

    if( !OutputFile )
    {
        StandardError( "Failed to create or open benchmarks/%s.csv\n", __FUNCTION__ );
        return 0;
    }

    fprintf( OutputFile, "N,L,Time\n" );

    printf( "%s...\n", __FUNCTION__ );

    double Fraction = 0.0;
    for( int Index = 1; Index <= L_Max; Index++ )
    {
        for( int SubIndex = 1; SubIndex <= N_Max; SubIndex+=100 )
        {
            const char* DiagnosticString = GetDiagnosticString_AppendToBuffer( SubIndex, Index );
            if( !DiagnosticString )
            {
                PrintError( "Failed the benchmark run" );
                return 0;
            }
            fprintf( OutputFile, DiagnosticString );

            Fraction++;

            PrintProgressBar( ( Fraction / ( N_Max * L_Max ) ) * 100 );
        }
    }
    printf("\n");

    fclose( OutputFile );

    return 1;
}

int main()
{

    int Result = BENCHMARK_AppendToBuffer( 100000, 100 );
    printf( "Result = %d\n", Result );

    return 0;
}
