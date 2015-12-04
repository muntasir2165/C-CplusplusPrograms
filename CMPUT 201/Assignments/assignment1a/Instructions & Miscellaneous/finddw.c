#include <stdio.h>
#include <string.h> /* For strncmp(), etc. */

#define MIN_BUF 256
#define MAX_BUF 2048

char Buffer[ MAX_BUF ];
char Word[ MIN_BUF ];
char NextWord[ MIN_BUF ];

void parseFile( FILE * fp, char * fname );

int main( int argc, char * argv[] )
{
        int     i;
        FILE    * fp;
        for( i = 1; i < argc; i++ )
        {
                fp = fopen( argv[ i ], "r" );
                if( fp == NULL )
                {
                        printf("Could not open file %s\n", argv[i]);
                        exit(-1);
                } 
                else 
                {
                        parseFile(fp, argv[i]);
                        fclose(fp);
                } 
        }
        return( 0 );
} /* main */

void parseFile( FILE * fp, char * fname )
{
        int     rval;
        /* Read first word */
        rval = fscanf( fp, "%s", Word );
        if( rval != 1 )
        {
                printf( "Failed to read first word\n" );
                exit( -1 );
	}
	while( ! feof( fp ) )
        {
                rval = fscanf( fp, "%s", NextWord );
                if( rval != 1 )
                        continue;
                if( strncmp( Word, NextWord, MIN_BUF ) == 0 )
                {
                        printf( "In file %s, repeated word:  %s %s\n", fname, Word, NextWord );
                
                /* Heuristic as to when to print out context info */
                
                /* First letter must be alpha-numeric */
                if( isalnum( Word[ 0 ] ) )
                {
                        fgets( Buffer, MAX_BUF, fp );
                        printf( "Context:  %s\n", Buffer );
                        printf( "Re-run program after fixing!!!\n" );
                }
        }
        strncpy( Word, NextWord, MIN_BUF );
        } /* while */
} /* parseFile */

