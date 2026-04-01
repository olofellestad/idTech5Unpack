#include "lib/containers/str.h"

void HeapStr::Alloc( const char *_ptr, int64 _num )
{
    Clear();
    
    ptr = (char *)Memory_Malloc( (_num + 1 ) * sizeof( char ) );
    
    if (  _ptr != nullptr && _num > 0 ) {
        Memory_Copy( ptr, _ptr, _num );
    }
    
    ptr[ _num ] = '\0';
    
    num = ( _num << 1 ) & ~STR_STATIC_MASK;
    end = ptr + _num + 1;
}

void HeapStr::Clear()
{
    if ( ptr != nullptr ) {
		Memory_Free( ptr );
    }
    
    num = STR_STATIC_MASK;
    ptr = nullptr;
    end = nullptr;
}

void HeapStr::Realloc( int64 size )
{
    ASSERT( size > 0 );
    
    int64 _num   = MIN( num >> 1, size - 1 );
	char *nptr = (char *)Memory_Malloc( size * sizeof( char ) );
    
    if ( ptr != nullptr ) {
        Memory_Copy( nptr, ptr, _num );
        nptr[ _num ] = '\0';
    }
    
    Clear();
        
    num = ( _num << 1 ) & ~STR_STATIC_MASK;
    ptr = nptr;
    end = ptr + size;
}

//--------------------------------------------------------------------------------------------------------------------------------
void StackStr::Alloc( const char *_ptr, int64 _num )
{
    ASSERT( _num < STR_STATIC_SIZE );
    
    if ( _ptr != nullptr && _num > 0 ) {
        Memory_Copy( buf, _ptr, _num );
    }
    
    buf[ _num ] = '\0';
    
    num = ( _num << 1 ) | STR_STATIC_MASK;
}

//--------------------------------------------------------------------------------------------------------------------------------
Str::Str( const bool b )
    : Str( b ? "true" : "false" )
{
}

Str::Str( const float32 f )
    : Str( va( "%f", f ) )
{
}

Str::Str( const int32 i )
    : Str( va( "%i", i ) )
{
}

Str::Str( const uint32 u )
    : Str( va( "%u", u ) )
{
}

Str::Str()
    : hstr( STR_STATIC_MASK,
            nullptr,
            nullptr )
{
}

Str::Str( const char *ptr, int64 num )
    : hstr( STR_STATIC_MASK,
            nullptr,
            nullptr )
{
    if ( num < STR_STATIC_SIZE ) {
        sstr.Alloc( ptr, num );
    }
    else {
        hstr.Alloc( ptr, num );
    }
}

Str::Str( const char *ptr )
    : Str( ptr, Str::GetLength( ptr ) )
{
}

Str::Str( char value, int64 num )
    : hstr( STR_STATIC_MASK,
            nullptr,
            nullptr )
{
    if ( num < STR_STATIC_SIZE ) {
        sstr.Alloc( nullptr, num );
        Memory_Fill( sstr.buf, value, num );
    }
    else {
        hstr.Alloc( nullptr, num );
        Memory_Fill( hstr.ptr, value, num );
    }
}

Str::Str( Str &&other ) noexcept
    : sstr( other.sstr )
{
    other.hstr.num = STR_STATIC_MASK;
    other.hstr.ptr = nullptr;
    other.hstr.end = nullptr;
}

Str::Str( const Str &other )
    : hstr( STR_STATIC_MASK,
            nullptr,
            nullptr )
{
    *this = other;
}

Str::~Str()
{
    Clear();
}

bool Str::IsStatic() const
{
    return hstr.num & STR_STATIC_MASK;
}

bool Str::IsValid() const
{
    return IsStatic() || hstr.ptr != nullptr;
}

bool Str::IsEmpty() const
{
    return IsStatic()
        ? ( sstr.num >> 1 ) == 0
        : ( hstr.num >> 1 ) == 0;
}

int64 Str::GetSize() const
{
    return IsStatic()
        ? STR_STATIC_SIZE
        : (int64)( hstr.end - hstr.ptr );
}

