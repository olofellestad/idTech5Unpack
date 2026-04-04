#include "file/file.h"

bool File_Recurse( handle64 *h, const char *fileName )
{
	const char *p = fileName;
    const char *q = Str::Find( p, '/' );

    while ( q ) {
		TStaticStr< 255 > dirName( fileName, (int64)( q - fileName ) );
        System_Mkdir( dirName.GetPtr() );
        p = q + 1;
        q = Str::Find( p, '/' );
    };
	
    return System_FileOpen( h, fileName, FILE_WRITE );
}

uint32 File_ReadUint32B( handle64 h )
{
	uint32 u = 0;

	System_FileRead( h, &u, sizeof( u ) );
	return BYTESWAP32( u );
}

uint32 File_ReadUint32L( handle64 h )
{
	uint32 u = 0;

	System_FileRead( h, &u, sizeof( u ) );
	return u;
}

int64 File_Read( handle64 h, void *buffer, int64 size )
{
	return System_FileRead( h, buffer, size );
}

int64 File_Seek( handle64 h, int64 offset )
{
	return System_FileSeek( h, offset, FILE_HEAD );
}

int64 File_Tell( handle64 h )
{
	return System_FileSeek( h, 0, FILE_CURR );
}

//--------------------------------------------------------------------------------------------------------------------------------
File_Memory::File_Memory( uint8 *_ptr, uint8 *_end, int _flags )
	: ptr( _ptr )
	, end( _end )
	, pos( _ptr )
	, flags( _flags )
{
}

File_Memory::File_Memory( uint8 *_ptr, int64 size, int _flags )
	: File_Memory( _ptr, _ptr + size, _flags )
{
}

File_Memory::File_Memory( File_Memory &&other ) noexcept
	: ptr( other.ptr )
	, end( other.end )
	, pos( other.pos )
	, flags( other.flags )
{
	other.ptr   = nullptr;
	other.end   = nullptr;
	other.pos   = nullptr;
	other.flags = 0;
}

int64 File_Memory::Read( void *buffer, int64 size )
{
	if ( !HAS_BIT( flags, FILE_READ ) ) {
		return INVALID_INT64;
	}

	if ( ( pos + size ) >= end ) {
		size = ( end - pos );
	}

	memcpy( buffer, pos, size );

	pos += size;
	return size;
}

int64 File_Memory::ReadInt64()
{
	if ( !HAS_BIT( flags, FILE_READ ) ) {
		return 0;
	}

	const int64 size = sizeof( int64 );
	if ( ( pos + size ) >= end ) {
		return 0;
	}

	int64 value = *(int64 *)pos;

	pos += size;
	return value;
}

int32 File_Memory::ReadInt32()
{
	if ( !HAS_BIT( flags, FILE_READ ) ) {
		return 0;
	}

	const int64 size = sizeof( int32 );
	if ( ( pos + size ) >= end ) {
		return 0;
	}

	int32 value = *(int32 *)pos;

	pos += size;
	return value;
}

int16 File_Memory::ReadInt16()
{
	if ( !HAS_BIT( flags, FILE_READ ) ) {
		return 0;
	}

	const int64 size = sizeof( int16 );
	if ( ( pos + size ) >= end ) {
		return 0;
	}

	int16 value = *(int16 *)pos;

	pos += size;
	return value;
}

int8 File_Memory::ReadInt8()
{
	if ( !HAS_BIT( flags, FILE_READ ) ) {
		return 0;
	}

	const int64 size = sizeof( int8 );
	if ( ( pos + size ) >= end ) {
		return 0;
	}

	int8 value = *(int8 *)pos;

	pos += size;
	return value;
}

uint64 File_Memory::ReadUint64()
{
	if ( !HAS_BIT( flags, FILE_READ ) ) {
		return 0;
	}

	const int64 size = sizeof( uint64 );
	if ( ( pos + size ) >= end ) {
		return 0;
	}

	uint64 value = *(uint64 *)pos;

	pos += size;
	return value;
}

uint32 File_Memory::ReadUint32()
{
	if ( !HAS_BIT( flags, FILE_READ ) ) {
		return 0;
	}

	const int64 size = sizeof( uint32 );
	if ( ( pos + size ) >= end ) {
		return 0;
	}

	uint32 value = *(uint32 *)pos;

	pos += size;
	return value;
}

uint16 File_Memory::ReadUint16()
{
	if ( !HAS_BIT( flags, FILE_READ ) ) {
		return 0;
	}

	const int64 size = sizeof( uint16 );
	if ( ( pos + size ) >= end ) {
		return 0;
	}

	uint16 value = *(uint16 *)pos;

	pos += size;
	return value;
}

uint8 File_Memory::ReadUint8()
{
	if ( !HAS_BIT( flags, FILE_READ ) ) {
		return 0;
	}

	const int64 size = sizeof( uint8 );
	if ( ( pos + size ) >= end ) {
		return 0;
	}

	uint8 value = *(uint8 *)pos;

	pos += size;
	return value;
}

int64 File_Memory::ReadStr( Str &str )
{
	if ( !HAS_BIT( flags, FILE_READ ) ) {
		return INVALID_INT64;
	}

	uint32 len = ReadUint32();
	str.Resize( len );

	int64 n  = Read( str.GetPtr(), len );
	str[ n ] = '\0';

	return n;
}
/*
int64 File_Memory::Write( const void *buffer, int64 size ) {}
int64 File_Memory::WriteInt64( int64 value ) {}
int32 File_Memory::WriteInt32( int32 value ) {}
int16 File_Memory::WriteInt16( int16 value ) {}
int8 File_Memory::WriteInt8( int8 value ) {}
uint64 File_Memory::WriteUint64( uint64 value ) {}
uint32 File_Memory::WriteUint32( uint32 value ) {}
uint16 File_Memory::WriteUint16( uint16 value ) {}
uint8 File_Memory::WriteUint8( uint8 value ) {}
int64 File_Memory::WriteStr( const char *ptr ) {}
*/
int64 File_Memory::Seek( int64 position, int where )
{
	switch ( where ) {
	case FILE_HEAD:	pos = ptr + position;	break;
	case FILE_CURR:	pos = pos + position;	break;
	case FILE_TAIL:	pos = end + position;	break;
	default: UNREACHABLE(); break;
	}

	return Tell();
}

int64 File_Memory::Size()
{
	return (int64)( end - ptr );
}

int64 File_Memory::Tell()
{
	return (int64)( pos - ptr );
}

time64 File_Memory::Time()
{
	return MIN_TIME64;
}

File_Memory &File_Memory::operator=( File_Memory &&other ) noexcept	
{
	if ( this == &other ) {
		return *this;
	}

	ptr   = other.ptr;
	end   = other.end;
	pos   = other.pos;
	flags = other.flags;

	other.ptr   = nullptr;
	other.end   = nullptr;
	other.pos   = nullptr;
	other.flags = 0;

	return *this;
}
