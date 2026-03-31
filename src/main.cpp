#include "unpack.h"

int main( int argc, char **argv )
{
	if ( !unpack->Init( argc, argv ) ) {
		return 1;
	}

	unpack->Run();
	unpack->Shutdown();

	return 0;
}