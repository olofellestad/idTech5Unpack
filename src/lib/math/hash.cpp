#include "lib/math/hash.h"

#define HASH_OFFSET 0xCBF29CE484222325ull
#define HASH_PRIME  0x100000001B3ull

// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash
uint64 Hash( const void *ptr, int64 num )
{
    const uint8 *bytes = (const uint8 *)ptr;
    
    uint64 hash = HASH_OFFSET;
    do {
        hash ^= (uint64)*bytes++;
        hash *= HASH_PRIME;
    } while ( --num > 0 );
    
    return hash;
}