int64 Str::GetLength() const
{
    return IsStatic()
        ? (int64)( sstr.num >> 1 )
        : ( hstr.num >> 1 );
}

int64 Str::GetBytes() const
{
    return GetSize() * sizeof( char );
}

const char *Str::GetPtr() const
{
    return IsStatic()
        ? ( sstr.buf )
        : ( hstr.ptr );
}

char *Str::GetPtr()
{
    return IsStatic()
        ? ( sstr.buf )
        : ( hstr.ptr );
}

const char *Str::GetEnd() const
{
    return IsStatic()
        ? ( sstr.buf + STR_STATIC_SIZE - 1 )
        : ( hstr.ptr + ( hstr.num >> 1 ) );
}

char *Str::GetEnd()
{
    return IsStatic()
        ? ( sstr.buf + STR_STATIC_SIZE - 1 )
        : ( hstr.ptr + ( hstr.num >> 1 ) );
}

void Str::Insert( int64 index, char value )
{
    int64 num = GetLength();
    
    if ( ( num + 1 ) >= GetSize() ) {
        Realloc( 0 );   // automatically infers size
    }
    
    char *ptr = GetPtr();
    
    for ( int64 i = num; i > index; i-- ) {
        ptr[ i ] = ptr[ i - 1 ];
    }
    
    ptr[ index ]  = value;
    ptr[ num + 1] = '\0';
    
    SetLength( num + 1 );
}

// FIXME: allocate enough room...
// use Str( va( ... ) ) instead?
/*
void Str::Formatv( const char *format, std::va_list args )
{
    int64 num = GetLength();
    char *ptr = GetPtr();
    
    Str::Formatv( ptr, num, format, args );
}

void Str::Format( const char *format, ... )
{
    VARGS( args, format, { Formatv( format, args ); });
}
*/
void Str::Insert( int64 index, const Str &str )
{
    Insert( index, str.GetPtr(), str.GetLength() );
}

void Str::Insert( int64 index, const char *ptr )
{
    Insert( index, ptr, Str::GetLength( ptr ) );
}

void Str::Insert( int64 index, const char *_ptr, int64 _num )
{
    int64 num = GetLength();
    
    if ( ( num + _num ) >= GetSize() ) {
        Realloc( num + _num + 1 );
    }
    
    char *ptr = GetPtr();
    
    for ( int64 i = num + _num - 1; i >= index + _num; i-- ) {
        ptr[ i ] = ptr[ i - _num ];
    }
    
    Memory_Copy( ptr + index, _ptr, _num );
    
    ptr[ num + _num ] = '\0';
    
    SetLength( num + _num );
}

void Str::Append( char value )
{
    int64 num = GetLength();
    
    if ( ( num + 1 ) >= GetSize() ) {
        Realloc( 0 );   // automatically infers size
    }
    
    char *ptr = GetPtr();
    
    ptr[ num ]     = value;
    ptr[ num + 1 ] = '\0';
    
    SetLength( num + 1 );
}

void Str::Append( const Str &str )
{
    Append( str.GetPtr(), str.GetLength() );
}

void Str::Append( const char *ptr )
{
    Append( ptr, Str::GetLength( ptr ) );
}

void Str::Append( const char *_ptr, int64 _num )
{
    int64 num = GetLength();
    
    if ( ( num + _num ) >= GetSize() ) {
        Realloc( num + _num + 1 );
    }
    
    char *ptr = GetPtr();
    
    Memory_Copy( ptr + num, _ptr, _num );
    
    ptr[ num + _num ] = '\0';
    
    SetLength( num + _num );
}

void Str::RemoveIndex( int64 index )
{
    int64   num = GetLength();
    char *ptr = GetPtr();
    
    ASSERT( index >= 0 );
    ASSERT( index < num );
    
    for ( int64 i = index; i < num - 1; i++ ) {
        ptr[ i ] = ptr[ i + 1 ];
    }
    
    ptr[ num - 1 ] = '\0';
    
    SetLength( num - 1 );
}

