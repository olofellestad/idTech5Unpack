//--------------------------------------------------------------------------------------------------------------------------------
//  rand.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

class Rand
{
public:
    Rand( uint64 _seed );
    
    int Roll( int n, int s, int x );
    int Roll( int n, int s );
    
    float32 RandomFloat32( float32 min, float32 max );
    int32   RandomInt32( int32 min, int32 max );
    uint32  RandomUint32( uint32 min, uint32 max );
    
private:
    uint64 Random();
    
private:
    uint64 seed;
};
