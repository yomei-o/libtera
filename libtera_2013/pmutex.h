/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/



#ifndef __PMUTEX_H__
#define __PMUTEX_H__



#ifdef unix
#include <pthread.h>
#include <string.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#include <process.h>
#endif


typedef void* pmutex_t;


#ifdef __cplusplus
extern "C"
{
#endif

pmutex_t pmutex_create(void);
int pmutex_destroy(pmutex_t pm);
int pmutex_lock(pmutex_t pm);
int pmutex_unlock(pmutex_t pm);
int pmutex_trylock(pmutex_t pm);
void pmutex_sleep(int s);


#ifdef __cplusplus
}
#endif

#endif /* __PMUTEX_H__ */
