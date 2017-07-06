

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base85.h"

static size_t bintoz85size( size_t binsz ) {
    size_t const regularSize = 5 * ( binsz / 4 );
    unsigned int const remainder = binsz % 4;
    unsigned int const extraSize = remainder? remainder + 2: 0;
    return regularSize + extraSize;
}

extern void createTable( void );
extern void tableTest( void );

int main( void ) {


    {
        char plain[] = { 0, 1, 2, 3 };
        char data[8];
        bintoz85( data, plain, sizeof plain );
        puts( data );

        char const* const end = z85decode( data );
        if( !end ) {
            fprintf( stderr, "%s", "Error. Bad base 85 format.\n" );
            return EXIT_FAILURE;
        }

        unsigned int const len = end - data;
        if( len != sizeof plain ) {
            fprintf( stderr, "%s%u%s%u%s", "Error decode. Expected length: ", sizeof plain, ", Length: ", len, ".\n" );
            return EXIT_FAILURE;
        }

        printf( "%s", "Output: ");
        for( unsigned int j = 0; j < len; ++j )
               printf("%u%c", data[j], ' ');
           printf( "%c", '\n' );

        return EXIT_SUCCESS;
    }

    char binary[9];
    for( unsigned int i = 0; i < sizeof binary; ++i )
        binary[i] = i;

    printf( "%s", "Binary: ");
    for( unsigned j = 0; j < sizeof binary; ++j )
           printf("%u%c", binary[j], ' ');
       printf( "%c", '\n' );


    char base85[24];
    for( unsigned int i = 1; i < sizeof binary; ++i ) {

        printf( "%s%u%s", "\nSize: ", i, ".\n");

        char* const z85end = bintoz85( base85, binary, i );

        unsigned int const z85len = z85end - base85;
        unsigned int const expectedLen = bintoz85size( i );
        if ( z85len != expectedLen ) {
            fprintf( stderr, "%s%u%s%u%s", "Error encode. Expected length: ", expectedLen, ", Length: ", z85len, ".\n" );
            return EXIT_FAILURE;
        }

        printf( "%s%s%s", "Base 85: \"", base85, "\"\n" );

        char* binend = z85decode( base85 );
        if( !binend ) {
            fprintf( stderr, "%s", "Error. Bad base 85 format.\n" );
            return EXIT_FAILURE;
        }

        unsigned int const binlen = binend - base85;
        if( binlen != i ) {
            fprintf( stderr, "%s%u%s%u%s", "Error decode. Expected length: ", i, ", Length: ", binlen, ".\n" );
            return EXIT_FAILURE;
        }

        bool const equal = !memcmp( binary, base85, i );
        printf( "%s", equal ? "OK: ": "Wrong: " );
        for( unsigned j = 0; j < i; ++j )
            printf("%u%c", base85[j], ' ');
        printf( "%c", '\n' );


    }
    return EXIT_SUCCESS;
}

