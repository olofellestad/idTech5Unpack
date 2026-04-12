#include "resource.h"

#include "file/file.h"

ResourceFile::ResourceFile( ResourceContainer *_rc )
	: rc( _rc )
	, dstOffset( 0 )
	, srcSize( 0 )
	, dstSize( 0 )
{
}

ResourceFile::ResourceFile( ResourceFile &&other ) noexcept
	: rc( other.rc )
	, typeName( std::move( other.typeName ) )
	, srcName( std::move( other.srcName ) )
	, dstName( std::move( other.dstName ) )
	, dstOffset( other.dstOffset )
	, srcSize( other.srcSize )
	, dstSize( other.dstSize )
{
	other.rc        = nullptr;
	other.dstOffset = 0;
	other.srcSize   = 0;
	other.dstSize   = 0;
}

ResourceFile::ResourceFile( const ResourceFile &other )
	: rc( nullptr )
	, dstOffset( 0 )
	, srcSize( 0 )
	, dstSize( 0 )
{
	*this = other;
}

bool ResourceFile::IsCompressed() const
{
	return srcSize != dstSize;
}

bool ResourceFile::Unpack()
{
	TStaticStr< 255 > outName;

	// this is a mess...
	const char *rcExt = rc->fileName.Find( 0ll, '.' );
	const char *rcPtr = rc->fileName.GetPtr();

	outName.Format( "%.*s/%s", (int64)( rcExt - rcPtr ),
							   rcPtr, dstName.GetPtr() );

	// TODO: DEFLATE compression
	if ( IsCompressed() ) {
		return false;
	}

	void *buffer = Memory_Malloc( dstSize );
    System_FileSeek( rc->fileHandle, dstOffset, FILE_HEAD );
    if ( System_FileRead( rc->fileHandle, buffer, dstSize ) != dstSize ) {
		Memory_Free( buffer );
        System_Warn( "Failed to read '%s'", dstName.GetPtr() );
		return false;
    }

	handle64 fileHandle = INVALID_HANDLE64;
	File_Recurse( &fileHandle, outName.GetPtr() );

	if ( System_FileWrite( fileHandle, buffer, dstSize ) != dstSize ) {
		System_Warn( "Failed to write '%s'", outName.GetPtr() );
	}

	System_FileClose( fileHandle );
	Memory_Free( buffer );

	return true;
}

ResourceFile &ResourceFile::operator=( ResourceFile &&other ) noexcept
{
	if ( this == &other ) {
		return *this;
	}

    rc        = other.rc;
    typeName  = std::move( other.typeName );
    srcName   = std::move( other.srcName );
    dstName   = std::move( other.dstName );
    dstOffset = other.dstOffset;
    srcSize   = other.srcSize;
    dstSize   = other.dstSize;

	return *this;
}

ResourceFile &ResourceFile::operator=( const ResourceFile &other )
{
	if ( this == &other ) {
		return *this;
	}

    rc        = other.rc;
    typeName  = other.typeName;
    srcName   = other.srcName;
    dstName   = other.dstName;
    dstOffset = other.dstOffset;
    srcSize   = other.srcSize;
    dstSize   = other.dstSize;

	return *this;
}

//--------------------------------------------------------------------------------------------------------------------------------
ResourceContainer::ResourceContainer( const char *_fileName )
	: fileName( _fileName )
	, fileHandle( INVALID_HANDLE64 )
	, dataOffset( 0 )
{
	if ( !System_FileOpen( &fileHandle, _fileName, FILE_READ ) ) {
		return;
	}

	uint32 magicNumber = File_ReadUint32B( fileHandle );
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

#define READBIGUINT32( f, x )							\
	x = ( f )->ReadUint32();							\
	x = BYTESWAP32( x )									\

void ResourceContainer::Load_RAGE()
{
	int64  n = System_FileSize( fileHandle );
	time64 a = System_GetMicroseconds();

	// Header
	uint32 indexOffset = File_ReadUint32B( fileHandle );
	uint32 indexSize   = File_ReadUint32B( fileHandle );
	(void)indexSize;

	// this is where the data is
	dataOffset = File_Tell( fileHandle );

	// Index
	File_Seek( fileHandle, indexOffset );

	uint8 *index = (uint8 *)Memory_Malloc( indexSize );
	File_Read( fileHandle, index, indexSize );

	File_Memory indexFile( index, indexSize, FILE_READ );

	uint32 numFiles; READBIGUINT32( &indexFile, numFiles );

	files.Resize( numFiles );

	char temp[ 128 ];

	for ( uint32 i = 0; i < numFiles; i++ ) {
		ResourceFile &rf = files[ i ];
		rf.rc 			 = this;

		uint32 unknown03; READBIGUINT32( &indexFile, unknown03 );

		uint32 typeNameLen = indexFile.ReadUint32(); 
		rf.typeName.Resize( typeNameLen );
		indexFile.Read( rf.typeName.GetPtr(), typeNameLen );
		rf.typeName[ typeNameLen ] = '\0';

		uint32 srcNameLen = indexFile.ReadUint32(); 
		rf.srcName.Resize( srcNameLen );
		indexFile.Read( rf.srcName.GetPtr(), srcNameLen );
		rf.srcName[ srcNameLen ] = '\0';

		uint32 dstNameLen = indexFile.ReadUint32(); 
		rf.dstName.Resize( dstNameLen );
		indexFile.Read( rf.dstName.GetPtr(), dstNameLen );
		rf.dstName[ dstNameLen ] = '\0';

		READBIGUINT32( &indexFile, rf.dstOffset );
		READBIGUINT32( &indexFile, rf.srcSize );
		READBIGUINT32( &indexFile, rf.dstSize );

		// optional data, for idmsa files this is a list of languages
		// each entry is always 24 bytes
		uint32 numUnknown; READBIGUINT32( &indexFile, numUnknown );
		for ( uint32 j = 0; j < numUnknown; j++ ) {
			// (void)File_ReadUint32B( fileHandle ); // ???
			// (void)File_ReadUint32B( fileHandle ); // ???
			// (void)File_ReadUint32B( fileHandle ); // ???
			// (void)File_ReadUint32B( fileHandle ); // ???
			// (void)File_ReadUint32B( fileHandle ); // ???
			// (void)File_ReadUint32B( fileHandle ); // ???
			indexFile.Read( temp, 24 );
		}

		// some kind of checksum? seems to be the same for all files
		// (void)File_ReadUint32B( fileHandle ); // ??? always 0x00FFFFFF ( be )
		// (void)File_ReadUint32B( fileHandle ); // ??? always 0x00000000 ( be )
		// (void)File_ReadUint32B( fileHandle ); // ??? always 0x4318BEFD ( be ) in .resources, checksum?
		// (void)File_ReadUint32B( fileHandle ); // ??? always 0xFE070000 ( be ) in .resources, checksum?
		// (void)File_ReadUint32B( fileHandle ); // ??? always 0x00000000 ( be )
		indexFile.Read( temp, 20 );
	}

	Memory_Free( index );

	time64 b   = System_GetMicroseconds();
	float32 ms = ( (float32)b - a ) / 1000.0f;
	System_Info( "parsing took %f ms", ms );
	System_Info( "%f MB/ms", ( n / 1000000ll ) / ms );
	System_Info( "%lli MB", n / 1000000ll );
}

void ResourceContainer::Unpack()
{
	for ( int64 i = 0; i < files.GetNum(); i++ ) {
		files[ i ].Unpack();
	}
}
