//--------------------------------------------------------------------------------------------------------------------------------
//  span.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

template< class Type >
class TSpan
{
    MAKE_DEFAULT_COPYABLE( TSpan );
    
public:
    constexpr TSpan( Type *_ptr, Type *_end );
    constexpr TSpan( Type *_ptr, int64 num );
    
    ~TSpan() = default;
    
    constexpr bool IsValid() const;
    constexpr bool IsEmpty() const;
    
    constexpr int64 GetSize() const;
    constexpr int64 GetNum() const;
    
    constexpr int64 GetBytes() const;
    
    constexpr const Type *GetPtr() const;
    Type *                GetPtr();
    constexpr const Type *GetEnd() const;
    Type *                GetEnd();
    
    TSpan< const Type > Slice( int64 offset, int64 num ) const;
    TSpan< Type > 		Slice( int64 offset, int64 num );
    
    int64       FindIndex( const Type &value ) const;
    const Type *Find( const Type &value ) const;
    Type *      Find( const Type &value );
    
    constexpr const Type &operator[]( int64 index ) const;
    constexpr Type &      operator[]( int64 index );
    
private:
    Type *ptr;
    Type *end;
};

//--------------------------------------------------------------------------------------------------------------------------------
template< class Type >
using TView = TSpan< const Type >;

template< class Type >
constexpr TSpan< Type >::TSpan( Type *_ptr, Type *_end )
    : ptr( _ptr )
    , end( _end )
{
}

template< class Type >
constexpr TSpan< Type >::TSpan( Type *_ptr, int64 num )
    : ptr( _ptr )
    , end( ptr + num )
{
}

template< class Type >
constexpr bool TSpan< Type >::IsValid() const
{
    return ptr != nullptr;
}

template< class Type >
constexpr bool TSpan< Type >::IsEmpty() const
{
    return ptr == end;
}

template< class Type >
constexpr int64 TSpan< Type >::GetSize() const
{
    return end - ptr;
}

template< class Type >
constexpr int64 TSpan< Type >::GetNum() const
{
    return end - ptr;
}

template< class Type >
constexpr int64 TSpan< Type >::GetBytes() const
{
    return GetNum() * sizeof( Type );
}

template< class Type >
constexpr const Type *TSpan< Type >::GetPtr() const
{
    return ptr;
}

template< class Type >
Type *TSpan< Type >::GetPtr()
{
    return ptr;
}

template< class Type >
constexpr const Type *TSpan< Type >::GetEnd() const
{
    return end;
}

template< class Type >
Type *TSpan< Type >::GetEnd()
{
    return end;
}

template< class Type >
TView< Type > TSpan< Type >::Slice( int64 offset, int64 num ) const
{
    ASSERT( ( ptr + offset + num ) >= ptr );
    ASSERT( ( ptr + offset + num ) <= end );
    
    return TView< Type >( ptr + offset, num );
}

template< class Type >
TSpan< Type > TSpan< Type >::Slice( int64 offset, int64 num )
{
    ASSERT( ( ptr + offset + num ) >= ptr );
    ASSERT( ( ptr + offset + num ) <= end );
    
    return TSpan< Type >( ptr + offset, num );
}

template< class Type >
int64 TSpan< Type >::FindIndex( const Type &value ) const
{
    const int64 num = GetNum();
    
    for ( int64 i = 0; i < num; i++ ) {
        if ( ptr[ i ] == value ) {
            return i;
        }
    }
    
    return INVALID_INT64;
}

template< class Type >
const Type *TSpan< Type >::Find( const Type &value ) const
{
    const int64 index = FindIndex( value );
    return index != INVALID_INT64 ? ptr + index : nullptr;
}

template< class Type >
Type *TSpan< Type >::Find( const Type &value )
{
    const int64 index = FindIndex( value );
    return index != INVALID_INT64 ? ptr + index : nullptr;
}

template< class Type >
constexpr const Type &TSpan< Type >::operator[]( int64 index ) const
{
    ASSERT( index >= 0 );
    ASSERT( index < GetNum() );
    
    return ptr[ index ];
}

template< class Type >
constexpr Type &TSpan< Type >::operator[]( int64 index )
{
    ASSERT( index >= 0 );
    ASSERT( index < GetNum() );
    
    return ptr[ index ];
}