void Str::Remove( char value )
{
    int64 index = FindIndex( 0, value );
    if ( index != INVALID_INT64 ) {
        RemoveIndex( index );
    }
}

void Str::Clear()
{
    if ( !IsStatic() ) {
        hstr.Clear();
    }
    
    hstr.num = STR_STATIC_MASK;
    hstr.ptr = nullptr;
    hstr.end = nullptr;
}

void Str::Resize( int64 length )
{
    ASSERT( length >= 0 );

    if ( length == GetLength() ) {
        return;
    } else if ( length == 0 ) {
        Clear();
    } else if ( length < STR_STATIC_SIZE ) {
        Shrink();
    } else {
       Realloc( length + 1 );
	   SetLength( length );
    }
}

void Str::Reserve( int64 size )
{
    ASSERT( size > 0 );
    
    if ( size == GetSize() ) {
        return;
    }
    
    Realloc( size );
}

void Str::Shrink()
{
    if ( IsStatic() ) {
        return;
    }
    
    int64 num  = GetLength();
    int64 size = GetSize();
    
    if ( num + 1 == size ) {
        return;
    }
    
    if ( num < STR_STATIC_SIZE ) {
        char tmp[ STR_STATIC_SIZE ];
        
        Memory_Copy( tmp, hstr.ptr, num );
        tmp[ num ] = '\0';
        
        hstr.Clear();
        sstr.Alloc( tmp, num );
    }
    else {
        hstr.Realloc( num + 1 );
    }
}


TSpan< char > Str::Slice( int64 offset, int64 num ) const
{
    ASSERT( offset >= 0 );
    ASSERT( num >= 0 );
    ASSERT( offset + num <= GetLength() );
    
    return TSpan< char >( const_cast< char * >( GetPtr() + offset ), num );
}

int64 Str::FindIndex( int64 start, char value ) const
{
    int64         num = GetLength();
    const char *ptr = GetPtr();
    
    for ( int64 i = start; i < num; i++ ) {
        if ( ptr[ i ] == value ) {
            return i;
        }
    }
    
    return INVALID_INT64;
}
 
int64 Str::FindIndex( int64 start, const Str &str ) const
{
    return FindIndex( start, str.GetPtr(), str.GetLength() );
}
 
int64 Str::FindIndex( int64 start, const char *ptr ) const
{
    return FindIndex( start, ptr, Str::GetLength( ptr ) );
}
 
int64 Str::FindIndex( int64 start, const char *_ptr, int64 _num ) const
{
    int64         num = GetLength();
    const char *ptr = GetPtr();
    
    if ( _num == 0 || _num > num ) {
        return INVALID_INT64;
    }
    
    for ( int64 i = start; i <= num - _num; i++ ) {
        if ( Compare( ptr + i, _ptr, _num ) == 0 ) {
            return i;
        }
    }
    
    return INVALID_INT64;
}

int64 Str::FindLastIndex( int64 start, char value ) const
{
    int64         num = GetLength();
    const char *ptr = GetPtr();
    
    for ( int64 i = num - 1; i >= start; i-- ) {
        if ( ptr[ i ] == value ) {
            return i;
        }
    }
    
    return INVALID_INT64;
}
 
int64 Str::FindLastIndex( int64 start, const Str &str ) const
{
    return FindLastIndex( start, str.GetPtr(), str.GetLength() );
}
 
int64 Str::FindLastIndex( int64 start, const char *ptr ) const
{
    return FindLastIndex( start, ptr, Str::GetLength( ptr ) );
}
 
int64 Str::FindLastIndex( int64 start, const char *_ptr, int64 _num ) const
{
    int64         num = GetLength();
    const char *ptr = GetPtr();
    
    if ( _num == 0 || _num > num ) {
        return INVALID_INT64;
    }
    
    for ( int64 i = num - _num; i >= start; i-- ) {
        if ( Compare( ptr + i, _ptr, _num ) == 0 ) {
            return i;
        }
    }
    
    return INVALID_INT64;
}

