/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/
 
#ifndef _FASTPOPEN_H_
#define _FASTPOPEN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef unix	
#define PIPEREADMODE "r"
#endif /* unix */

#ifdef _WIN32
#define PIPEREADMODE "rb"
#endif /* _WIN32 */

FILE *fastpopen( char *cmd, char *mode );
void fastpclose( FILE *fp );
void fastpabort( FILE *fp );
void fastpkill( FILE *fp );

#ifdef __cplusplus
}
#endif


#endif /* _FASTPOPEN_H_ */


