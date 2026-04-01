#include "resource.h"

#define SWAP32( u )                                                                                \
	( ( ( ( u ) & 0xFF000000 ) >> 24 ) | ( ( ( u ) & 0x00FF0000 ) >> 8 )                           \
	  | ( ( ( u ) & 0x0000FF00 ) << 8 ) | ( ( u ) << 24 ) )

static void Read( handle64 fileHandle, void *ptr, uint32 size )
{
	(void)System_FileRead( fileHandle, ptr, size );
}

static uint32 ReadUint32_LE( handle64 fileHandle )
{
	uint32 u = 0;
	int64  n = System_FileRead( fileHandle, &u, sizeof( u ) );
	return n == sizeof( u ) ? u : 0;
}

static uint32 ReadUint32_BE( handle64 fileHandle )
{
	uint32 u = ReadUint32_LE( fileHandle );
	return SWAP32( u );
}

ResourceFile::ResourceFile( ResourceContainer *_rc )
	: dstOffset( 0 )
	, srcSize( 0 )
	, dstSize( 0 )
	, rc( _rc )
{
}

ResourceFile::ResourceFile( ResourceFile &&other ) noexcept
	: typeName( std::move( other.typeName ) )
	, srcName( std::move( other.srcName ) )
	, dstName( std::move( other.dstName ) )
	, dstOffset( other.dstOffset )
	, srcSize( other.srcSize )
	, dstSize( other.dstSize )
	, rc( other.rc )
{
	other.dstOffset = 0;
	other.srcSize   = 0;
	other.dstSize   = 0;
	other.rc        = nullptr;
}

ResourceFile::ResourceFile( const ResourceFile &other )
	: dstOffset( 0 )
	, srcSize( 0 )
	, dstSize( 0 )
	, rc( nullptr )
{
	*this = other;
}

ResourceFile &ResourceFile::operator=( ResourceFile &&other ) noexcept
{
	if ( this == &other ) {
		return *this;
	}

    typeName  = std::move( other.typeName );
    srcName   = std::move( other.srcName );
    dstName   = std::move( other.dstName );
    dstOffset = other.dstOffset;
    srcSize   = other.srcSize;
    dstSize   = other.dstSize;
    rc        = other.rc;

	return *this;
}

ResourceFile &ResourceFile::operator=( const ResourceFile &other )
{
	if ( this == &other ) {
		return *this;
	}

    typeName  = other.typeName;
    srcName   = other.srcName;
    dstName   = other.dstName;
    dstOffset = other.dstOffset;
    srcSize   = other.srcSize;
    dstSize   = other.dstSize;
    rc        = other.rc;

	return *this;
}

ResourceContainer::ResourceContainer( const char *_fileName )
	: fileName( _fileName )
	, fileHandle( INVALID_HANDLE64 )
	, dataOffset( 0 )
{
	if ( !System_FileOpen( &fileHandle, _fileName, FILE_READ ) ) {
		return;
	}

	uint32 magicNumber = ReadUint32_BE( fileHandle );
	switch ( magicNumber ) {
	case 0xD000000D: System_Fail( "Unsupported file!" ); System_FileClose( fileHandle ); break; // TODO: DOOM BFG
	case 0x03534552: System_Fail( "Unsupported file!" ); System_FileClose( fileHandle ); break; // TODO: res3
	case 0x05534552: System_Fail( "Unsupported file!" ); System_FileClose( fileHandle ); break; // TODO: res5
	case 0x2294ABCD: Load_RAGE(); break;
	default:		 System_Fail( "Unsupported file!" ); System_FileClose( fileHandle ); break;
	}
}

ResourceContainer::~ResourceContainer()
{
	System_FileClose( fileHandle );
}

void ResourceContainer::Load_RAGE()
{
	// Header
	uint32 indexOffset = ReadUint32_BE( fileHandle );
	uint32 indexSize   = ReadUint32_BE( fileHandle );
	(void)indexSize;

	// this is where the data is
	dataOffset = System_FileSeek( fileHandle, 0, FILE_CURR );

	// Index
	System_FileSeek( fileHandle, indexOffset, FILE_HEAD );

	uint32 numFiles = ReadUint32_BE( fileHandle );

	// files.Resize( numFiles );
	files.Reserve( numFiles );

	for ( uint32 i = 0; i < numFiles; i++ ) {
		ResourceFile rf( this );

		uint32 unknown03 = ReadUint32_BE( fileHandle ); // flags?
		(void)unknown03;

		uint32 typeNameLen = ReadUint32_LE( fileHandle );
		rf.typeName.Resize( typeNameLen );
		Read( fileHandle, rf.typeName.GetPtr(), typeNameLen );
		rf.typeName[ typeNameLen ] = '\0';

		uint32 srcNameLen = ReadUint32_LE( fileHandle );
		rf.srcName.Resize( srcNameLen );
		Read( fileHandle, rf.srcName.GetPtr(), srcNameLen );
		rf.srcName[ srcNameLen ] = '\0';

		uint32 dstNameLen = ReadUint32_LE( fileHandle );
		rf.dstName.Resize( dstNameLen );
		Read( fileHandle, rf.dstName.GetPtr(), dstNameLen );
		rf.dstName[ dstNameLen ] = '\0';

		rf.dstOffset = ReadUint32_BE( fileHandle );
		rf.srcSize	 = ReadUint32_BE( fileHandle );
		rf.dstSize	 = ReadUint32_BE( fileHandle );

		// optional data, for idmsa files this is a list of languages
		// each entry is always 24 bytes
		uint32 numUnknown = ReadUint32_BE( fileHandle );
		for ( uint32 j = 0; j < numUnknown; j++ ) {
			(void)ReadUint32_BE( fileHandle ); // ???
			(void)ReadUint32_BE( fileHandle ); // ???
			(void)ReadUint32_BE( fileHandle ); // ???
			(void)ReadUint32_BE( fileHandle ); // ???
			(void)ReadUint32_BE( fileHandle ); // ???
			(void)ReadUint32_BE( fileHandle ); // ???
		}

		// some kind of checksum? seems to be the same for all files
		(void)ReadUint32_BE( fileHandle ); // ??? always 0x00FFFFFF ( be )
		(void)ReadUint32_BE( fileHandle ); // ??? always 0x00000000 ( be )
		(void)ReadUint32_BE( fileHandle ); // ??? always 0x4318BEFD ( be ) in .resources, checksum?
		(void)ReadUint32_BE( fileHandle ); // ??? always 0xFE070000 ( be ) in .resources, checksum?
		(void)ReadUint32_BE( fileHandle ); // ??? always 0x00000000 ( be )
		
		files.Insert( rf.dstName, std::move( rf ) );
	}
}
