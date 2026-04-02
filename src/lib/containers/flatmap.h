#pragma once

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

    constexpr const VType *GetPtr() const;
    VType *                GetPtr();
    constexpr const VType *GetEnd() const;
    VType *                GetEnd();

    void Insert( const KType &key, VType &&value );
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
	NOUNIQUE Sort  sort;
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
constexpr const VType *TFlatMap< KType, VType, Sort >::GetPtr() const
{
	return values.GetPtr();
}

template< class KType, class VType, class Sort >
VType *TFlatMap< KType, VType, Sort >::GetPtr()
{
	return values.GetPtr();
}

template< class KType, class VType, class Sort >
constexpr const VType *TFlatMap< KType, VType, Sort >::GetEnd() const
{
	return values.GetEnd();
}

template< class KType, class VType, class Sort >
VType *TFlatMap< KType, VType, Sort >::GetEnd()
{
	return values.GetEnd();
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Insert( const KType &key, VType &&value )
{
	const int64 index = LowerBound( keys.GetPtr(), keys.GetNum(), key, sort );

	if ( index < keys.GetNum() && keys[ index ] == key ) {
		values[ index ] = std::move( value );
	}
	else {
		values.Insert( index, std::move( value ) );
		keys.Insert( index, key );
	}
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Insert( const KType &key, const VType &value )
{
	const int64 index = LowerBound( keys.GetPtr(), keys.GetNum(), key, sort );

	if ( index < keys.GetNum() && keys[ index ] == key ) {
		values[ index ] = value;
	}
	else {
		values.Insert( index, value );
		keys.Insert( index, key );
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
		values.Emplace( index, std::forward< Args >( args )... );
		keys.Insert( index, key );
	}
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Remove( const KType &key )
{
	const int64 index = LowerBound( keys.GetPtr(), keys.GetNum(), key, sort );

    if ( index >= keys.GetNum() || keys[ index ] != key ) {
        return;
    }

    values.Remove( index );
    keys.Remove( index );
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Clear()
{
	values.Clear();
	keys.Clear();
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Resize( int64 num )
{
	values.Resize( num );
	keys.Resize( num );
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Reserve( int64 size )
{
	values.Reserve( size );
	keys.Reserve( size );
}

template< class KType, class VType, class Sort >
void TFlatMap< KType, VType, Sort >::Shrink()
{
	values.Shrink();
	keys.Shrink();
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

template< class KType, class VType, class Sort >
constexpr VType &TFlatMap< KType, VType, Sort >::operator[]( const KType &key )
{
	const int64 index = LowerBound( keys.GetPtr(), keys.GetNum(), key, sort );

	if ( index < keys.GetNum() && keys[ index ] == key ) {
		return values[ index ];
	}

	values.Insert( index, VType() );
	keys.Insert( index, key );

	return values[ index ];
}

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
