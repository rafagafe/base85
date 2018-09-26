
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

#include "base85.h"

/** Lookup table to convert a binary number in a base 85 digit. */
static char const bintodigit[] = {

    '0','1','2','3','4','5','6','7','8','9',

    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',

    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z',

    '!','#','$','%','&','(',')','*','+','-',';',
    '<','=','>','?','@','^','_','`','{','|','}','~',
};

/** Escape values. */
enum escape_e {
    notadigit = 85u /**< Return value when a non-digit-base-85 is found. */
};

/** Lookup table to convert a base 85 digit in a binary number. */
static unsigned char const digittobin[] = {
    85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
    85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
    85, 62, 85, 63, 64, 65, 66, 85, 67, 68, 69, 70, 85, 71, 85, 85,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 85, 72, 73, 74, 75, 76,
    77, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 85, 85, 85, 78, 79,
    80, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 81, 82, 83, 84, 85,
    85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
    85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
    85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
    85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
    85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
    85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
    85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
    85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
};

/** Some powers of 85. */
enum p855_e {
    p850 = 1ul,         /*< 85^0 */
    p851 = 85ul,        /*< 85^1 */
    p852 = p851 * p851, /*< 85^2 */
    p853 = p851 * p852, /*< 85^3 */
    p854 = p851 * p853, /*< 85^4 */
};

/** Powers of 85 list. */
static unsigned long const pow85[] = { p854, p853, p852, p851, p850 };

/** Converts a integer of 4 bytes in 5 digits of base 85.
  * @param dest Memory block where to put the 5 digits of base 85.
  * @param value Value of the integer of 4 bytes.
  * @return dest + 5. */
static char* ultob85( char* dest, unsigned int long value ) {

    unsigned int const digitsQty = sizeof pow85 / sizeof *pow85;

    for( unsigned int i = 0; i < digitsQty; ++i ) {
        unsigned int const bin = value / pow85[ i ];
        dest[ i ] = bintodigit[ bin ];
        value -= bin * pow85[ i ];
    }

    return dest + digitsQty;
}

/** Helper constant to get the endianness of the running machine. */
static short const endianness = 1;

/** Points to 1 if little-endian or points to 0 if big-endian. */
static char const* const littleEndian = (char const*)&endianness;

/** Convert a big-endian array of  bytes in a unsigned long.
  * @param src Pointer to array of bytes.
  * @param sz Size in bytes of array from 0 until 4.
  * @return  The unsigned long value. */
static unsigned long betoul( void const* src, int sz ) {

    unsigned long value = 0;
    char* const d = (char*)&value;
    char const* const s = (char const*)src;

    for( int i = 0; i < sz; ++i )
        d[ *littleEndian ? 3 - i : i ] = s[ i ];

    for( int i = sz; i < 4; ++i )
        d[ *littleEndian ? 3 - i : i ] = 0;

    return value;
}

/* Convert a binary memory block in a base85 null-terminated string. */
char* bintob85( char* dest, void const* src, size_t size ) {

    char const* s = (char const*)src;
    size_t const quartets = size / 4;
    for( size_t i = 0; i < quartets; ++i, s += 4 ) {
        unsigned long const value = betoul( s, 4 );
        dest = ultob85( dest, value );
    }

    int const remainder = size % 4;
    if ( remainder ) {
        unsigned long const value = betoul( s, remainder );
        dest = ultob85( dest, value );
    }

    *dest = '\0';
    return dest;
}

/** Copy a unsigned long in a big-endian array of 4 bytes.
  * @param dest Destination memory block.
  * @param value Value to copy.
  * @return  dest + 4 */
static void* ultobe( void* dest, unsigned long value ) {

    char* const d = (char*)dest;
    char const* const s = (char*)&value;

    for( int i = 0; i < 4; ++i )
        d[ i ] = s[ *littleEndian ? 3 - i : i ];

    return d + 4;
}

/* Convert a base85 string to binary format. */
void* b85tobin( void* dest, char const* src ) {

    for( unsigned char const* s = (unsigned char const*)src;; ) {

        unsigned long value = 0;
        for( int i = 0; i < sizeof pow85 / sizeof *pow85; ++i, ++s ) {
            unsigned int const bin = digittobin[ *s ];
            if ( bin == notadigit ) return i == 0 ? dest : 0;
            value += bin * pow85[ i ];
        }

        dest = ultobe( dest, value );
    }
}
