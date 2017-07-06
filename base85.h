
/*
 * Developed by Rafa Garcia <rafagarcia77@gmail.com>
 *
 * base85.h is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * base85.h is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with base85.h.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _BASE85_H_
#define	_BASE85_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/** @defgroup base85 Base85 Converter.
  * Base85 RFC 1924 version. The character set is, in order, 0–9, A–Z, a–z, and
  * then the 23 characters !#$%&()*+-;<=>?@^_`{|}~.
  * @{ */

/** Convert a binary memory block in a base85 null-terminated string.
  * If the size of the source memory block is not a multiple of four,
  * as many zeros as necessary are added to convert it to a multiple of four.
  * @param dest Destination memory where to put the base85 null-terminated string.
  * @param src Source binary memory block.
  * @param size Size in bytes of source binary memory block.
  * @return A pointer to the null character of the base85 null-terminated string. */
char* bintob85( char* dest, void const* src, size_t size );

/** Convert a base85 string to binary format.
  * @param dest Destination memory block.
  * @param src Source base85 string.
  * @return If success a pointer to the next byte in memory block.
  *         Null if string has a bad format.  */
void* b85tobin( void* dest, char const* src );

/** Convert a base85 string to binary format.
  * @param p Source base85 string and destination memory block.
  * @return If success a pointer to the next byte in memory block.
  *         Null if string has a bad format.  */
static inline void* b85decode( void* p ) {
    return b85tobin( p, (char*)p );
}

/** @ } */

#ifdef __cplusplus
}
#endif

#endif	/* _BASE85_H_ */