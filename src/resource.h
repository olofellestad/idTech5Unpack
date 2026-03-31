#pragma once

class ResourceContainer;

class ResourceFile
{
	friend class ResourceContainer;
	friend class Unpack;

private:
	Str typeName;
	Str srcName;
	Str dstName;

	uint32 dstOffset;
	uint32 srcSize;
	uint32 dstSize;

	ResourceContainer *rc;
};

class ResourceContainer
{
	friend class Unpack;

public:
	ResourceContainer( const char *_fileName );
	~ResourceContainer();

	// void Load_DBFG();
	void Load_RAGE();
	// void Load_RES3();
	// void Load_RES5();

private:
	Str fileName;

	TList< ResourceFile > files;

	handle64 fileHandle;
	uint32   dataOffset;
};
