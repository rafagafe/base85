
/*
 * Developed by Rafa Garcia <rafagarcia77@gmail.com>
 *
 * base85.c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * base85.c is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with base85.c.  If not, see <http://www.gnu.org/licenses/>.
 *
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
static unsigned long betoul( void const* src, unsigned int sz ) {

    unsigned long value = 0;
    char* const d = (char*)&value;
    char const* const s = (char const*)src;

    for( unsigned int i = 0; i < sz; ++i )
        d[ *littleEndian ? 3 - i : i ] = s[ i ];

    for( unsigned int i = sz; i < 4; ++i )
        d[ *littleEndian ? 3 - i : i ] = 0;

    return value;
}

/* Convert a binary memory block in a base85 null-terminated string. */
char* bintob85( char* dest, void const* src, size_t size ) {

    char const* s = (char const*)src;
    unsigned int const quartets = size / 4;
    for( unsigned int i = 0; i < quartets; ++i, s += 4 ) {
        unsigned long const value = betoul( s, 4 );
        dest = ultob85( dest, value );
    }

    unsigned int const remainder = size % 4;
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

    for( unsigned int i = 0; i < 4; ++i )
        d[ i ] = s[ *littleEndian ? 3 - i : i ];

    return d + 4;
}

/* Convert a base85 string to binary format. */
void* b85tobin( void* dest, char const* src ) {

    for( unsigned char const* s = (unsigned char const*)src;; ++s ) {

        unsigned int const bin = digittobin[ *s ];
        if ( bin == notadigit ) return dest;
        unsigned long value = bin * pow85[ 0 ];

        for( unsigned int i = 1; i < sizeof pow85 / sizeof *pow85; ++i ) {
            unsigned int const bin = digittobin[ *++s ];
            if ( bin == notadigit ) return 0;
            value += bin * pow85[ i ];
        }

        dest = ultobe( dest, value );
    }
}
