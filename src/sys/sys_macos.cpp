#include "unpack.h"

void System_Quit( int code )
{
    exit( code );
}

struct Foo{Foo(){printf("Foo!\n");}~Foo(){printf("~Foo!\n");}};

int main( int argc, char **argv )
{
	// if ( !unpack->Init( argc, argv ) ) {
		// return 1;
	// }

	// unpack->Run();
	// unpack->Shutdown();
	
	UNUSED( argc );
	UNUSED( argv );
#if 0
	{
		TMap< Str, int > imap;
		imap.Insert( "ABCD", 5 );

		int *i = imap.Find( "ABCD" );
		if ( i ) {
			printf( "i = %i\n", *i );
		}
	}
	{
		Foo foo;

		TMap< Str, Foo > fmap;
		fmap.Insert( "foo.1", foo );

		Foo *f = fmap.Find( "foo.1" );
		printf( "%p\n", (void *)f );
	}
#endif
	{
		TList< Str > slist;
		slist.Append( "Hello, world!" );

		printf( "%s\n", slist[ 0 ].GetPtr() );

		slist.Resize( 32 );
	}

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