const char *Str::Find( int64 start, char value ) const
{
    int64 index = FindIndex( start, value );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

const char *Str::Find( int64 start, const Str &str ) const
{
    int64 index = FindIndex( start, str );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

const char *Str::Find( int64 start, const char *ptr ) const
{
    int64 index = FindIndex( start, ptr );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

const char *Str::Find( int64 start, const char *ptr, int64 num ) const
{
    int64 index = FindIndex( start, ptr, num );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

char *Str::Find( int64 start, char value )
{
    int64 index = FindIndex( start, value );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

char *Str::Find( int64 start, const Str &str )
{
    int64 index = FindIndex( start, str );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

char *Str::Find( int64 start, const char *ptr )
{
    int64 index = FindIndex( start, ptr );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

char *Str::Find( int64 start, const char *ptr, int64 num )
{
    int64 index = FindIndex( start, ptr, num );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

const char *Str::FindLast( int64 start, char value ) const
{
    int64 index = FindLastIndex( start, value );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

const char *Str::FindLast( int64 start, const Str &str ) const
{
    int64 index = FindLastIndex( start, str );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

const char *Str::FindLast( int64 start, const char *ptr ) const
{
    int64 index = FindLastIndex( start, ptr );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

const char *Str::FindLast( int64 start, const char *ptr, int64 num ) const
{
    int64 index = FindLastIndex( start, ptr, num );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

char *Str::FindLast( int64 start, char value )
{
    int64 index = FindLastIndex( start, value );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

char *Str::FindLast( int64 start, const Str &str )
{
    int64 index = FindLastIndex( start, str );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

char *Str::FindLast( int64 start, const char *ptr )
{
    int64 index = FindLastIndex( start, ptr );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

char *Str::FindLast( int64 start, const char *ptr, int64 num )
{
    int64 index = FindLastIndex( start, ptr, num );
    return ( index != INVALID_INT64 ) ? GetPtr() + index : nullptr;
}

bool Str::operator==( const Str &other ) const
{
    return GetLength() == other.GetLength()
        && Compare( GetPtr(), other.GetPtr() ) == 0;
}

bool Str::operator!=( const Str &other ) const
{
    return !operator==( other );
}

const char &Str::operator[]( int64 index ) const
{
    const char *ptr = GetPtr();
    return ptr[ index ];
}

char &Str::operator[]( int64 index )
{
    char *ptr = GetPtr();
    return ptr[ index ];
}

Str &Str::operator=( Str &&other ) noexcept
{
    if ( this == &other ) {
        return *this;
    }

    Clear();
    
    sstr = other.sstr;
    
    other.hstr.num = STR_STATIC_MASK;
    other.hstr.ptr = nullptr;
    other.hstr.end = nullptr;
    
    return *this;
}

Str &Str::operator=( const Str &other )
{
    if ( this == &other ) {
        return *this;
    }

    Clear();
    
    if ( other.IsStatic() ) {
        sstr = other.sstr;
    }
    else {
        hstr.Alloc( other.hstr.ptr, other.GetLength() );
    }
    
    return *this;
}

void Str::Realloc( int64 size )
{
    int64 num = GetLength();
    if ( IsStatic() ) {
        // char *nptr = new char[ size ? size : 64 ];
        char *nptr = (char *)Memory_Malloc( ( size ? size : 64 ) * sizeof( char ) );
        Memory_Copy( nptr, sstr.buf, num );
        nptr[ num ] = '\0';
        
        hstr.num = ( ( num ) << 1 ) & ~STR_STATIC_MASK;
        hstr.ptr = nptr;
        hstr.end = nptr + size;
    }
    else {
        size = size ? size : GetSize() * 2;
        hstr.Realloc( size );
    }
}

void Str::SetLength( int64 num )
{
    if ( IsStatic() ) {
        sstr.num = ( num << 1 ) | STR_STATIC_MASK;
    }
    else {
        hstr.num = ( num << 1 ) & ~STR_STATIC_MASK;
    }
}
