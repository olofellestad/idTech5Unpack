#include "lib/sys/system.h"

// #include <SDL3/SDL_filesystem.h>
// #include <SDL3/SDL_log.h>

#include <CoreFoundation/CoreFoundation.h>

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define MACOS_LEN_ERROR_MESSAGE 128

static const char *MACOS_GetErrorMessage()
{
    static char message[ MACOS_LEN_ERROR_MESSAGE ];

    strerror_r( errno, message, MACOS_LEN_ERROR_MESSAGE );
    return message;
}

static time64 MACOS_TimespecTotime64( struct timespec ts )
{
    return (time64)ts.tv_sec * 1000000LL + ts.tv_nsec / 1000;
}

//--------------------------------------------------------------------------------------------------------------------------------
const char *System_FilePath( const char *fileName )
{
    ASSERT( LEN_FILE_PATH >= PATH_MAX );
    static char filePath[ PATH_MAX ];
    
/*
    const char *p = fileName;
    char *q       = filePath;

    while ( *p && ( ( q - filePath ) < LEN_FILE_PATH ) ) {
        switch ( *p ) {
        case '\\':  *( q++ ) = '/'; p++; break;
        default:    *( q++ ) = *( p++ ); break;
        }
    }
*/
    if ( realpath( fileName, filePath ) != nullptr ) {
        return filePath;
    }

    System_Info( "ERR: System_FilePath: %s", MACOS_GetErrorMessage() );
    return fileName;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"

bool System_FileOpen( handle64 *h, const char *fileName, int fileFlags )
{
    int flags = HAS_BIT( fileFlags, FILE_READ )
        ? ( HAS_BIT( fileFlags, FILE_WRITE )
            ? O_RDWR
            : O_RDONLY )
        : O_WRONLY | O_CREAT;
    
    *h = (handle64)open( fileName, flags, S_IRWXU );
    if ( *(int *)h >= 0 ) {
        return true;
    }
    
    System_Info( "ERR: System_FileOpen: %s", MACOS_GetErrorMessage() );
    return false;
}

void System_FileClose( handle64 &h )
{
    // make sure we don't lose any information
    ASSERT( ( (uint64)h ) == ( (uint64)*(int *)&h ) );
    
    if ( close( *(int *)&h ) >= 0 ) {
        h = INVALID_HANDLE64;
    }
}

int64 System_FileRead( handle64 h, void *buffer, int64 size )
{
    size_t  bytesToRead = (size_t)size;
    ssize_t bytesRead = read( *(int *)&h, buffer, bytesToRead );
    
    if ( bytesRead >= 0 ) {
        return (int64)bytesRead;
    }

    System_Info( "ERR: System_FileRead: %s", MACOS_GetErrorMessage() );
    return INVALID_INT64;
}

int64 System_FileWrite( handle64 h, const void *buffer, int64 size )
{
    size_t  bytesToWrite = (size_t)size;
    ssize_t bytesWritten = write( *(int *)&h, buffer, bytesToWrite );
    
    if ( bytesWritten >= 0 ) {
        return (int64)bytesWritten;
    }

    System_Info( "ERR: System_FileWrite: %s", MACOS_GetErrorMessage() );
    return INVALID_INT64;
}
 
int64 System_FileSeek( handle64 h, int64 position, int where )
{
    static const int seek[] = { SEEK_SET, SEEK_CUR, SEEK_END };
    
    int whence     = seek[ ( where + 1 ) % 3 ];
    off_t distance = (off_t)position;
    off_t offset   = lseek( *(int *)&h, distance, whence );
    
    if ( offset >= 0 ) {
        return (int64)offset;
    }
    
    System_Info( "ERR: System_FileSeek: %s", MACOS_GetErrorMessage() );
    return INVALID_INT64;
}

int64 System_FileSize( handle64 h )
{
    struct stat st = {};
    
    if ( fstat( *(int *)&h, &st ) >= 0 ) {
        return (int64)st.st_size;
    }
    
    System_Info( "ERR: System_FileSize: %s", MACOS_GetErrorMessage() );
    return INVALID_INT64;
}

time64 System_FileTime( handle64 h )
{
    struct stat st = {};
    
    if ( fstat( *(int *)&h, &st ) >= 0 ) {
        return (int64)st.st_mtime * 1000000LL;
    }
    
    System_Info( "ERR: System_FileSize: %s", MACOS_GetErrorMessage() );
    return INVALID_INT64;
}

bool System_Mkdir( const char *dirName )
{
    if ( mkdir( dirName, S_IRWXU ) >= 0 ) {
        return true;
    }
    
    System_Info( "ERR: System_Mkdir: %s", MACOS_GetErrorMessage() );
    return false;
}

bool System_FileExists( const char *fileName )
{
    struct stat st = {};
    
    return stat( fileName, &st ) == 0
        && S_ISREG( st.st_mode );
}

bool System_DirExists( const char *dirName )
{
    struct stat st = {};
    
    return stat( dirName, &st ) == 0
        && S_ISDIR( st.st_mode );
}
/*
const char *System_GetUserDir( const char *org, const char *app ) {}

const char *System_GetRootDir() {}
*/
#pragma clang diagnostic pop
//--------------------------------------------------------------------------------------------------------------------------------
const char *System_GetHostName()
{
    ASSERT( LEN_USER_NAME >= _POSIX_HOST_NAME_MAX );
    static char hostName[ LEN_HOST_NAME ];
    
    if ( gethostname( hostName, LEN_HOST_NAME ) != 0 ) {
        strncpy( hostName, "host", 5 );
    }
    
    return hostName;
}

const char *System_GetUserName()
{
    ASSERT( LEN_USER_NAME >= _POSIX_LOGIN_NAME_MAX );
    static char userName[ LEN_USER_NAME ];
    
    if ( getlogin_r( userName, LEN_USER_NAME ) != 0 ) {
        strncpy( userName, "user", 5 );
    }
    
    return userName;
}

//--------------------------------------------------------------------------------------------------------------------------------
void *System_Malloc( int64 size, int64 align )
{
    return aligned_alloc( align, ALIGN( size, align ) );
}

void System_Free( void *ptr )
{
    free( ptr );
}

//--------------------------------------------------------------------------------------------------------------------------------
int64 System_GetMilliseconds()
{
    return System_GetMicroseconds() / 1000LL;
}

int64 System_GetMicroseconds()
{
    struct timespec ts = {};
    clock_gettime( CLOCK_MONOTONIC, &ts );
    
    static int64 timeBase = MACOS_TimespecTotime64( ts );
    int64 timeNow         = MACOS_TimespecTotime64( ts );
    return timeNow - timeBase;
}

time64 System_GetTime()
{
    struct timespec ts = {};
    clock_gettime( CLOCK_REALTIME, &ts );
    
    return MACOS_TimespecTotime64( ts );
}

bool System_TimeToDateTime( time64 t, dateTime_t *dt )
{
    time_t _t = t / 1000000ll;

    struct tm tm = {};
    if ( localtime_r( &_t, &tm ) == nullptr ) {
        return false;
    }

    dt->date.y = tm.tm_year + 1900;
    dt->date.m = tm.tm_mon;
    dt->date.d = tm.tm_mday;
    dt->time.h = tm.tm_hour;
    dt->time.m = tm.tm_min;
    dt->time.s = tm.tm_sec;
    return true;
}

bool System_GetDateTime( dateTime_t *dt )
{
    return System_TimeToDateTime( System_GetTime(), dt );
}

bool System_GetBuildDateTime( dateTime_t *dt )
{
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";

    dt->date.y = ( __DATE__[ 0x7 ] - '0' ) * 1000 + ( __DATE__[ 0x8 ] - '0' ) * 100
               + ( __DATE__[ 0x9 ] - '0' ) * 10   + ( __DATE__[ 0xA ] - '0' );
    dt->date.d = ( __DATE__[ 0x4 ] - '0' ) * 10   + ( __DATE__[ 0x5 ] - '0' );

    while ( strncmp( __DATE__, months + dt->date.m * 3, 3 ) != 0 ) {
        if ( ++dt->date.m >= 12 ) return false;
    }

    dt->time.h = ( __TIME__[ 0x0 ] - '0' ) * 10 + ( __TIME__[ 0x1 ] - '0' );
    dt->time.m = ( __TIME__[ 0x3 ] - '0' ) * 10 + ( __TIME__[ 0x4 ] - '0' );
    dt->time.s = ( __TIME__[ 0x6 ] - '0' ) * 10 + ( __TIME__[ 0x7 ] - '0' );
    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
void System_Info( const char *_message, ... )
{
	static char message[ LEN_MESSAGE ];

	va_list args;
    va_start( args, _message );
    vsnprintf( message, LEN_MESSAGE, _message, args );
    va_end( args );

	fprintf( stdout, "%s\n", message );
}

void System_Warn( const char *_message, ... )
{
	static char message[ LEN_MESSAGE ];

	va_list args;
    va_start( args, _message );
    vsnprintf( message, LEN_MESSAGE, _message, args );
    va_end( args );

    CFStringRef msg = CFStringCreateWithCString( NULL, message, kCFStringEncodingUTF8 );
    CFUserNotificationDisplayAlert( 0, kCFUserNotificationCautionAlertLevel, NULL, NULL, NULL,
                                    CFSTR( "Warning" ), msg, NULL, NULL, NULL, NULL );
}

void System_Fail( const char *_message, ... )
{
	static char message[ LEN_MESSAGE ];

	va_list args;
    va_start( args, _message );
    vsnprintf( message, LEN_MESSAGE, _message, args );
    va_end( args );

    CFStringRef msg = CFStringCreateWithCString( NULL, message, kCFStringEncodingUTF8 );
    CFUserNotificationDisplayAlert( 0, kCFUserNotificationStopAlertLevel, NULL, NULL, NULL,
                                    CFSTR( "Fatal" ), msg, NULL, NULL, NULL, NULL );
    System_Quit( 1 );
}
