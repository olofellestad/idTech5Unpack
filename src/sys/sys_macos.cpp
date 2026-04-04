#include "unpack.h"

void System_Quit( int code )
{
    exit( code );
}

struct Foo{Foo(){printf("Foo!\n");}~Foo(){printf("~Foo!\n");}};

int main( int argc, char **argv )
{
	if ( !unpack->Init( argc, argv ) ) {
		return 1;
	}

	// unpack->Run();
	unpack->Shutdown();

	System_Info( "current memory: %lli\n"
				 "maximum memory: %lli\n"
			 	 "current allocs: %lli\n"
				 "maximum allocs: %lli",
				 Memory_GetCurrentMemory(), 
				 Memory_GetMaximumMemory(),
				 Memory_GetCurrentAllocs(), 
				 Memory_GetMaximumAllocs() );
	Memory_CheckLeaks();

	return 0;
}
