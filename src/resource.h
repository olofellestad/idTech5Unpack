#pragma once

// this uses more memory but we no longer allocate during file loading
#define USE_STATIC_STR

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

#ifdef USE_STATIC_STR
	TStaticStr< 255 > typeName;
	TStaticStr< 255 > srcName;
	TStaticStr< 255 > dstName;
#else
	Str typeName;
	Str srcName;
	Str dstName;
#endif

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
#ifdef USE_STATIC_STR
	TFlatMap< TStaticStr< 255 >, ResourceFile > files;
#else
	TFlatMap< Str, ResourceFile > files;
#endif

	Str fileName;

	handle64 fileHandle;
	uint32   dataOffset;
};
