//--------------------------------------------------------------------------------------------------------------------------------
//  search.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

template< class Type >
int64 BinarySearch( const Type *ptr, int64 num, const Type &value )
{
    ASSERT( ptr != nullptr );
    
    if ( num <= 0 ) {
        return INVALID_INT64;
    }
    
    int64 r = num - 1;
    int64 l = 0;
    
    do {
        int64 m = ( l + r ) >> 1;
        if ( ptr[ m ] < value ) {
            l = m + 1;
        }
        else {
            r = m;
        }
    } while ( l < r );
    
    if ( ptr[ l ] == value ) {
        return l;
    }
    
    return INVALID_INT64;
}
