#include "lib/math/rand.h"

Rand::Rand( uint64 _seed )
    : seed( _seed )
{
}

// Roll w/ advantage/disadvantage
//  x: +1 = advantage
//  x: -1 = disadvantage
int Rand::Roll( int n, int s, int x )
{
    int a = Roll( n, s );
    int b = Roll( n, s );
    return ( x < 0 )
        ? MIN( a, b )
        : MAX( a, b );
}

int Rand::Roll( int n, int s )
{
    int r = 0;
    for ( int i = 0; i < n; i++ ) {
        r += RandomInt32( 0, s ) + 1;
    }
    
    return r;
}

float32 Rand::RandomFloat32( float32 min, float32 max )
{
    return ( max - min ) * ( Random() / (float32)MAX_UINT64 ) + min;
}

int32 Rand::RandomInt32( int32 min, int32 max )
{
    return Random() % ( max - min ) + min;
}

uint32 Rand::RandomUint32( uint32 min, uint32 max )
{
    return Random() % ( max - min ) + min;
}

uint64 Rand::Random()
{
    seed = seed * 6364136223846793005ull + 1;
    return seed >> 33;
}
