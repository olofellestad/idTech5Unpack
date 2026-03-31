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

	// void Load_DBFG();
	void Load_RAGE();
	// void Load_RES3();
	// void Load_RES5();

private:
	const char	 *name;
	ResourceFile *files;

	HANDLE fileHandle;
	UINT32 dataOffset;
	UINT32 numFiles;
};
