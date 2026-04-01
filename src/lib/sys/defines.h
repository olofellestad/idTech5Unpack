//--------------------------------------------------------------------------------------------------------------------------------
//  defines.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <cfloat>

#if defined( _WIN32 ) || defined( _WIN64 )
#    define SYSTEM_WIN32
#elif defined( __APPLE__ )
#    define SYSTEM_MACOS
#else
#    error Unsupported system!
#endif

#if defined( __clang__ )
#    define COMPILER_CLANG ( __clang_major__ * 10 + __clang_minor__ )
#elif defined( _MSC_VER )
#    define COMPILER_MSVC ( _MSC_VER )
#else
#    error Unsupported compiler!
#endif

#if INTPTR_MAX != INT64_MAX
#    error Unsupported architecture!
#endif

#ifdef COMPILER_MSVC
#   define BYTE_ORDER_LE
#else
#   if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#       define BYTE_ORDER_LE
#   else
#       error Unsupported byte order!
#   endif
#endif

#ifdef NDEBUG
#	define BUILD_RELEASE
#else
#	define BUILD_DEBUG
#endif

#ifdef BUILD_DEBUG
#    define ASSERT( x )                                                                                                \
        ( UNLIKELY( !( x ) ) ? System_Fail( va( "%s:%i:\nAssertion \"%s\" failed", FILENAME, FILELINE, STR( x ) ) )    \
                             : UNUSED( x ) )
#else
#    define ASSERT( x ) UNUSED( x )
#endif

#define STR_HELPER( x ) #x
#define STR( x )        STR_HELPER( x )

#define CONCAT_HELPER( a, b ) a##b
#define CONCAT( a, b )        CONCAT_HELPER( a, b )

// hack to make filename relative to src directory
#define FILENAME ( &__FILE__[ sizeof( SOURCE_DIR ) ] )
#define FILELINE __LINE__

#define UNUSED( x ) ( (void)( x ) )

#ifdef COMPILER_MSVC
#    define DISCARD( ... ) __noop
#else
#    define DISCARD( ... ) UNUSED( 0 )
#endif

#ifdef COMPILER_MSVC
#    define UNREACHABLE() __assume( 0 )
#else
#    define UNREACHABLE() __builtin_assume( 0 )
#endif

#ifdef COMPILER_MSVC
#    define UNLIKELY( x ) ( x )
#    define LIKELY( x )   ( x )
#else
#    define UNLIKELY( x ) __builtin_expect( !!( x ), 0 )
#    define LIKELY( x )   __builtin_expect( !!( x ), 1 )
#endif

#ifdef COMPILER_MSVC
#   define ALWAYS_INLINE __forceinline
#else
#   define ALWAYS_INLINE inline
#endif

#define NOUNIQUE  [[no_unique_address]]
#define NODISCARD [[nodiscard]]

#define MAKE_DEFAULT_COPYABLE( x )                      \
    public:                                             \
    x( const x &other )            = default;           \
    x &operator=( const x &other ) = default;
#define MAKE_DEFAULT_MOVABLE( x )                       \
    public:                                             \
    x( x &&other )            = default;                \
    x &operator=( x &&other ) = default;
#define MAKE_NONCOPYABLE( x )                           \
    public:                                             \
    x( const x &other )            = delete;            \
    x &operator=( const x &other ) = delete;
#define MAKE_NONMOVABLE( x )                            \
    public:                                             \
    x( x &&other )            = delete;                 \
    x &operator=( x &&other ) = delete;

#define HAS_BIT( a, b ) ( !!( ( a ) & ( b ) ) )
#define BIT( x )        ( 1 << ( x ) )
#define MAX( a, b )     ( ( a ) > ( b ) ? ( a ) : ( b ) )
#define MIN( a, b )     ( ( a ) < ( b ) ? ( a ) : ( b ) )
#define ABS( x )        ( ( x ) < 0 ? -( x ) : ( x ) )
#define SIGN( x )       ( ( x ) < 0 ? -1 : 1 )
#define SQR( x )        ( ( x ) * ( x ) )

#define ALIGN( a, b )   ( ( a + b - 1 ) & ( ~( b - 1 ) ) )

#define INVALID_INT64       ( (int64)-1 )
#define INVALID_HANDLE64    ( (handle64)-1 )

#define MIN_INT64 INT64_MIN
#define MAX_INT64 INT64_MAX
#define MIN_INT32 INT32_MIN
#define MAX_INT32 INT32_MAX
#define MIN_INT16 INT16_MIN
#define MAX_INT16 INT16_MAX
#define MIN_INT8  INT8_MIN
#define MAX_INT8  INT8_MAX

#define MAX_UINT64 UINT64_MAX
#define MAX_UINT32 UINT32_MAX
#define MAX_UINT16 UINT16_MAX
#define MAX_UINT8  UINT8_MAX

#define MIN_FLOAT64 DBL_MIN
#define MAX_FLOAT64 DBL_MAX
#define MIN_FLOAT32 FLT_MIN
#define MAX_FLOAT32 FLT_MAX

#define MIN_TIME64 MIN_I64
#define MAX_TIME64 MAX_I64

using uint = unsigned int;

using int64 = std::int64_t;
using int32 = std::int32_t;
using int16 = std::int16_t;
using int8  = std::int8_t;

using uint64 = std::uint64_t;
using uint32 = std::uint32_t;
using uint16 = std::uint16_t;
using uint8  = std::uint8_t;

using float64 = double;
using float32 = float;

using handle64 = void *;
using time64 = int64;
