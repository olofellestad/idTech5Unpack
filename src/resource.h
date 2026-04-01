#pragma once

class ResourceContainer;

class ResourceFile
{
	friend class ResourceContainer;
	friend class Unpack;

public:
	ResourceFile( ResourceContainer *_rc );
	ResourceFile( ResourceFile &&other ) noexcept;
	ResourceFile( const ResourceFile &other );

	~ResourceFile() = default;
	
	void Unpack();

	ResourceFile &operator=( ResourceFile &&other ) noexcept;
	ResourceFile &operator=( const ResourceFile &other );

private:
	ResourceContainer *rc;

	Str typeName;
	Str srcName;
	Str dstName;

	uint32 dstOffset;
	uint32 srcSize;
	uint32 dstSize;
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

	void Unpack();

private:
	TFlatMap< Str, ResourceFile > files;

	Str fileName;

	handle64 fileHandle;
	uint32   dataOffset;
};
