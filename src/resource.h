#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class ResourceContainer;

class ResourceFile
{
	friend class ResourceContainer;
	friend class Unpack;

public:
private:
	const char *typeName;
	const char *srcName;
	const char *dstName;

	UINT32 dstOffset;
	UINT32 srcSize;
	UINT32 dstSize;

	ResourceContainer *rc;
};

class ResourceContainer
{
	friend class Unpack;

public:
	ResourceContainer( const char *fileName );
	~ResourceContainer();

	// void Load_DBFG( UINT32 u );
	void Load_RAGE( UINT32 u );
	// void Load_RES3( UINT32 u );
	// void Load_RES5( UINT32 u );

private:
	const char	 *name;
	ResourceFile *files;

	HANDLE fileHandle;
	UINT32 dataOffset;
	UINT32 numFiles;
};
