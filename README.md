# Base85
Base85 is a C library to convert from plain to base 85 and vice versa suitable for embedded systems. It uses the character set of the Base85 RFC 1924.


# API
To convert from plain to base 85 the following function is used:
```C
char* bintob85( char* dest, void const* src, size_t size );
```
* The parameter 'src' is the source memory block to be converted.
* The parameter 'size' is the size in bytes of the source memory block.
* The parameter 'dest' is the destination buffer where the translation to base 85 is stored. Also a null character is added at the end to become a null-terminated string.
* The return value is a pointer to the null character.

To convert from base 85 to plain the following function is used:
```C
void* b85tobin( void* dest, char const* src );
```
* The parameter 'src' is the source base 85 string. It must be terminated in a non base 85 digit. A null-terminated string is OK.
* The parameter 'dest' is the destination memory block where the result of convertion is stroed.
* The return value is a null pointer if the base 85 string has bad format. If the convertion is successfully the return value is a pointer to the end of the convertion. With this pointer you can get the length of the result.

Convert from base 85 to plain in the same memory block is possible using the previous function by setting the same pointer for the destination and source. Or just use the following wrapper function:
```C
static inline void* b85decode( void* p ) {
    return b85tobin( p, (char*)p );
}
```
