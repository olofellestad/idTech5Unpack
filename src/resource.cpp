#include "resource.h"

#include <stdio.h>

#define SWAP32( u )                                                                                \
	( ( ( ( u ) & 0xFF000000 ) >> 24 ) | ( ( ( u ) & 0x00FF0000 ) >> 8 )                           \
	  | ( ( ( u ) & 0x0000FF00 ) << 8 ) | ( ( u ) << 24 ) )

static void Read( HANDLE fileHandle, void *ptr, UINT32 size )
{
	DWORD n = 0;
	(void)ReadFile( fileHandle, ptr, size, &n, nullptr );
}

static UINT32 ReadU32_LE( HANDLE fileHandle )
{
	UINT32 u = 0;
	DWORD  n = 0;
	if ( !ReadFile( fileHandle, &u, sizeof( u ), &n, nullptr ) ) {
		return 0;
	}

	return u;
}

static UINT32 ReadU32_BE( HANDLE fileHandle )
{
	UINT32 u = 0;
	DWORD  n = 0;
	if ( !ReadFile( fileHandle, &u, sizeof( u ), &n, nullptr ) ) {
		return 0;
	}

	return SWAP32( u );
}

ResourceContainer::ResourceContainer( const char *fileName )
	: name( _strdup( fileName ) )
	, files( nullptr )
	, fileHandle( INVALID_HANDLE_VALUE )
	, dataOffset( 0 )
	, numFiles( 0 )
{
	fileHandle = CreateFileA( fileName, GENERIC_READ, 0, nullptr, OPEN_EXISTING,
							  FILE_ATTRIBUTE_NORMAL, nullptr );
	if ( fileHandle == INVALID_HANDLE_VALUE ) {
		return;
	}

	UINT32 u = ReadU32_BE( fileHandle );
	switch ( u ) {
	case 0xD000000D: /*dbfg*/ CloseHandle( fileHandle ); break; // TODO: support
	case 0x03534552: /*res3*/ CloseHandle( fileHandle ); break; // TODO: support
	case 0x05534552: /*res5*/ CloseHandle( fileHandle ); break; // TODO: support
	case 0x2294ABCD: Load_RAGE(); break;
	default:		 CloseHandle( fileHandle ); break;
	}
}

ResourceContainer::~ResourceContainer()
{
	CloseHandle( fileHandle );
}

void ResourceContainer::Load_RAGE()
{
	// Header
	UINT32 indexOffset = ReadU32_BE( fileHandle );
	UINT32 indexSize   = ReadU32_BE( fileHandle );
	(void)indexSize;

	// this is where the data is
	dataOffset = SetFilePointer( fileHandle, 0, nullptr, FILE_CURRENT );

	// Index
	SetFilePointer( fileHandle, indexOffset, nullptr, FILE_BEGIN );

	numFiles = ReadU32_BE( fileHandle );
	files	 = new ResourceFile[ numFiles ];

	for ( UINT32 i = 0; i < numFiles; i++ ) {
		ResourceFile &rf = files[ i ];

		rf.rc = this;

		UINT32 unknown03 = ReadU32_BE( fileHandle ); // flags?
		(void)unknown03;

		UINT32 typeNameLen = ReadU32_LE( fileHandle );
		rf.typeName		   = new char[ typeNameLen + 1 ];
		Read( fileHandle, (char *)rf.typeName, typeNameLen );
		( (char *)rf.typeName )[ typeNameLen ] = '\0';

		UINT32 srcNameLen = ReadU32_LE( fileHandle );
		rf.srcName		  = new char[ srcNameLen + 1 ];
		Read( fileHandle, (char *)rf.srcName, srcNameLen );
		( (char *)rf.srcName )[ srcNameLen ] = '\0';

		UINT32 dstNameLen = ReadU32_LE( fileHandle );
		rf.dstName		  = new char[ dstNameLen + 1 ];
		Read( fileHandle, (char *)rf.dstName, dstNameLen );
		( (char *)rf.dstName )[ dstNameLen ] = '\0';

		rf.dstOffset = ReadU32_BE( fileHandle );
		rf.srcSize	 = ReadU32_BE( fileHandle );
		rf.dstSize	 = ReadU32_BE( fileHandle );

		// optional data, for idmsa files this is a list of languages
		// each entry is always 24 bytes
		UINT32 numUnknown = ReadU32_BE( fileHandle );
		for ( UINT32 j = 0; j < numUnknown; j++ ) {
			(void)ReadU32_BE( fileHandle ); // ???
			(void)ReadU32_BE( fileHandle ); // ???
			(void)ReadU32_BE( fileHandle ); // ???
			(void)ReadU32_BE( fileHandle ); // ???
			(void)ReadU32_BE( fileHandle ); // ???
			(void)ReadU32_BE( fileHandle ); // ???
		}

		// some kind of checksum? seems to be the same for all files
		(void)ReadU32_BE( fileHandle ); // ??? always 0x00FFFFFF ( be )
		(void)ReadU32_BE( fileHandle ); // ??? always 0x00000000 ( be )
		(void)ReadU32_BE( fileHandle ); // ??? always 0x4318BEFD ( be ) in .resources, checksum?
		(void)ReadU32_BE( fileHandle ); // ??? always 0xFE070000 ( be ) in .resources, checksum?
		(void)ReadU32_BE( fileHandle ); // ??? always 0x00000000 ( be )
	}
}