#include "resource.h"

#define SWAP32( u )                                                                                \
	( ( ( ( u ) & 0xFF000000 ) >> 24 ) | ( ( ( u ) & 0x00FF0000 ) >> 8 )                           \
	  | ( ( ( u ) & 0x0000FF00 ) << 8 ) | ( ( u ) << 24 ) )

static void Read( handle64 fileHandle, void *ptr, uint32 size )
{
	(void)System_FileRead( fileHandle, ptr, size );
	// UNUSED(ptr);
	// System_FileSeek( fileHandle, size, FILE_CURR );
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
	case 0xD000000D: /*dbfg*/ System_FileClose( fileHandle ); break; // TODO: support
	case 0x03534552: /*res3*/ System_FileClose( fileHandle ); break; // TODO: support
	case 0x05534552: /*res5*/ System_FileClose( fileHandle ); break; // TODO: support
	case 0x2294ABCD: Load_RAGE(); break;
	default:		 System_FileClose( fileHandle ); break;
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

	files.Resize( numFiles );

	for ( uint32 i = 0; i < files.GetNum(); i++ ) {
		ResourceFile &rf = files[ i ];

		rf.rc = this;

		uint32 unknown03 = ReadUint32_BE( fileHandle ); // flags?
		(void)unknown03;

		uint32 typeNameLen = ReadUint32_LE( fileHandle );
		rf.typeName.Reserve( typeNameLen + 1 );
		Read( fileHandle, rf.typeName.GetPtr(), typeNameLen );
		rf.typeName[ typeNameLen ] = '\0';

		uint32 srcNameLen = ReadUint32_LE( fileHandle );
		rf.srcName.Reserve( srcNameLen + 1 );
		Read( fileHandle, rf.srcName.GetPtr(), srcNameLen );
		rf.srcName[ srcNameLen ] = '\0';

		uint32 dstNameLen = ReadUint32_LE( fileHandle );
		rf.dstName.Reserve( dstNameLen + 1 );
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
	}
}