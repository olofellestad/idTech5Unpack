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
/*
template< class KType, class VType, class Hash = THash< KType > >
class TMap
{
public:
    constexpr TMap();
//    TMap( std::initializer_list< entry_t > entries );
    TMap( TMap &&other ) noexcept;
    TMap( const TMap &other );
    
    ~TMap();
    
    constexpr bool IsValid() const;
    constexpr bool IsEmpty() const;
    
    constexpr I64 GetSize() const;
    constexpr I64 GetNum() const;
    
    constexpr I64 GetBytes() const;
    
    constexpr const VType *GetPtr() const;
    VType *                GetPtr();
    constexpr const VType *GetEnd() const;
    VType *                GetEnd();
    
    bool HasKey( const KType &key ) const;
    bool Has( const VType &value ) const;
    
    void Insert( const KType &key, const VType &value );
    
    void Clear();
    
    void Resize( I64 size );
    void Shrink();
    
    template< class Sort = TSort< VType > >
    void Sort( const Sort &sort = Sort() )
    {
        return QuickSort( GetPtr(), GetEnd(), sort );
    }
    
    TSpan< VType > Slice( I64 offset, I64 _num ) const;
    
    I64          FindKey( const KType &key ) const;
    const VType *Find( const VType &value ) const;
    VType *      Find( const VType &value );
    
    constexpr bool operator==( const TMap &other ) const;
    constexpr bool operator!=( const TMap &other ) const;
    
    constexpr const VType &operator[]( const KType &key ) const;
    constexpr VType &      operator[]( const KType &key );
    
    TMap &operator=( TMap &&other ) noexcept;
    TMap &operator=( const TMap &other );
    
private:
    void Realloc( I64 size );
    void Realloc();
    
private:
//    VType *ptr;
//    VType *end;
//    I64    num;
};

//--------------------------------------------------------------------------------------------------------------------------------
*/
