//--------------------------------------------------------------------------------------------------------------------------------
//  staticlist.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

template< class Type, int64 NUM >
class TStaticList
{
    MAKE_DEFAULT_COPYABLE( TStaticList );
    
public:
    constexpr TStaticList() = default;
    constexpr TStaticList( std::initializer_list< Type > values );
    constexpr TStaticList( const Type &value );
    
    ~TStaticList() = default;
    
    constexpr int64 GetSize() const;
    constexpr int64 GetNum() const;
    
    constexpr int64 GetBytes() const;
    
    constexpr const Type *GetPtr() const;
    Type *                GetPtr();
    constexpr const Type *GetEnd() const;
    Type *                GetEnd();
    
    template< class Sort = TSort< Type > >
    void Sort( const Sort &sort = Sort() )
    {
        return QuickSort( ptr, GetEnd(), sort );
    }
    
    TView< Type > Slice( int64 offset, int64 num ) const;
    TSpan< Type > Slice( int64 offset, int64 num );
    
    int64       FindIndex( const Type &value ) const;
    const Type *Find( const Type &value ) const;
    Type *      Find( const Type &value );
    
    constexpr bool operator==( const TStaticList &other ) const;
    constexpr bool operator!=( const TStaticList &other ) const;
    
    constexpr const Type &operator[]( int64 index ) const;
    constexpr Type &      operator[]( int64 index );
    
private:
    Type ptr[ NUM ];
};

//--------------------------------------------------------------------------------------------------------------------------------
template< class Type, int64 NUM >
constexpr TStaticList< Type, NUM >::TStaticList( std::initializer_list< Type > values )
{
    ASSERT( values.size() == NUM );
    
	for ( int64 i = 0; i < NUM; i++ ) {
		ptr[ i ] = values[ i ];
	}
}

template< class Type, int64 NUM >
constexpr TStaticList< Type, NUM >::TStaticList( const Type &value )
{
	for ( int64 i = 0; i < NUM; i++ ) {
		ptr[ i ] = value;
	}
}

template< class Type, int64 NUM >
constexpr int64 TStaticList< Type, NUM >::GetSize() const
{
    return NUM;
}

template< class Type, int64 NUM >
constexpr int64 TStaticList< Type, NUM >::GetNum() const
{
    return NUM;
}

template< class Type, int64 NUM >
constexpr int64 TStaticList< Type, NUM >::GetBytes() const
{
    return NUM * sizeof( Type );
}

template< class Type, int64 NUM >
constexpr const Type *TStaticList< Type, NUM >::GetPtr() const
{
    return ptr;
}

template< class Type, int64 NUM >
Type *TStaticList< Type, NUM >::GetPtr()
{
    return ptr;
}

template< class Type, int64 NUM >
constexpr const Type *TStaticList< Type, NUM >::GetEnd() const
{
    return ptr + NUM;
}

template< class Type, int64 NUM >
Type *TStaticList< Type, NUM >::GetEnd()
{
    return ptr + NUM;
}

template< class Type, int64 NUM >
TView< Type > TStaticList< Type, NUM >::Slice( int64 offset, int64 num ) const
{
    ASSERT( ( ptr + offset + num ) >= ptr );
    ASSERT( ( offset + num ) <= NUM );
    
    return TView< Type >( ptr + offset, num );
}

template< class Type, int64 NUM >
TSpan< Type > TStaticList< Type, NUM >::Slice( int64 offset, int64 num )
{
    ASSERT( ( ptr + offset + num ) >= ptr );
    ASSERT( ( offset + num ) <= NUM );
    
    return TSpan< Type >( ptr + offset, num );
}

template< class Type, int64 NUM >
int64 TStaticList< Type, NUM >::FindIndex( const Type &value ) const
{
    for ( int64 i = 0; i < NUM; i++ ) {
        if ( ptr[ i ] == value ) {
            return i;
        }
    }
    
    return INVALID_INT64;
}

template< class Type, int64 NUM >
const Type *TStaticList< Type, NUM >::Find( const Type &value ) const
{
    const int64 index = FindIndex( value );
    return index != INVALID_INT64 ? ptr + index : nullptr;
}

template< class Type, int64 NUM >
Type *TStaticList< Type, NUM >::Find( const Type &value )
{
    const int64 index = FindIndex( value );
    return index != INVALID_INT64 ? ptr + index : nullptr;
}

template< class Type, int64 NUM >
constexpr bool TStaticList< Type, NUM >::operator==( const TStaticList &other ) const
{
    for ( int64 i = 0; i < NUM; i++ ) {
        if ( ptr[ i ] != other.ptr[ i ] ) {
            return false;
        }
    }
    
    return true;
}

template< class Type, int64 NUM >
constexpr bool TStaticList< Type, NUM >::operator!=( const TStaticList &other ) const
{
    return !operator==( other );
}

template< class Type, int64 NUM >
constexpr const Type &TStaticList< Type, NUM >::operator[]( int64 index ) const
{
    ASSERT( index >= 0 );
    ASSERT( index < NUM );
    
    return ptr[ index ];
}

template< class Type, int64 NUM >
constexpr Type &TStaticList< Type, NUM >::operator[]( int64 index )
{
    ASSERT( index >= 0 );
    ASSERT( index < NUM );
    
    return ptr[ index ];
}
