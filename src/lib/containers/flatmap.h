#pragma once

// TODO: hash?
template< class KType, class VType, class Sort = TSort< KType > >
class TFlatMap
{
public:
	constexpr TFlatMap( const Sort &_sort = TSort< KType >() );
	TFlatMap( TFlatMap &&other ) noexcept = default;
	TFlatMap( const TFlatMap &other ) = default;

	~TFlatMap() = default;

    constexpr bool IsValid() const;
    constexpr bool IsEmpty() const;
    
    constexpr int64 GetSize() const;
    constexpr int64 GetNum() const;
    
    constexpr int64 GetBytes() const;

    void Insert( const KType &key, const VType &value );

    template< class... Args >
    void Emplace( const KType &key, Args&&... args );

	void Remove( const KType &key );
    void Clear();
    
    void Resize( int64 num );
    void Reserve( int64 size );
    void Shrink();
    
    int64        FindIndex( const KType &value ) const;
    const VType *Find( const KType &value ) const;
    VType *      Find( const KType &value );
    
    constexpr bool operator==( const TFlatMap &other ) const;
    constexpr bool operator!=( const TFlatMap &other ) const;
    
    constexpr const VType &operator[]( const KType &key ) const;
    constexpr VType &      operator[]( const KType &key );
    
	constexpr const VType &operator[]( int64 index ) const;
    constexpr VType &      operator[]( int64 index );

private:
	TList< VType > values;
	TList< KType > keys;
	Sort		   sort;
};

//--------------------------------------------------------------------------------------------------------------------------------
template< class KType, class VType, class Sort >
constexpr TFlatMap< KType, VType, Sort >::TFlatMap( const Sort &_sort )
	: sort( _sort )
{
}

template< class KType, class VType, class Sort >
constexpr bool TFlatMap< KType, VType, Sort >::IsValid() const
{
	return keys.IsValid() && values.IsValid();
}

template< class KType, class VType, class Sort >
constexpr bool TFlatMap< KType, VType, Sort >::IsEmpty() const
{
	return keys.IsEmpty() && values.IsEmpty();
}

template< class KType, class VType, class Sort >
constexpr int64 TFlatMap< KType, VType, Sort >::GetSize() const
{
	ASSERT( keys.GetSize() == values.GetSize() );

	return keys.GetSize();
}

template< class KType, class VType, class Sort >
constexpr int64 TFlatMap< KType, VType, Sort >::GetNum() const
{
	ASSERT( keys.GetNum() == values.GetNum() );

	return keys.GetNum();
}

template< class KType, class VType, class Sort >
constexpr int64 TFlatMap< KType, VType, Sort >::GetBytes() const
{
	return keys.GetBytes() + values.GetBytes();
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Insert( const KType &key, const VType &value )
{
	const int64 index = LowerBound( keys.GetPtr(), keys.GetNum(), key, sort );

	if ( index < keys.GetNum() && keys[ index ] == key ) {
		values[ index ] = value;
	}
	else {
		keys.Insert( index, key );
		values.Insert( index, value );
	}
}

template< class KType, class VType, class Sort >
template< class... Args >
void TFlatMap< KType, VType, Sort >::Emplace( const KType &key, Args&&... args )
{
	const int64 index = LowerBound( keys.GetPtr(), keys.GetNum(), key, sort );

	if ( index < keys.GetNum() && keys[ index ] == key ) {
		values[ index ] = VType( std::forward< Args >( args )... );
	}
	else {
		keys.Insert( index, key );
		values.Emplace( index, std::forward< Args >( args )... );
	}
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Remove( const KType &key )
{
	const int64 index = LowerBound( keys.GetPtr(), keys.GetNum(), key, sort );

    if ( index >= keys.GetNum() || keys[ index ] != key ) {
        return;
    }

    keys.Remove( index );
    values.Remove( index );
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Clear()
{
	keys.Clear();
	values.Clear();
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Resize( int64 num )
{
	keys.Resize( num );
	values.Resize( num );
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Reserve( int64 size )
{
	keys.Reserve( size );
	values.Reserve( size );
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Shrink()
{
	keys.Shrink();
	values.Shrink();
}

template< class KType, class VType, class Sort >
int64 TFlatMap< KType, VType, Sort >::FindIndex( const KType &key ) const
{
	const int64 index = LowerBound( keys.GetPtr(), keys.GetNum(), key, sort );

	if ( index < keys.GetNum() && keys[ index ] == key ) {
		return index;
	}

	return INVALID_INT64;
}

template< class KType, class VType, class Sort >
const VType *TFlatMap< KType, VType, Sort >::Find( const KType &key ) const
{
    const int64 index = FindIndex( key );
    return ( index != INVALID_INT64 ) ? &values[ index ] : nullptr;
}

template< class KType, class VType, class Sort >
VType *TFlatMap< KType, VType, Sort >::Find( const KType &key )
{
    const int64 index = FindIndex( key );
    return ( index != INVALID_INT64 ) ? &values[ index ] : nullptr;
}

template< class KType, class VType, class Sort >
constexpr const VType &TFlatMap< KType, VType, Sort >::operator[]( const KType &key ) const
{
    const int64 index = FindIndex( key );
    return values[ index ]; 
}
/* TODO: need to insert value if not in map...
template< class KType, class VType, class Sort >
constexpr VType &TFlatMap< KType, VType, Sort >::operator[]( const KType &key )
{
	ASSERT( 0 && "not implemented" );
}
*/
template< class KType, class VType, class Sort >
constexpr const VType &TFlatMap< KType, VType, Sort >::operator[]( int64 index ) const
{
    return values[ index ];
}

template< class KType, class VType, class Sort >
constexpr VType &TFlatMap< KType, VType, Sort >::operator[]( int64 index )
{
    return values[ index ];
}
