//--------------------------------------------------------------------------------------------------------------------------------
//  staticstr.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

template< int64 NUM >
class TStaticStr
{
    MAKE_DEFAULT_COPYABLE( TStaticStr );
    
public:
    constexpr TStaticStr();
    constexpr TStaticStr( const char *_ptr, int64 _num );
    constexpr TStaticStr( const char *_ptr );
    
    ~TStaticStr() = default;
    
    int64 GetSize() const;
    int64 GetLength() const;
    
    int64 GetBytes() const;
    
    const char *GetPtr() const;
    char *      GetPtr();
    const char *GetEnd() const;
    char *      GetEnd();
    
    void Formatv( const char *format, std::va_list args );
    void Format( const char *format, ... );
    
 	void Resize( int64 length );

    TView< char > Slice( int64 offset, int64 _num ) const;
    TSpan< char > Slice( int64 offset, int64 _num );
/*
    int64         FindIndex( int64 start, char value ) const;
    int64         FindIndex( int64 start, const Str &str ) const;
    int64         FindIndex( int64 start, const char *ptr ) const;
    int64         FindIndex( int64 start, const char *ptr, int64 num ) const;
    int64         FindLastIndex( int64 start, char value ) const;
    int64         FindLastIndex( int64 start, const Str &str ) const;
    int64         FindLastIndex( int64 start, const char *ptr ) const;
    int64         FindLastIndex( int64 start, const char *ptr, int64 num ) const;
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
*/
    bool operator==( const TStaticStr &other ) const;
    bool operator!=( const TStaticStr &other ) const;
    
    const char &operator[]( int64 index ) const;
    char &      operator[]( int64 index );
    
private:
    char  ptr[ NUM + 1 ];
	int64 num;
};

//--------------------------------------------------------------------------------------------------------------------------------
template< int64 NUM >
class TSort< TStaticStr< NUM > >
{
public:
    int Compare( const TStaticStr< NUM > &a, const TStaticStr< NUM > &b ) const
    {
        return Str::Compare( a.GetPtr(), b.GetPtr() );
    }
};

//--------------------------------------------------------------------------------------------------------------------------------
template< int64 NUM >
constexpr TStaticStr< NUM >::TStaticStr()
	: num( 0 )
{
    Memory_Fill( ptr, '\0', NUM + 1 );
}

template< int64 NUM >
constexpr TStaticStr< NUM >::TStaticStr( const char *_ptr, int64 _num )
	: num( _num )
{
    ASSERT( num <= NUM );
    
    Memory_Copy( ptr, _ptr, num );
    ptr[ num ] = '\0';
}

template< int64 NUM >
constexpr TStaticStr< NUM >::TStaticStr( const char *_ptr )
    : TStaticStr( _ptr, Str::GetLength( _ptr ) )
{
}

template< int64 NUM >
int64 TStaticStr< NUM >::GetSize() const
{
    return NUM + 1;
}

template< int64 NUM >
int64 TStaticStr< NUM >::GetLength() const
{
    return num;
}

template< int64 NUM >
int64 TStaticStr< NUM >::GetBytes() const
{
    return GetSize() * sizeof( char );
}

template< int64 NUM >
const char *TStaticStr< NUM >::GetPtr() const
{
    return ptr;
}

template< int64 NUM >
char *TStaticStr< NUM >::GetPtr()
{
    return ptr;
}

template< int64 NUM >
const char *TStaticStr< NUM >::GetEnd() const
{
    return ptr + num;
}

template< int64 NUM >
char *TStaticStr< NUM >::GetEnd()
{
    return ptr + num;
}

template< int64 NUM >
void TStaticStr< NUM >::Formatv( const char *format, std::va_list args )
{
    Str::Formatv( ptr, NUM, format, args );
}

template< int64 NUM >
void TStaticStr< NUM >::Format( const char *format, ... )
{
    va_list args;
    va_start( args, format );
    Formatv( format, args );
    va_end( args );
}

template< int64 NUM >
void TStaticStr< NUM >::Resize( int64 length )
{
	ASSERT( length <= NUM );

	num        = length;
	ptr[ num ] = '\0';
}

template< int64 NUM >
TView< char > TStaticStr< NUM >::Slice( int64 offset, int64 _num ) const
{
    ASSERT( ( ptr + offset + _num ) >= ptr );
    ASSERT( ( offset + _num ) <= num );
    ASSERT( ( offset + _num ) <= NUM );
    
    return TView< char >( ptr + offset, _num );
}

template< int64 NUM >
TSpan< char > TStaticStr< NUM >::Slice( int64 offset, int64 _num )
{
    ASSERT( ( ptr + offset + _num ) >= ptr );
    ASSERT( ( offset + _num ) <= num );
    ASSERT( ( offset + _num ) <= NUM );
    
    return TSpan< char >( ptr + offset, _num );
}

template< int64 NUM >
bool TStaticStr< NUM >::operator==( const TStaticStr &other ) const
{
    return num == other.num && Str::Compare( ptr, other.ptr ) == 0;
}

template< int64 NUM >
bool TStaticStr< NUM >::operator!=( const TStaticStr &other ) const
{
    return !operator==( other );
}

template< int64 NUM >
const char &TStaticStr< NUM >::operator[]( int64 index ) const
{
    ASSERT( index >= 0 );
    ASSERT( index <= NUM );
    
    return ptr[ index ];
}


template< int64 NUM >
char &TStaticStr< NUM >::operator[]( int64 index )
{
    ASSERT( index >= 0 );
    ASSERT( index <= NUM );
    
    return ptr[ index ];
}
