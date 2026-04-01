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

	unpack->Run();
	unpack->Shutdown();

	System_Log( va( "current alloc: %lli\n"
					"maximum alloc: %lli\n"
					"num allocs:    %lli",
					Memory_GetCurrentAlloc(), 
					Memory_GetMaximumAlloc(),
					Memory_GetNumAllocs() ) );

	Memory_CheckLeaks();

	return 0;
}
