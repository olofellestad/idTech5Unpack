#include "unpack.h"

#include <stdio.h>

static Unpack _unpack;
Unpack		 *unpack = &_unpack;

bool Unpack::Init( int argc, char **argv )
{
	if ( argc > 1 ) {
		rc = new ResourceContainer( argv[ 1 ] );
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
	printf( "%s\n", rc->name );
	printf( "====================\n" );
	for ( UINT32 i = 0; i < rc->numFiles; i++ ) {
		ResourceFile &rf = rc->files[ i ];

		if ( rf.dstSize != rf.srcSize ) {
			// continue; // skip compressed files
		}

		printf( "%-16s\t", rf.typeName );
		printf( "%-96s\t", rf.srcName );
		printf( "%-96s\t", rf.dstName );
		printf( "%-16u\t", rf.srcSize );
		printf( "%-16u\n", rf.dstSize );
	}

	printf( "====================\n" );
	printf( "total files: %u\n", rc->numFiles );
	printf( "====================\n" );
}
