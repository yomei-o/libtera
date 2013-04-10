/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#ifndef _SPAWN_H_MY_
#define _SPAWN_H_MY_

#include <stdint.h>

#define MYP_WAIT 0
#define MYP_NOWAIT 1
#define MYP_OVERLAY  2
#define MYP_NOWAITO 3


#ifdef __cplusplus
extern "C"
{
#endif

int mywait(intptr_t pid);
int mynwait(intptr_t pid,int* retcode);

int  handle_2_pid(intptr_t hProcess);


intptr_t myspawnl( int mode, const char *cmdname, const char *arg0, ... );
intptr_t myspawnv( int mode, const char *cmdname, const char *const *argv );
intptr_t myspawnle( int mode, const char *cmdname, const char *arg0, ...);
intptr_t myspawnve( int mode, const char *cmdname, const char *const *argv, const char *const *envp );
intptr_t myspawnlp( int mode, const char *cmdname, const char *arg0, ...);
intptr_t myspawnvp( int mode, const char *cmdname, const char *const *argv );
intptr_t myspawnlpe( int mode, const char *cmdname, const char *arg0, ...);
intptr_t myspawnvpe( int mode, const char *cmdname, const char *const *argv,   const char *const *envp);
/* Ç±ÇÃä÷êîÇÕLINUXÇ…ÇÕÇ»Ç¢ */


#ifdef __cplusplus
}
#endif



#endif /* _SPAWN_H_MY_ */




