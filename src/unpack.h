#pragma once

#include "resource.h"

class Unpack
{
public:
	bool Init( int argc, char **argv );
	void Shutdown();

	void Run();

private:
	ResourceContainer *rc;
};

extern Unpack *unpack;