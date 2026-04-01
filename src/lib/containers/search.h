//--------------------------------------------------------------------------------------------------------------------------------
//  search.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

template< class Type, class Sort >
int64 LowerBound( const Type *ptr, int64 num, const Type &value, const Sort &sort )
{
    // ASSERT( ptr != nullptr );
	
	// allows inserting to empty flatmap
	// but this is maybe not so good...
	if ( ptr == nullptr ) {
		return 0;
	}
    
    int64 r = num;
    int64 l = 0;
    
    while ( l < r ) {
        int64 m = l + ( ( r - l ) >> 1 );

        if ( sort.Compare( ptr[ m ], value ) < 0 ) {
            l = m + 1;
        }
        else {
            r = m;
        }
    }
    
	return l;
}

template< class Type, class Sort >
int64 BinarySearch( const Type *ptr, int64 num, const Type &value, const Sort &sort )
{
    ASSERT( ptr != nullptr );
    
    int64 r = num - 1;
    int64 l = 0;
    
    while ( l < r ) {
        int64 m = ( l + r ) >> 1;

        if ( sort.Compare( ptr[ m ], value ) < 0 ) {
            l = m + 1;
        }
        else {
            r = m;
        }
    }
    
    if ( ptr[ l ] == value ) {
        return l;
    }
    
    return INVALID_INT64;
}
