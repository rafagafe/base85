
/*
 * Developed by Rafa Garcia <rafagarcia77@gmail.com>
 *
 * example-02.c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * example-02.c is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with example-02.c.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <string.h>
#include "base85.h"

/* 1) A null-terminated string is stored in an array named 'plain'.
 * 2) The text in 'plain' is printed.
 * 3) Convert 'plain' in base85 format in an array named 'buffer'.
 * 4) Print 'buffer'.
 * 5) Convert 'buffer' in plain text in the same array 'buffer'.
 * 6) Check if the conversion was successful.
 * 7) Check the length of 'buffer'.
 * 8) Compare 'plain' with 'buffer'.
 * 9) Print the plain text stored in 'buffer'. */
int main( void ) {

    static char const plain[] = "This is a plain text.";
    puts( plain );

    char buffer[128];
    bintob85( buffer, plain, sizeof plain );

    printf( "%s%s%s", "The base85: '", buffer, "'.\n" );

    char* const end = b85decode( buffer );
    if ( !end ) {
        fputs( "Bad base85 format.", stderr );
        return -1;
    }

    int const equal = !strcmp( plain, buffer );
    if( !equal ) {
        fputs( "The output is different from the input.\n", stderr );
        return -1;
    }

    puts( buffer );

    return 0;
}