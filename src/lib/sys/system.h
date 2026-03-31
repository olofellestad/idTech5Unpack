//--------------------------------------------------------------------------------------------------------------------------------
//  system.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

#define FILE_READ  BIT( 1 )
#define FILE_WRITE BIT( 2 )

#define FILE_HEAD  -1
#define FILE_CURR   0
#define FILE_TAIL  +1

#define LEN_HOST_NAME	256
#define LEN_USER_NAME   256
#define LEN_FILE_PATH   1024
#define LEN_FILE_NAME   256

struct dateTime_t
{
	struct { int y, m, d; } date;
	struct { int h, m, s; } time;
};

bool System_FileOpen( handle64 *h, const char *fileName, int fileFlags );
void System_FileClose( handle64 &h );

int64 System_FileRead( handle64 h, void *buffer, int64 size );
int64 System_FileWrite( handle64 h, const void *buffer, int64 size );

int64 System_FileSeek( handle64 h, int64 position, int where );
int64 System_FileSize( handle64 h );

time64 System_FileTime( handle64 h );

const char *System_GetFilePath( const char *fileName );

bool System_Mkdir( const char *dirName );

bool System_FileExists( const char *fileName );
bool System_DirExists( const char *dirName );

const char *System_GetUserDir( const char *org, const char *app );
const char *System_GetRootDir();

void System_Log( const char *message );

const char *System_GetHostName();
const char *System_GetUserName();

int64 System_GetMilliseconds();
int64 System_GetMicroseconds();
time64 System_GetTime();

bool System_TimeToDateTime( time64 t, dateTime_t *dt );

bool System_GetDateTime( dateTime_t *dt );
bool System_GetBuildDateTime( dateTime_t *dt );

// TODO: threading stuff?

void *System_Malloc( int64 size, int64 align );
void  System_Free( void *ptr );

void System_Fail( const char *message );
void System_Quit( int code );
