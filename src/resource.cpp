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

	const char *srcExt = Str::Find( srcName.GetPtr(), '.' );
	const char *dstExt = Str::Find( dstName.GetPtr(), '.' );

	// TODO: DEFLATE compression
	if ( IsCompressed() && Str::ICompare( srcExt, dstExt ) != 0 ) {
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

void ResourceContainer::Load_RAGE()
{
	// Header
	uint32 indexOffset = File_ReadUint32B( fileHandle );
	uint32 indexSize   = File_ReadUint32B( fileHandle );
	(void)indexSize;

	// this is where the data is
	dataOffset = File_Tell( fileHandle );

	// Index
	File_Seek( fileHandle, indexOffset );

	uint32 numFiles = File_ReadUint32B( fileHandle );

	// files.Resize( numFiles );
	files.Reserve( numFiles );

	for ( uint32 i = 0; i < numFiles; i++ ) {
		ResourceFile rf( this );

		uint32 unknown03 = File_ReadUint32B( fileHandle ); // flags?
		(void)unknown03;

		uint32 typeNameLen = File_ReadUint32L( fileHandle );
		rf.typeName.Resize( typeNameLen );
		File_Read( fileHandle, rf.typeName.GetPtr(), typeNameLen );
		rf.typeName[ typeNameLen ] = '\0';

		uint32 srcNameLen = File_ReadUint32L( fileHandle );
		rf.srcName.Resize( srcNameLen );
		File_Read( fileHandle, rf.srcName.GetPtr(), srcNameLen );
		rf.srcName[ srcNameLen ] = '\0';

		uint32 dstNameLen = File_ReadUint32L( fileHandle );
		rf.dstName.Resize( dstNameLen );
		File_Read( fileHandle, rf.dstName.GetPtr(), dstNameLen );
		rf.dstName[ dstNameLen ] = '\0';

		rf.dstOffset = File_ReadUint32B( fileHandle );
		rf.srcSize	 = File_ReadUint32B( fileHandle );
		rf.dstSize	 = File_ReadUint32B( fileHandle );

		// optional data, for idmsa files this is a list of languages
		// each entry is always 24 bytes
		uint32 numUnknown = File_ReadUint32B( fileHandle );
		for ( uint32 j = 0; j < numUnknown; j++ ) {
			(void)File_ReadUint32B( fileHandle ); // ???
			(void)File_ReadUint32B( fileHandle ); // ???
			(void)File_ReadUint32B( fileHandle ); // ???
			(void)File_ReadUint32B( fileHandle ); // ???
			(void)File_ReadUint32B( fileHandle ); // ???
			(void)File_ReadUint32B( fileHandle ); // ???
		}

		// some kind of checksum? seems to be the same for all files
		(void)File_ReadUint32B( fileHandle ); // ??? always 0x00FFFFFF ( be )
		(void)File_ReadUint32B( fileHandle ); // ??? always 0x00000000 ( be )
		(void)File_ReadUint32B( fileHandle ); // ??? always 0x4318BEFD ( be ) in .resources, checksum?
		(void)File_ReadUint32B( fileHandle ); // ??? always 0xFE070000 ( be ) in .resources, checksum?
		(void)File_ReadUint32B( fileHandle ); // ??? always 0x00000000 ( be )
		
		files.Insert( rf.dstName, std::move( rf ) );
	}
}

void ResourceContainer::Unpack()
{
	for ( int64 i = 0; i < files.GetNum(); i++ ) {
		files[ i ].Unpack();
	}
}
