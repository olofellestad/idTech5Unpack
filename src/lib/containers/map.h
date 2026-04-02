//--------------------------------------------------------------------------------------------------------------------------------
//  map.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

template< class Type >
class THash
{
public:
    uint64 Hash( const Type &value )
    {
        return ::Hash( &value, sizeof( Type ) );
    }
};
#if 0
template< class KType, class VType >
class TMapEntry
{
public:
	TMapEntry()
		: state( 0 )
	{}
public:
	KType key;
	VType value;
	uint8 state;
};

// rename to hashmap
template< class KType, class VType, class Hash = THash< KType > >
class TMap
{
public:
	using EType = TMapEntry< KType, VType >;

public:
    constexpr TMap( const Hash &_hash = THash< KType >() );
    TMap( TMap &&other ) noexcept;
    TMap( const TMap &other );
    
    ~TMap();
    
    constexpr bool IsValid() const;
    constexpr bool IsEmpty() const;
    
    constexpr int64 GetSize() const;
    constexpr int64 GetNum() const;
    
    constexpr int64 GetBytes() const;
    
    constexpr const EType *GetPtr() const;
    EType *                GetPtr();
    constexpr const EType *GetEnd() const;
    EType *                GetEnd();
    
    bool HasKey( const KType &key ) const;
    bool Has( const VType &value ) const;
    
    void Insert( const KType &key, const VType &value );
    void Insert( const KType &key, VType &&value );

    template< class... Args >
    void Emplace( const KType &key, Args&&... args );

	void Remove( const KType &key );
    void Clear();
    
    void Resize( int64 num );
    void Reserve( int64 size );
    void Shrink();
    
    int64        FindIndex( const KType &key ) const;
    const VType *Find( const KType &key ) const;
    VType *      Find( const KType &key );
    
    constexpr bool operator==( const TMap &other ) const;
    constexpr bool operator!=( const TMap &other ) const;
    
    constexpr const VType &operator[]( const KType &key ) const;
    constexpr VType &      operator[]( const KType &key );
    
    TMap &operator=( TMap &&other ) noexcept;
    TMap &operator=( const TMap &other );
    
private:
    void Realloc( int64 size );
    void Realloc();
    
private:
	EType *			ptr;
	int64    		num;
	int64    		size;
	NOUNIQUE Hash	hash;
};

//--------------------------------------------------------------------------------------------------------------------------------
template< class KType, class VType, class Hash >
constexpr TMap< KType, VType, Hash >::TMap( const Hash &_hash )
	: ptr( nullptr )
	, num( 0 )
	, size( 0 )
	, hash( _hash )
{
}

template< class KType, class VType, class Hash >
TMap< KType, VType, Hash >::TMap( TMap &&other ) noexcept
{
}

template< class KType, class VType, class Hash >
TMap< KType, VType, Hash >::TMap( const TMap &other )
{
}

template< class KType, class VType, class Hash >
TMap< KType, VType, Hash >::~TMap()
{
	Clear();
}

template< class KType, class VType, class Hash >
void TMap< KType, VType, Hash >::Insert( const KType &key, const VType &value )
{
	if ( num >= ( size >> 1 ) ) {
		Realloc();
	}
	
	const uint64 h = hash.Hash( key );
	int64 i        = h & ( size - 1 );

	for ( ; ptr[ i ].state; i++ ) {
		i &= ( size - 1 );

		if ( ptr[ i ].key == key ) {
			ptr[ i ].value = value;
			return;
		}
	}

	i &= ( size - 1 );

	ptr[ i ].key   = key;
	ptr[ i ].value = value;
	ptr[ i ].state = 1;
}

template< class KType, class VType, class Hash >
void TMap< KType, VType, Hash >::Insert( const KType &key, VType &&value )
{
	if ( num >= ( size >> 1 ) ) {
		Realloc();
	}
	
	const uint64 h = hash.Hash( key );
	int64 i        = h & ( size - 1 );

	for ( ; ptr[ i ].state; i++ ) {
		i &= ( size - 1 );

		if ( ptr[ i ].key == key ) {
			ptr[ i ].value = std::move( value );
			return;
		}
	}

	i &= ( size - 1 );

	ptr[ i ].key   = key;
	ptr[ i ].value = std::move( value );
	ptr[ i ].state = 1;
}

template< class KType, class VType, class Hash >
VType *TMap< KType, VType, Hash >::Find( const KType &key )
{
	const uint64 h = hash.Hash( key );
	int64 i        = h & ( size - 1 );

	for ( ; ptr[ i ].state; i++ ) {
		i &= ( size - 1 );

		if ( ptr[ i ].key == key ) {
			return &ptr[ i ].value;
		}
	}

	return nullptr;
}

template< class KType, class VType, class Hash >
void TMap< KType, VType, Hash >::Clear()
{
	if ( ptr != nullptr ) {
        for ( int64 i = 0; i < num; i++ ) {
            ptr[ i ].~EType();
        }

        Memory_Free( ptr );
    }
    
    ptr  = nullptr;
    num  = 0;
	size = 0;
}

template< class KType, class VType, class Hash >
void TMap< KType, VType, Hash >::Resize( int64 _num )
{
    ASSERT( _num >= 0 );

    if ( _num == num ) {
        return;
    } else if ( _num == 0) {
        Clear();
    } else {
       Realloc( _num );
       num = _num;
    }
}

template< class KType, class VType, class Hash >
void TMap< KType, VType, Hash >::Reserve( int64 _size )
{
    ASSERT( _size > 0 );
    
    if ( _size == size ) {
        return;
    }
    
    Realloc( _size );
}

template< class KType, class VType, class Hash >
void TMap< KType, VType, Hash >::Realloc( int64 _size )
{
    ASSERT( _size > 0 );
    
    EType *nptr = (EType *)Memory_Malloc( _size * sizeof( EType ) );
    
    int64 _num = MIN( num, _size );

    if ( ptr != nullptr ) {
        Memory_Copy( nptr, ptr, _num );
    }
        
    Clear();
        
    num  = _num;
    ptr  = nptr;
    size = _size;
}

template< class KType, class VType, class Hash >
void TMap< KType, VType, Hash >::Realloc()
{
    if ( ptr == nullptr ) {
        ptr  = (EType *)Memory_Malloc( 16 * sizeof( EType ) );
		num  = 0;
		size = 16;
    }
    else {
        Realloc( size * 2 );
    }
}
#endif
