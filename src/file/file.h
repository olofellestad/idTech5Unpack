#pragma once

bool File_Recurse( handle64 *h, const char *fileName );

uint32 File_ReadUint32B( handle64 h );
uint32 File_ReadUint32L( handle64 h );
int64  File_Read( handle64 h, void *buffer, int64 size );

int64 File_Seek( handle64 h, int64 offset );
int64 File_Tell( handle64 h );
