
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
#include "base85.h"


// ----------------------------------------------------- Test "framework": ---

#define done() return 0
#define fail() return __LINE__
static int checkqty = 0;
#define check( x ) do { ++checkqty; if (!(x)) fail(); } while ( 0 )

struct test {
    int(*func)(void);
    char const* name;
};

static int test_suit( struct test const* tests, int numtests ) {
    printf( "%s", "\n\nTests:\n" );
    int failed = 0;
    for( int i = 0; i < numtests; ++i ) {
        printf( " %02d%s%-25s ", i, ": ", tests[i].name );
        int linerr = tests[i].func();
        if ( 0 == linerr )
            printf( "%s", "OK\n" );
        else {
            printf( "%s%d\n", "Failed, line: ", linerr );
            ++failed;
        }
    }
    printf( "\n%s%d\n", "Total checks: ", checkqty );
    printf( "%s[ %d / %d ]\r\n\n\n", "Tests PASS: ", numtests - failed, numtests );
    return failed;
}


// ----------------------------------------------------------- Unit tests: ---

static int allDigits( void ) {

    static char const digits[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "!#$%&()*+-;<=>?@^_`{|}~";

    char binary[68];
    char* const end = b85tobin( binary, digits );
    check( end );

    int const binarylen = end - binary;
    int const digitslen = sizeof digits - 1;
    int const calclen   = 4 * digitslen / 5;
    check( binarylen == calclen );

    char base85[85];
    char* const nullchar = bintob85( base85, binary, binarylen );
    check( nullchar );
    check( '\0' == *nullchar );
    check( !strcmp( base85, digits ) );

    done();
}

static int bintob85size( int binsz ) {
    int const regularSize = 5 * ( binsz / 4 );
    int const remainder = binsz % 4;
    int const extraSize = remainder? 5: 0;
    return regularSize + extraSize;
}

static int aliasing ( void ) {

    static char const message[] = "Aliasing";

    char base85[32];
    char* const nullchar = bintob85( base85, message, sizeof message );
    check( nullchar );
    check( '\0' == *nullchar );
    int const len = nullchar - base85;
    int const expectlen = bintob85size( sizeof message );
    check( len == expectlen );

    char* end = b85decode( base85 );
    check( end );
    check( !strcmp( message, base85 ) );

    done();
}

int badformats( void ) {
    {
        static char const base85[] = { 0 };
        char binary[5];
        char* end = b85tobin( binary, base85 );
        check( end == binary );
    }
    {
        static char const base85[] = {  '0', -1, '1', '2',  '3', 0 };
        char binary[32];
        char* end = b85tobin( binary, base85 );
        check( !end );
    }
    {
        static char const base85[] = "aaaaaa";
        char binary[32];
        char* end = b85tobin( binary, base85 );
        check( !end );
    }
    done();
}


// --------------------------------------------------------- Execute tests: ---

int main( void ) {
    static struct test const tests[] = {
        { allDigits,   "All digits"        },
        { aliasing,    "Aliasing"          },
        { badformats,  "Bad Formats"       }
    };
    return test_suit( tests, sizeof tests / sizeof *tests );
}


