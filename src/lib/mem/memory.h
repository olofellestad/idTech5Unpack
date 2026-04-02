//--------------------------------------------------------------------------------------------------------------------------------
//  memory.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

#include <cstring>
#include <new>

// clang-format off
#ifdef DEBUG_MEMORY
NODISCARD void *Memory_Malloc( int64 size, const char *fileName, int fileLine );
void Memory_Free( void *ptr );

template< class Type, class... Args >
inline Type *Memory_Place( Type *ptr, Args&&... args )
{
    return new ( ptr ) Type( std::forward< Args >( args )... );
}

int64 Memory_GetCurrentMemory();
int64 Memory_GetMaximumMemory();
int64 Memory_GetCurrentAllocs();
int64 Memory_GetMaximumAllocs();
void Memory_CheckLeaks();

NODISCARD inline void *operator new( std::size_t size, const char *fileName, int fileLine )
{ return Memory_Malloc( (int64)size, fileName, fileLine ); }
NODISCARD inline void *operator new[]( std::size_t size, const char *fileName, int fileLine )
{ return Memory_Malloc( (int64)size, fileName, fileLine ); }
inline void operator delete( void *ptr ) noexcept { Memory_Free( ptr ); }
inline void operator delete[]( void *ptr ) noexcept { Memory_Free( ptr ); }

#   define Memory_Malloc( size )    Memory_Malloc( size, FILENAME, FILELINE )
#   define new                      new( FILENAME, FILELINE )
#else
NODISCARD inline void *Memory_Malloc( int64 size ) { return System_Malloc( size, 16 ); }
inline void Memory_Free( void *ptr ) { System_Free( ptr ); }

template< class Type, class... Args >
inline Type *Memory_Place( Type *ptr, Args&&... args )
{
    return new ( ptr ) Type( std::forward< Args >( args )... );
}

#define Memory_GetCurrentMemory()   0ll
#define Memory_GetMaximumMemory()   0ll
#define Memory_GetCurrentAllocs()   0ll
#define Memory_GetMaximumAllocs()   0ll
#define Memory_CheckLeaks()         DISCARD()

NODISCARD inline void *operator new( std::size_t size ) { return Memory_Malloc( (int64)size ); }
NODISCARD inline void *operator new[]( std::size_t size ) { return Memory_Malloc( (int64)size ); }
inline void operator delete( void *ptr ) noexcept { Memory_Free( ptr ); }
inline void operator delete[]( void *ptr ) noexcept { Memory_Free( ptr ); }
#endif
// clang-format on

template< class Type >
inline int Memory_Compare( const Type *dst, const Type *src, int64 num )
{
    if constexpr ( std::is_trivially_copyable_v< Type > ) {
        return std::memcmp( dst, src, num * sizeof( Type ) );
    }
    else {
        for ( int64 i = 0; i < num; i++ ) {
            if ( dst[ i ] < src[ i ] ) {
                return -1;
            }
            if ( dst[ i ] > src[ i ] ) {
                return +1;
            }
        }
        
        return 0;
    }
}

// FIXME: remove these and replace with placement new
template< class Type >
inline void Memory_Copy( Type *dst, const Type *src, int64 num )
{
    if constexpr ( std::is_trivially_copyable_v< Type > ) {
        std::memcpy( dst, src, num * sizeof( Type ) );
    }
    else {
        for ( int64 i = 0; i < num; i++ ) {
            dst[ i ] = src[ i ];
        }
    }
}

template< class Type >
inline void Memory_Move( Type *dst, const Type *src, int64 num )
{
    if constexpr ( std::is_trivially_copyable_v< Type > ) {
        std::memcpy( dst, src, num * sizeof( Type ) );
    }
    else {
        for ( int64 i = 0; i < num; i++ ) {
            dst[ i ] = std::move( src[ i ] );
        }
    }
}

template< class Type >
inline void Memory_Fill( Type *dst, const Type &value, int64 num )
{
    if constexpr ( std::is_trivially_copyable_v< Type > && sizeof( Type ) == 1 ) {
        std::memset( dst, (uint8)value, num );
    }
    else {
        for ( int64 i = 0; i < num; i++ ) {
            dst[ i ] = value;
        }
    }
}
