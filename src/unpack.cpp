#include "unpack.h"

#include "file/file.h"

#include <stdio.h>

// #include <miniz_tinfl.h>

static Unpack _unpack;
Unpack		 *unpack = &_unpack;

bool Unpack::Init( int argc, char **argv )
{
	if ( argc > 1 ) {
		rc = new ResourceContainer( argv[ 1 ] );
/*
		if ( argc > 2 ) {
			for ( UINT32 i = 0; i < rc->numFiles; i++ ) {
				ResourceFile &rf = rc->files[ i ];

				if ( strcmp( rf.dstName, argv[ 2 ] ) != 0 ) {
					continue;
				}

				char fileName[ 256 ];

				char *s = strrchr( (char *)rc->name, '\\' );
				char *z = strrchr( (char *)rc->name, '.' );

				snprintf( fileName, 256, "%.*s/", (int)( z - s - 1 ), s + 1 );
				strcat( fileName, rf.dstName );

				// printf( "'%s'\n", fileName );

				char *p = fileName;
				char *q = strchr( p, '/' );
				do {
					char dirName[ 256 ];
					strncpy( dirName, p, (size_t)( q - p ) );
					dirName[ q - p ] = '\0';

					CreateDirectoryA( dirName, nullptr );

					// printf( "%s\n", dirName );

					q = strchr( q + 1, '/' );
				} while ( q != nullptr );

				HANDLE fileHandle = CreateFileA( p, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
												 FILE_ATTRIBUTE_NORMAL, nullptr );
				UINT8 *buffer	  = new UINT8[ rf.dstSize ];
				DWORD  n		  = 0;

				SetFilePointer( rc->fileHandle, rf.dstOffset, nullptr, FILE_BEGIN );
				if ( ReadFile( rc->fileHandle, buffer, rf.dstSize, &n, nullptr ) ) {
					WriteFile( fileHandle, buffer, n, &n, nullptr );
				}

				delete[] buffer;

				CloseHandle( fileHandle );
				break;
			}
		}
*/
	}
	else {
		return false;
	}

	return true;
}

void Unpack::Shutdown()
{
	delete rc;
}

void Unpack::Run()
{
	printf( "====================\n" );
	printf( "%s\n", rc->fileName.GetPtr() );
	printf( "====================\n" );
	for ( int64 i = 0; i < rc->files.GetNum(); i++ ) {
		ResourceFile &rf = rc->files[ i ];
#if 0
		if ( rf.dstSize != rf.srcSize ) {
			if ( Str::ICompare( rf.dstName.GetPtr(), "script/script_events.script" ) != 0 ) {
				continue;
			}

			printf( "%-16s\t", rf.typeName.GetPtr() );
			printf( "%-96s\t", rf.srcName.GetPtr() );
			printf( "%-96s\t", rf.dstName.GetPtr() );
			printf( "%-16u\t", rf.dstOffset );
			printf( "%-16u\t", rf.srcSize );
			printf( "%-16u\n", rf.dstSize );

			handle64 h = INVALID_HANDLE64;

			char *p = strrchr( rf.dstName.GetPtr(), '/' );

			if ( System_FileOpen( &h, p + 1, FILE_WRITE ) ) {
				void *buffer = Memory_Malloc( rf.dstSize );

				System_FileSeek( rc->fileHandle, rf.dstOffset, FILE_HEAD );
				System_FileRead( rc->fileHandle, buffer, rf.dstSize );

				// deflate
				{
					void *uncomp = Memory_Malloc( rf.srcSize );
					
					tinfl_decompress_mem_to_mem( uncomp, rf.srcSize, buffer, rf.dstSize, 0 );

					System_FileWrite( h, uncomp, rf.dstSize );
					Memory_Free( uncomp );
				}

				// System_FileWrite( h, buffer, rf.dstSize );
				System_FileClose( h );

				Memory_Free( buffer );
			}

			break;

			// continue; // skip compressed files
		}
#endif
		printf( "%-16s\t", rf.typeName.GetPtr() );
		printf( "%-96s\t", rf.srcName.GetPtr() );
		printf( "%-96s\t", rf.dstName.GetPtr() );
		printf( "%-16u\t", rf.srcSize );
		printf( "%-16u\n", rf.dstSize );
	}

	printf( "====================\n" );
	printf( "total files: %lli\n", rc->files.GetNum() );
	printf( "====================\n" );
}
