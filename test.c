
/*
 * Developed by Rafa Garcia <rafagarcia77@gmail.com>
 *
 * test.c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * test.h is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with test.c.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "base85.h"

#define done() return 0

#define fail() return __LINE__

static unsigned int checkqty = 0;

#define check( x ) { ++checkqty; if (!(x)) fail(); }



static int allDigits( void ) {

    static char const digits[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "!#$%&()*+-;<=>?@^_`{|}~";

    unsigned char binary[68];
    unsigned char* const end = b85tobin( binary, digits );
    check( end );

    unsigned int const binarylen = end - binary;
    unsigned int const digitslen = sizeof digits - 1;
    unsigned int const calclen   = 4 * digitslen / 5;
    check( binarylen == calclen );

    char base85[85];
    char* const nullchar = bintob85( base85, binary, binarylen );
    check( nullchar );
    check( '\0' == *nullchar );
    check( !strcmp( base85, digits ) );

    done();
}

static size_t bintob85size( size_t binsz ) {
    size_t const regularSize = 5 * ( binsz / 4 );
    unsigned int const remainder = binsz % 4;
    unsigned int const extraSize = remainder? 5: 0;
    return regularSize + extraSize;
}


static int aliasing ( void ) {

    static char const message[] = "Aliasing";

    char base85[32];
    char* const nullchar = bintob85( base85, message, sizeof message );
    check( nullchar );
    check( '\0' == *nullchar );
    unsigned int const len = nullchar - base85;
    unsigned int const expectlen = bintob85size( sizeof message );
    check( len == expectlen );

    char* end = b85decode( base85 );
    check( end );
    check( !strcmp( message, base85 ) );

    done();
}



int badformats( void ) {
    {
        static char const base85[] = { 0 };
        uint8_t binary[5];
        uint8_t* end = b85tobin( binary, base85 );
        check( end == binary );
    }
    {
        static char const base85[] = {  '0', -1, '1', '2',  '3', 0 };
        uint8_t binary[32];
        uint8_t* end = b85tobin( binary, base85 );
        check( !end );
    }
    {
        static char const base85[] = "aaaaaa";
        uint8_t binary[32];
        uint8_t* end = b85tobin( binary, base85 );
        check( !end );
    }
    done();
}

struct test {
    int(*func)(void);
    char const* name;
};

static int test_exec( struct test const* test ) {
    int const err = test->func();
    if ( err ) {
        fprintf( stderr, "%s%s%s%d%s", "Failed test: '", test->name, "' Line: ", err, ".\n" );
        return 1;
    }
    return 0;
}

static struct test const tests[] = {
    { allDigits,   "All digits"        },
    { aliasing,    "Aliasing"          },
    { badformats,  "Bad Formats"       },
};

int main( void ) {
    int failed = 0;
    unsigned int const qty = sizeof tests / sizeof *tests;
    for( unsigned int i = 0; i < qty; ++i )
        failed += test_exec( tests + i );
    unsigned int const percent = 100.0 * ( qty - failed ) / qty;
    printf( "%d%s%d%s", percent, "%. ", checkqty, " checks.\n" );
    return failed;
}
