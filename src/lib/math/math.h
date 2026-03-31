//--------------------------------------------------------------------------------------------------------------------------------
//  math.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

#include <cmath>

#define PI      3.141592653589793115997963
#define HALF_PI 1.570796326794896557998982
#define TWO_PI  6.283185307179586231995927
#define INV_PI  0.318309886183790691216444
#define SQRT2   1.414213562373095145474622
#define INF     INFINITY

template< class Type >
ALWAYS_INLINE Type ToRadians( Type degrees )
{
    return degrees * Type( 0.01745329251994329576923690768489 );
}

template< class Type >
ALWAYS_INLINE Type ToDegrees( Type radians )
{
    return radians * Type( 57.2957795130823208767981548141050 );
}

template< class Type >
ALWAYS_INLINE Type Sqrt( Type value )
{
    return std::sqrt( value );
}

template< class Type >
ALWAYS_INLINE Type InvSqrt( Type value )
{
    return Type( 1 ) / Sqrt( value );
}

template< class Type >
ALWAYS_INLINE void Sincos( Type value, Type *x, Type *y )
{
    *x = std::sin( value );
    *y = std::cos( value );
}

template< class Type >
ALWAYS_INLINE Type Cos( Type value )
{
    return std::cos( value );
}

template< class Type >
ALWAYS_INLINE Type Sin( Type value )
{
    return std::sin( value );
}

template< class Type >
ALWAYS_INLINE Type Tan( Type value )
{
    return std::tan( value );
}

template< class Type >
ALWAYS_INLINE Type Arccos( Type value )
{
    return std::acos( value );
}

template< class Type >
ALWAYS_INLINE Type Arcsin( Type value )
{
    return std::asin( value );
}

template< class Type >
ALWAYS_INLINE Type Arctan( Type value )
{
    return std::atan( value );
}

ALWAYS_INLINE float32 Fabs( float32 value )
{
    return std::fabs( value );
}

ALWAYS_INLINE float64 Fabs( float64 value )
{
    return std::fabs( value );
}
