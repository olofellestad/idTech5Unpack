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
