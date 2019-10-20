
/*
<https://github.com/rafagafe/base85>

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
  Copyright (c) 2016-2018 Rafa Garcia <rafagarcia77@gmail.com>.
  Permission is hereby  granted, free of charge, to any  person obtaining a copy
  of this software and associated  documentation files (the "Software"), to deal
  in the Software  without restriction, including without  limitation the rights
  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#include <stdio.h>
#include <string.h>
#include "../base85.h"


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
    memcpy( base85, message, sizeof message );
    char* const nullchar = b85encode( base85, sizeof message );
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
