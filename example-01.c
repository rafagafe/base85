
/*
 * Developed by Rafa Garcia <rafagarcia77@gmail.com>
 *
 * example-01.c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * example-01.c is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with example-01.c.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base85.h"

/* 1) Fill an array named 'binary'.
 * 2) Convert 'binary' in base85 format in an array named 'base85'.
 * 3) Print 'base85'.
 * 4) Convert 'base85' in binary format in an array named 'output'.
 * 5) Check if the conversion was successful.
 * 6) Check the length of 'output'.
 * 7) Compare 'binary' with 'output'. */
int main( void ) {

    uint8_t binary[64];
    for( unsigned int i = 0; i < 64; ++i )
        binary[i] = i;

    char base85[128];
    bintob85( base85, binary, sizeof binary );

    printf( "%s%s%s", "The base85: '", base85, "'.\n" );

    uint8_t output[64];
    uint8_t* const end = b85tobin( output, base85 );
    if ( !end ) {
        fputs( "Bad base85 format.", stderr );
        return EXIT_FAILURE;
    }

    unsigned int const outputlen = end - output;
    if ( outputlen != sizeof binary ) {
        fputs( "The length of the output is not as expected.\n", stderr );
        return EXIT_FAILURE;
    }

    bool const equal = !memcmp( binary, output, sizeof binary );
    if( !equal ) {
        fputs( "The output is different from the input.\n", stderr );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}