//--------------------------------------------------------------------------------------------------------------------------------
//  sort.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

template< class Type >
class TSort
{
public:
    int Compare( const Type &a, const Type &b ) const
    {
        return ( a < b ) ? -1 : ( a > b ) ? +1 : 0;
    }
};

template< class Type >
class TSort_Reverse
{
public:
    int Compare( const Type &a, const Type &b ) const
    {
        return ( a < b ) ? +1 : ( a > b ) ? -1 : 0;
    }
};

template< class Type, class Sort >
void QuickSort_r( Type *ptr, Type *end, const Sort &sort )
{
    ASSERT( ptr != nullptr );
    ASSERT( end != nullptr );
    
    if ( ( end - ptr ) < 2 ) {
        return;
    }
    
    // partition
    Type pivot = *( ptr + ( end - ptr ) / 2 );

    Type *i = ptr;
    Type *j = end - 1;
    
    do {
        while ( i < end && sort.Compare( *i, pivot ) < 0 ) ++i;
        while ( j > ptr && sort.Compare( *j, pivot ) > 0 ) --j;
        if ( i > j ) break;

        std::swap( *i, *j );
    } while ( ++i <= --j );

    QuickSort_r( ptr, j + 1, sort );
    QuickSort_r( i, end, sort );
}

template< class Type, class Sort >
ALWAYS_INLINE void QuickSort( Type *ptr, Type *end, const Sort &sort )
{
    QuickSort_r( ptr, end, sort );
}
