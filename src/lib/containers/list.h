//--------------------------------------------------------------------------------------------------------------------------------
//  list.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

template< class Type >
class TList
{
public:
    constexpr TList();
    TList( std::initializer_list< Type > values );
    TList( const Type &value, int64 _num );
    TList( TList &&other ) noexcept;
    TList( const TList &other );
    
    ~TList();
    
    constexpr bool IsValid() const;
    constexpr bool IsEmpty() const;
    
    constexpr int64 GetSize() const;
    constexpr int64 GetNum() const;
    
    constexpr int64 GetBytes() const;
    
    constexpr const Type *GetPtr() const;
    Type *                GetPtr();
    constexpr const Type *GetEnd() const;
    Type *                GetEnd();
    
    void Insert( int64 index, const Type &value );
    void Insert( int64 index, Type &&value );
    void Append( const Type &value );

    template< class... Args >
    void Emplace( Args&&... args );
    
    void RemoveIndex( int64 index );
    void Remove( const Type &value );
    void Clear();
    
    void Resize( int64 _num );
    void Reserve( int64 size );
    void Shrink();
    
    template< class Sort = TSort< Type > >
    void Sort( const Sort &sort = Sort() )
    {
        return QuickSort( GetPtr(), GetEnd(), sort );
    }
    
    TSpan< Type > Slice( int64 offset, int64 _num ) const;
    
    int64         FindIndex( const Type &value ) const;
    const Type *Find( const Type &value ) const;
    Type *      Find( const Type &value );
    
    constexpr bool operator==( const TList &other ) const;
    constexpr bool operator!=( const TList &other ) const;
    
    constexpr const Type &operator[]( int64 index ) const;
    constexpr Type &      operator[]( int64 index );
    
    TList &operator=( TList &&other ) noexcept;
    TList &operator=( const TList &other );
    
private:
    void Realloc( int64 size );
    void Realloc();
    
private:
    Type *ptr;
    Type *end; // points to capacity!
    int64   num;
};

//--------------------------------------------------------------------------------------------------------------------------------
template< class Type >
constexpr TList< Type >::TList()
    : ptr( nullptr )
    , end( nullptr )
    , num( 0 )
{
}

template< class Type >
TList< Type >::TList(std::initializer_list< Type > values )
    : ptr( (Type *)Memory_Malloc( values.size() * sizeof( Type ) ) )
    , end( ptr + values.size() )
    , num( values.size() )
{
        ASSERT( ptr != nullptr );
        ASSERT( num > 0 );
        
        Memory_Copy< Type >( ptr, values.begin(), values.size() );
}

template< class Type >
TList< Type >::TList( const Type &value, int64 _num )
    : ptr( (Type *)Memory_Malloc( _num * sizeof( Type ) ) )
    , end( ptr + _num )
    , num( _num )
{
        ASSERT( ptr != nullptr );
        ASSERT( num > 0 );
        
        Memory_Fill< Type >( ptr, value, _num );
}

template< class Type >
TList< Type >::TList( TList &&other ) noexcept
    : ptr( other.ptr )
    , end( other.end )
    , num( other.num )
{
    other.ptr = nullptr;
    other.end = nullptr;
    other.num = 0;
}

template< class Type >
TList< Type >::TList( const TList &other )
    : ptr( nullptr )
    , end( nullptr )
    , num( 0 )
{
        *this = other;
}

template< class Type >
TList< Type >::~TList()
{
    Clear();
}

template< class Type >
constexpr bool TList< Type >::IsValid() const
{
    return ptr != nullptr;
}

template< class Type >
constexpr bool TList< Type >::IsEmpty() const
{
    return num == 0;
}

template< class Type >
constexpr int64 TList< Type >::GetSize() const
{
    return (int64)( end - ptr );
}

template< class Type >
constexpr int64 TList< Type >::GetNum() const
{
    return num;
}

template< class Type >
constexpr int64 TList< Type >::GetBytes() const
{
    return GetSize() * sizeof( Type );
}

template< class Type >
constexpr const Type *TList< Type >::GetPtr() const
{
    return ptr;
}

template< class Type >
Type *TList< Type >::GetPtr()
{
    return ptr;
}

template< class Type >
constexpr const Type *TList< Type >::GetEnd() const
{
    return ptr + num;
}

template< class Type >
Type *TList< Type >::GetEnd()
{
    return ptr + num;
}

template< class Type >
void TList< Type >::Insert( int64 index, const Type &value )
{
    // ASSERT( ptr != nullptr );
    ASSERT( index >= 0 );
    ASSERT( index <= num );
    
    if ( ( ptr + num ) >= end ) {
        Realloc();
    }

    for ( int64 i = num; i > index; i-- ) {
        // ptr[ i ] = ptr[ i - 1 ];
        ptr[ i ] = std::move( ptr[ i - 1 ] );
    }
    
    ptr[ index ] = value;
    ++num;
}

template< class Type >
void TList< Type >::Insert( int64 index, Type &&value )
{
    // ASSERT( ptr != nullptr );
    ASSERT( index >= 0 );
    ASSERT( index <= num );
    
    if ( ( ptr + num ) >= end ) {
        Realloc();
    }

    for ( int64 i = num; i > index; i-- ) {
        // ptr[ i ] = ptr[ i - 1 ];
        ptr[ i ] = std::move( ptr[ i - 1 ] );
    }
    
    ptr[ index ] = std::move( value );
    ++num;
}

