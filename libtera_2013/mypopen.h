/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/
 
#ifndef _MYPOPEN_H_
#define _MYPOPEN_H_

#include <stdio.h> 

#ifdef _WIN32
#include <windows.h> 
#include <io.h> 
#include <fcntl.h>
#include <process.h>
#endif

#ifdef unix
#endif


#if defined(_WINDOWS) || defined(_USRDLL)
FILE* mypopen(char* cmd,char* mode);
int mypclose( FILE *stream );
#else
#ifdef _WIN32
#define mypopen _popen
#define mypclose _pclose
#else
#define mypopen popen
#define mypclose pclose
#endif
#endif /* _WINDOWS */


#ifdef unix
#define PIPEREADMODE "r"
#endif /* unix */

#ifdef _WIN32
#define PIPEREADMODE "rb"
#endif /* _WIN32 */


#endif /* _MYPOPEN_H_ */



