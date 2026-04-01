//--------------------------------------------------------------------------------------------------------------------------------
//  str.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

#include <cstdarg>

#define STR_STATIC_SIZE (int64)( sizeof( HeapStr ) - 1 )
#define STR_STATIC_MASK 1ll

#ifndef BYTE_ORDER_LE
#   error Unsupported byte order!
#endif

class HeapStr
{
public:
    void Alloc( const char *_ptr, int64 _num );
    void Clear();
    void Realloc( int64 size );
    
public:
    int64 num;
    char *ptr;
    char *end;
};

class StackStr
{
public:
    void Alloc( const char *_ptr, int64 _num );
    
public:
    uint8 num;
    char  buf[ STR_STATIC_SIZE ];
};

// TODO: need to optimize...
class Str
{
public:
    static constexpr int    Compare( const char *a, const char *b );
    static constexpr int    Compare( const char *a, const char *b, int64 num );
    static constexpr uint64 Hash( const char *ptr );
    static constexpr uint64 Hash( const char *ptr, int64 num );
    
    static constexpr int    ICompare( const char *a, const char *b );
    static constexpr int    ICompare( const char *a, const char *b, int64 num );
    static constexpr uint64 IHash( const char *ptr );
    static constexpr uint64 IHash( const char *ptr, int64 num );
    
    static constexpr int64 Formatv( char *ptr, int64 num, const char *fmt, std::va_list args );
    static constexpr int64 Format( char *ptr, int64 num, const char *fmt, ... );

	static constexpr const char *Find( const char *ptr, int64 num, char value );
	static constexpr const char *Find( const char *ptr, char value );
	static constexpr const char *FindLast( const char *ptr, int64 num, char value );
	static constexpr const char *FindLast( const char *ptr, char value );
    
    static constexpr char ToLower( char value );
    static constexpr char ToUpper( char value );
    
    static constexpr bool IsLower( char value );
    static constexpr bool IsUpper( char value );
/*
    static constexpr void ToLowerUTF8( char *dst, const char *ptr );
    static constexpr void ToUpperUTF8( char *dst, const char *ptr );
    static constexpr bool IsLowerUTF8( const char *ptr );
    static constexpr bool IsUpperUTF8( const char *ptr );
    static constexpr bool IsAlphaUTF8( const char *ptr );
    static constexpr bool IsAlnumUTF8( const char *ptr );
    static constexpr bool IsUTF8( const char *ptr );
*/
    static constexpr bool IsAscii( char value );
    static constexpr bool IsAlpha( char value );
    static constexpr bool IsAlnum( char value );
    static constexpr bool IsDigit( char value );
    static constexpr bool IsEscape( char value );
    static constexpr bool IsSpace( char value );
    
    static constexpr int64 GetLength( const char *ptr );
    
public:
    explicit Str( const bool b );
    explicit Str( const float32 f );
    explicit Str( const int32 i );
    explicit Str( const uint32 u );
    
    Str();
    Str( const char *ptr, int64 num );
    Str( const char *ptr );
    Str( char value, int64 num );
    Str( Str &&other ) noexcept;
    Str( const Str &other );
    
    ~Str();
    
    bool IsStatic() const;
    bool IsValid() const;
    bool IsEmpty() const;
    
    int64 GetSize() const;
    int64 GetLength() const;
    
    int64 GetBytes() const;
    
    const char *GetPtr() const;
    char *      GetPtr();
    const char *GetEnd() const;
    char *      GetEnd();
    
//    void Formatv( const char *format, std::va_list args );
//    void Format( const char *format, ... );
    
    void Insert( int64 index, char value );
    void Insert( int64 index, const Str &str );
    void Insert( int64 index, const char *str );
    void Insert( int64 index, const char *ptr, int64 num );
    void Append( char value );
    void Append( const Str &str );
    void Append( const char *str );
    void Append( const char *ptr, int64 num );
    
    void RemoveIndex( int64 index );
    void Remove( char value );
    void Clear();
    
 	void Resize( int64 length );
    void Reserve( int64 size );
    void Shrink();
    
    TView< char > Slice( int64 offset, int64 num ) const;
    TSpan< char > Slice( int64 offset, int64 num );
    
