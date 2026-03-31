#include "unpack.h"

void System_Quit( int code )
{
    exit( code );
}

int main( int argc, char **argv )
{
	if ( !unpack->Init( argc, argv ) ) {
		return 1;
	}

	// unpack->Run();
	unpack->Shutdown();

	Memory_CheckLeaks();
	return 0;
}
