#include "lib/mem/memory.h"

#undef Memory_Malloc

#ifdef DEBUG_MEMORY
struct allocInfo_t : public TLink< allocInfo_t >
{
    const char *fileName;
    int         fileLine;
    int64         size;
};

static TLinkList< allocInfo_t > allocInfo;
static int64 currentMemory = 0;       // how much memory are we currently using?
static int64 maximumMemory = 0;       // how much memory did we use at most?
static int64 currentAllocs = 0;       // how many allocations are active?
static int64 maximumAllocs = 0;       // how many allocations have we made?

void *Memory_Malloc( int64 size, const char *fileName, int fileLine )
{
    if ( size <= 0 ) {
        return nullptr;
    }

    void *ptr = System_Malloc( size + sizeof( allocInfo_t ), 16 );
    
    allocInfo_t *m  = (allocInfo_t *)ptr;
    m->fileName     = fileName;
    m->fileLine     = fileLine;
    m->size         = size;
    
    allocInfo.Append( m );
/*
    System_Log( va( "Memory_Malloc: %s:%i: %p ( %lli )",
                    ( (allocInfo_t *)m )->fileName,
                    ( (allocInfo_t *)m )->fileLine,
                    ( (void *)m ),
                    ( (allocInfo_t *)m )->size ) );
*/
    currentMemory += m->size;
    maximumMemory = MAX( maximumMemory, currentMemory );

    ++currentAllocs;
	++maximumAllocs;
    
    return ( (uint8 *)ptr + sizeof( allocInfo_t ) );
}

void Memory_Free( void *ptr )
{
    if ( ptr == nullptr ) {
        return;
    }
    
    ptr = ( (uint8 *)ptr - sizeof( allocInfo_t ) );
    
    allocInfo_t *m = (allocInfo_t *)ptr;
    allocInfo.Remove( m );
    
    currentMemory -= m->size;

	--currentAllocs;
    
    System_Free( ptr );
}

int64 Memory_GetCurrentMemory()
{
    return currentMemory;
}

int64 Memory_GetMaximumMemory()
{
    return maximumMemory;
}

int64 Memory_GetCurrentAllocs()
{
    return currentAllocs;
}

int64 Memory_GetMaximumAllocs()
{
    return maximumAllocs;
}

void Memory_CheckLeaks()
{
    int64 leak = 0;
    
    for ( TLink< allocInfo_t > *m = allocInfo.GetHead(); m->next != allocInfo.GetHead(); m = m->next ) {
        System_Log( va( "Memory_CheckLeaks: %s:%i: %p ( %lli )",
                        ( (allocInfo_t *)m )->fileName,
                        ( (allocInfo_t *)m )->fileLine,
                        ( (void *)m ),
                        ( (allocInfo_t *)m )->size ) );
        leak += ( (allocInfo_t *)m )->size;
    }
    
    System_Log( va( "Memory_CheckLeaks: %lli bytes leaked", leak ) );
}
#endif