    int64       FindIndex( int64 start, char value ) const;
    int64       FindIndex( int64 start, const Str &str ) const;
    int64       FindIndex( int64 start, const char *ptr ) const;
    int64       FindIndex( int64 start, const char *ptr, int64 num ) const;
    int64       FindLastIndex( int64 start, char value ) const;
    int64       FindLastIndex( int64 start, const Str &str ) const;
    int64       FindLastIndex( int64 start, const char *ptr ) const;
    int64       FindLastIndex( int64 start, const char *ptr, int64 num ) const;
    const char *Find( int64 start, char value ) const;
    const char *Find( int64 start, const Str &str ) const;
    const char *Find( int64 start, const char *ptr ) const;
    const char *Find( int64 start, const char *ptr, int64 num ) const;
    char *      Find( int64 start, char value ) ;
    char *      Find( int64 start, const Str &str );
    char *      Find( int64 start, const char *ptr );
    char *      Find( int64 start, const char *ptr, int64 num );
    const char *FindLast( int64 start, char value ) const;
    const char *FindLast( int64 start, const Str &str ) const;
    const char *FindLast( int64 start, const char *ptr ) const;
    const char *FindLast( int64 start, const char *ptr, int64 num ) const;
    char *      FindLast( int64 start, char value ) ;
    char *      FindLast( int64 start, const Str &str );
    char *      FindLast( int64 start, const char *ptr );
    char *      FindLast( int64 start, const char *ptr, int64 num );
    
    bool operator==( const Str &other ) const;
    bool operator!=( const Str &other ) const;
    
    const char &operator[]( int64 index ) const;
    char &      operator[]( int64 index );
    
    Str &operator=( Str &&other ) noexcept;
    Str &operator=( const Str &other );
    
private:
    void Realloc( int64 size );
    
    void SetLength( int64 num );
    
private:
    union {
        HeapStr  hstr;
        StackStr sstr;
    };
};

//--------------------------------------------------------------------------------------------------------------------------------
// TODO: case-insensitive
template<>
class TSort< Str >
{
public:
    int Compare( const Str &a, const Str &b ) const
    {
        return Str::Compare( a.GetPtr(), b.GetPtr() );
    }
};

template<>
class TSort< const char * >
{
public:
    int Compare( const char *a, const char *b ) const
    {
        return Str::Compare( a, b );
    }
};

template<>
class TSort_Reverse< Str >
{
public:
    int Compare( const Str &a, const Str &b ) const
    {
        return -Str::Compare( a.GetPtr(), b.GetPtr() );
    }
};

template<>
class TSort_Reverse< const char * >
{
public:
    int Compare( const char *a, const char *b ) const
    {
        return -Str::Compare( a, b );
    }
};

template<>
class THash< Str >
{
public:
    uint64 Hash( const Str &value ) const
    {
        return Str::Hash( value.GetPtr(), value.GetLength() );
    }
};

template<>
class THash< const char * >
{
public:
    uint64 Hash( const char *value ) const
    {
        return Str::Hash( value );
    }
};

//--------------------------------------------------------------------------------------------------------------------------------
constexpr int Str::Compare( const char *a, const char *b )
{
    for ( ; *a && *a == *b; a++, b++ );
    return *(uint8 *)a - *(uint8 *)b;
}

constexpr int Str::Compare( const char *a, const char *b, int64 num )
{
    for ( ; *a && *b && num && *a == *b; a++, b++, num-- );
    if ( num == 0 ) return 0;
    return *(uint8 *)a - *(uint8 *)b;
}

constexpr uint64 Str::Hash( const char *ptr )
{
    uint64 hash = 0xCBF29CE484222325;
    while ( *ptr ) {
        hash ^= (uint64)*(uint8 *)ptr;
        hash *= 0x100000001B3;
        ++ptr;
    }
    
    return hash;
}

constexpr uint64 Str::Hash( const char *ptr, int64 num )
{
    uint64 hash = 0xCBF29CE484222325;
    while ( num-- ) {
        hash ^= (uint64)*(uint8 *)ptr;
        hash *= 0x100000001B3;
        ++ptr;
    }
    
    return hash;
}

constexpr int Str::ICompare( const char *a, const char *b )
{
    for ( ; *a && Str::ToLower( *a ) == Str::ToLower( *b ); a++, b++ );
    return *(uint8 *)a - *(uint8 *)b;
}

