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
static int64 currentAlloc = 0;        // how much memory are we currently using?
static int64 maximumAlloc = 0;        // how much memory did we use at most?

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
    currentAlloc += m->size;
    maximumAlloc = MAX( maximumAlloc, currentAlloc );
    
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
    
    currentAlloc -= m->size;
    
    System_Free( ptr );
}

int64 Memory_GetCurrentAlloc()
{
    return currentAlloc;
}

int64 Memory_GetMaximumAlloc()
{
    return maximumAlloc;
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