template< class Type >
void TList< Type >::Append( const Type &value )
{
    if ( ( ptr + num ) >= end ) {
        Realloc();
    }
    
    ptr[ num ] = value;
    ++num;
}

template< class Type >
template< class... Args >
void TList< Type >::Emplace( Args&&... args )
{
    if ( ( ptr + num ) >= end ) {
        Realloc();
    }
    
    UNUSED( Memory_Place< Type >( ptr + num, std::forward< Args >( args )... ) );
    
    ++num;
}

template< class Type >
void TList< Type >::RemoveIndex( int64 index )
{
    ASSERT( ptr != nullptr );
    ASSERT( index >= 0 );
    ASSERT( index < num );
    
    --num;
    for ( int64 i = index; i < num; i++ ) {
        // ptr[ i ] = ptr[ i + 1 ];
        ptr[ i ] = std::move( ptr[ i + 1 ] );	// TODO: Memory_Move?
    }
}

template< class Type >
void TList< Type >::Remove( const Type &value )
{
    int64 index = FindIndex( value );
    if ( index != INVALID_INT64 ) {
        RemoveIndex( index );
    }
}

template< class Type >
void TList< Type >::Clear()
{
    if ( ptr != nullptr ) {
        for ( int64 i = 0; i < num; i++ ) {
            ptr[ i ].~Type();
        }

        Memory_Free( ptr );
    }
    
    ptr = nullptr;
    end = nullptr;
    num = 0;
}

// TODO: cleanup, num = _num is a hack and should
//       preferably be handled during allocation...
template< class Type >
void TList< Type >::Resize( int64 _num )
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

template< class Type >
void TList< Type >::Reserve( int64 size )
{
    ASSERT( size > 0 );
    
    if ( size == GetSize() ) {
        return;
    }
    
    Realloc( size );
}

template< class Type >
void TList< Type >::Shrink()
{
    if ( num == 0 ) {
        Clear();
        return;
    }
    
    if ( num == GetSize() ) {
        return;
    }
    
    Realloc( num );
}

template< class Type >
TSpan< Type > TList< Type >::Slice( int64 offset, int64 _num ) const
{
    ASSERT( ( ptr + offset + _num ) >= ptr );
    ASSERT( ( offset + _num ) <= num );
    
    return TSpan< Type >( ptr + offset, _num );
}

template< class Type >
int64 TList< Type >::FindIndex( const Type &value ) const
{
    for ( int64 i = 0; i < num; i++ ) {
        if ( ptr[ i ] == value ) {
            return i;
        }
    }
    
    return INVALID_INT64;
}

template< class Type >
const Type *TList< Type >::Find( const Type &value ) const
{
    const int64 index = FindIndex( value );
    return ( index != INVALID_INT64 ) ? ptr + index : nullptr;
}

template< class Type >
Type *TList< Type >::Find( const Type &value )
{
    const int64 index = FindIndex( value );
    return ( index != INVALID_INT64 ) ? ptr + index : nullptr;
}

template< class Type >
constexpr bool TList< Type >::operator==( const TList &other ) const
{
    if ( num != other.num ) {
        return false;
    }
    
    for ( int64 i = 0; i < num; i++ ) {
        if ( ptr[ i ] != other.ptr[ i ] ) {
            return false;
        }
    }

    return true;
}

template< class Type >
constexpr bool TList< Type >::operator!=( const TList &other ) const
{
    return !operator==( other );
}

template< class Type >
constexpr const Type &TList< Type >::operator[]( int64 index ) const
{
    ASSERT( ptr != nullptr );
    ASSERT( index >= 0 );
    ASSERT( index < GetSize() );
    
    return ptr[ index ];
}

template< class Type >
constexpr Type &TList< Type >::operator[]( int64 index )
{
    ASSERT( ptr != nullptr );
    ASSERT( index >= 0 );
    ASSERT( index < GetSize() );
    
    return ptr[ index ];
}

template< class Type >
TList< Type > &TList< Type >::operator=( TList &&other ) noexcept
{
    if ( this == &other ) {
        return *this;
    }
    
    Clear();
    
    ptr = other.ptr;
    end = other.end;
    num = other.num;
    
    other.ptr = nullptr;
    other.end = nullptr;
    other.num = 0;
    
    return *this;
}

template< class Type >
TList< Type > &TList< Type >::operator=( const TList &other )
{
    if ( this == &other ) {
        return *this;
    }
    
    Clear();
    
    num = other.num;
    
    if ( num == 0 ) {
        return *this;
    }
    
    ptr = (Type *)Memory_Malloc( num * sizeof( Type ) );
    
    Memory_Copy( ptr, other.ptr, num );
    
    end = ptr + num;
    
    return *this;
}

template< class Type >
void TList< Type >::Realloc( int64 size )
{
    ASSERT( size > 0 );
    
    Type *nptr = (Type *)Memory_Malloc( size * sizeof( Type ) );
    
    int64 _num = MIN( num, size );

    if ( ptr != nullptr ) {
        Memory_Copy( nptr, ptr, _num );
    }
        
    Clear();
        
    num = _num;
    ptr = nptr;
    end = ptr + size;
}

template< class Type >
void TList< Type >::Realloc()
{
    if ( ptr == nullptr ) {
        ptr = (Type *)Memory_Malloc( 16 * sizeof( Type ) );
        end = ptr + 16;
    }
    else {
        int64 size = GetSize();
        size += size >> 1;
        Realloc( size );
    }
}
