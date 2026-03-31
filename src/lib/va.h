//--------------------------------------------------------------------------------------------------------------------------------
//  va.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

#include <cstdarg>
#include <cstdio>

inline char *va( const char *format, ... )
{
    static char string[ 4 ][ 16384 ];
    static int  i = 0;

    char *buf = string[ i ];
    i         = ( i + 1 ) & 3;

    va_list args;
    va_start( args, format );
    vsnprintf( buf, 16384, format, args );
    va_end( args );

    return buf;
}
