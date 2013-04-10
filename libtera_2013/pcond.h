/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/
#ifndef __PCOND_H__
#define __PCOND_H__



#ifdef unix
#include <pthread.h>
#include <string.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#include <process.h>
#endif

#include "pmutex.h"



typedef void* pcond_t;


#ifdef __cplusplus
extern "C"
{
#endif

pcond_t pcond_init(void);
int pcond_destroy(pcond_t pc);

int pcond_wait(pcond_t pc,pmutex_t pm);
int pcond_signal(pmutex_t pc);
int pcond_broadcast(pmutex_t pc);

void pcond_sleep(int s);


#ifdef __cplusplus
}
#endif

#endif /* __PCOND_H__ */
