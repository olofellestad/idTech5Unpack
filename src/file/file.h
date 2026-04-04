#pragma once

bool File_Recurse( handle64 *h, const char *fileName );

uint32 File_ReadUint32B( handle64 h );
uint32 File_ReadUint32L( handle64 h );
int64  File_Read( handle64 h, void *buffer, int64 size );

int64 File_Seek( handle64 h, int64 offset );
int64 File_Tell( handle64 h );

class IFile
{
public:
	virtual ~IFile() {}

	virtual const char *GetName() const = 0;
	virtual const char *GetPath() const = 0;

	virtual int64 Read( void *buffer, int64 size ) = 0;
	virtual int64 ReadInt64() = 0;
	virtual int32 ReadInt32() = 0;
	virtual int16 ReadInt16() = 0;
	virtual int8  ReadInt8() = 0;

	virtual uint64 ReadUint64() = 0;
	virtual uint32 ReadUint32() = 0;
	virtual uint16 ReadUint16() = 0;
	virtual uint8  ReadUint8() = 0;

	virtual int64 ReadStr( Str &str ) = 0;
/*
	virtual int64 Write( const void *buffer, int64 size ) = 0;
	virtual int64 WriteInt64( int64 value ) = 0;
	virtual int64 WriteInt32( int32 value ) = 0;
	virtual int64 WriteInt16( int16 value ) = 0;
	virtual int64 WriteInt8( int8 value ) = 0;

	virtual int64 WriteUint64( uint64 value ) = 0;
	virtual int64 WriteUint32( uint32 value ) = 0;
	virtual int64 WriteUint16( uint16 value ) = 0;
	virtual int64 WriteUint8( uint8 value ) = 0;

	virtual int64 WriteStr( const char *ptr ) = 0;
*/
	virtual int64 Seek( int64 position, int where ) = 0;
	virtual int64 Size() = 0;
	virtual int64 Tell() = 0;

	virtual time64 Time() = 0;
};

// class File_Real final : public IFile {};

class File_Memory final : public IFile
{
	// MAKE_DEFAULT_MOVABLE( File_Memory );
	MAKE_NONCOPYABLE( File_Memory );

public:
	File_Memory( uint8 *_ptr, uint8 *_end, int flags );
	File_Memory( uint8 *_ptr, int64 size, int flags );
	File_Memory( File_Memory &&other ) noexcept;

	virtual ~File_Memory() {}

	virtual const char *GetName() const { return ""; }
	virtual const char *GetPath() const { return ""; }

	virtual int64 Read( void *buffer, int64 size );
	virtual int64 ReadInt64();
	virtual int32 ReadInt32();
	virtual int16 ReadInt16();
	virtual int8  ReadInt8();

	virtual uint64 ReadUint64();
	virtual uint32 ReadUint32();
	virtual uint16 ReadUint16();
	virtual uint8  ReadUint8();

	virtual int64 ReadStr( Str &str );
/*
	virtual int64 Write( const void *buffer, int64 size );
	virtual int64 WriteInt64( int64 value );
	virtual int64 WriteInt32( int32 value );
	virtual int64 WriteInt16( int16 value );
	virtual int64 WriteInt8( int8 value );

	virtual int64 WriteUint64( uint64 value );
	virtual int64 WriteUint32( uint32 value );
	virtual int64 WriteUint16( uint16 value );
	virtual int64 WriteUint8( uint8 value );

	virtual int64 WriteStr( const char *ptr );
*/
	virtual int64 Seek( int64 position, int where );
	virtual int64 Size();
	virtual int64 Tell();

	virtual time64 Time();

	File_Memory &operator=( File_Memory &&other ) noexcept;

private:
	uint8 *ptr;
	uint8 *end;
	uint8 *pos;
	int    flags;
};

