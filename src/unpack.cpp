#include "unpack.h"

#include <stdio.h>

static Unpack _unpack;
Unpack		 *unpack = &_unpack;

bool Unpack::Init( int argc, char **argv )
{
	if ( argc > 1 ) {
		rc = new ResourceContainer( argv[ 1 ] );
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