constexpr int Str::ICompare( const char *a, const char *b, int64 num )
{
    for ( ; *a && *b && num && Str::ToLower( *a ) == Str::ToLower( *b ); a++, b++, num-- );
    if ( num == 0 ) return 0;
    return *(uint8 *)a - *(uint8 *)b;
}

constexpr uint64 Str::IHash( const char *ptr )
{
    uint64 hash = 0xCBF29CE484222325;
    while ( *ptr ) {
        hash ^= (uint64)ToLower( *(uint8 *)ptr );
        hash *= 0x100000001B3;
        ++ptr;
    }
    
    return hash;
}

constexpr uint64 Str::IHash( const char *ptr, int64 num )
{
    uint64 hash = 0xCBF29CE484222325;
    while ( num-- ) {
        hash ^= (uint64)ToLower( *(uint8 *)ptr );
        hash *= 0x100000001B3;
        ++ptr;
    }
    
    return hash;
}

constexpr int64 Str::Formatv( char *ptr, int64 num, const char *format, std::va_list args )
{
    return std::vsnprintf( ptr, num, format, args );
}

constexpr int64 Str::Format( char *ptr, int64 num, const char *fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    int64 n = Formatv( ptr, num, fmt, args );
    va_end( args );
    
    return n;
}

constexpr const char *Str::Find( const char *ptr, int64 num, char value )
{
	for ( int64 i = 0; i < num; i++ ) {
		if ( ptr[ i ] == value ) {
			return ptr + i;
		}
	}

	return nullptr;
}

constexpr const char *Str::Find( const char *ptr, char value )
{
	for ( int64 i = 0; ptr[ i ]; i++ ) {
		if ( ptr[ i ] == value ) {
			return ptr + i;
		}
	}

	return nullptr;
}

constexpr const char *Str::FindLast( const char *ptr, int64 num, char value )
{
	for ( int64 i = num - 1; i >= 0; i-- ) {
		if ( ptr[ i ] == value ) {
			return ptr + i;
		}
	}

	return nullptr;
}

constexpr const char *Str::FindLast( const char *ptr, char value )
{
	return Str::FindLast( ptr, Str::GetLength( ptr ), value );
}

constexpr char Str::ToLower( char value )
{
    return IsUpper( value ) ? value + 0x20 : value;
}

constexpr char Str::ToUpper( char value )
{
    return IsLower( value ) ? value - 0x20 : value;
}

constexpr bool Str::IsLower( char value )
{
    return value >= 'a' && value <= 'z';
}

constexpr bool Str::IsUpper( char value )
{
    return value >= 'A' && value <= 'Z';
}
/*
constexpr void Str::ToLowerUTF8( char *dst, const char *ptr )
{
    ASSERT( 0 && "not implemented" );
}

constexpr void Str::ToUpperUTF8( char *dst, const char *ptr )
{
    ASSERT( 0 && "not implemented" );
}

constexpr bool Str::IsLowerUTF8( const char *ptr )
{
    ASSERT( 0 && "not implemented" );
}

constexpr bool Str::IsUpperUTF8( const char *ptr )
{
    ASSERT( 0 && "not implemented" );
}

constexpr bool Str::IsUTF8( const char *ptr )
{
    ASSERT( 0 && "not implemented" );
}
*/
constexpr bool Str::IsAscii( char value )
{
    return value >= '\x00' && value <= '\x7F';
}

constexpr bool Str::IsAlpha( char value )
{
    return ( value >= 'A' && value <= 'Z' )
        || ( value >= 'a' && value <= 'z' );
}
 
constexpr bool Str::IsAlnum( char value )
{
    return IsAlpha( value ) || IsDigit( value );
}

constexpr bool Str::IsDigit( char value )
{
    return value >= '0' && value <= '9';
}

constexpr bool Str::IsEscape( char value )
{
    return ( value >= '\x00' && value <= '\x1F' )
        || ( value == '\x7F' );
}

constexpr bool Str::IsSpace( char value )
{
    return ( value >= '\t' && value <= '\r' )
        || ( value == ' ' );
}

constexpr int64 Str::GetLength( const char *ptr )
{
    int64 i = 0;
    while ( ptr[ i ] ) i++;
    return i;
}
