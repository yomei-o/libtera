/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

//if not defined _WIN32_WINNT, trylock were undefined 
#ifdef _WIN32
#define _WIN32_WINNT 0x0400
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pmutex.h"
#ifdef unix
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

void pmutex_sleep(int s)
{
#ifdef	unix
	sleep(s);
#endif
#ifdef _WIN32
	Sleep(s*1000);
#endif
}

pmutex_t pmutex_create(void)
{
#ifdef unix	
	pthread_mutex_t* m;
	
	m=malloc(sizeof(pthread_mutex_t));
	if(m==NULL)return NULL;

	//*m=PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_init(m,NULL);
	return (void*)m;
#endif
#ifdef _WIN32
	LPCRITICAL_SECTION m;
	
	m=malloc(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection(m);
	return (void*)m;	
#endif
	return NULL;	
}
int pmutex_destroy(pmutex_t pm)
{
#ifdef unix	
	if(pm==NULL)return 0;
	pthread_mutex_destroy(pm);
	free(pm);
	return 1;
#endif
#ifdef _WIN32
	if(pm==NULL)return 0;
	DeleteCriticalSection(pm);
	free(pm);
	return 1;
#endif
	return 0;
}
int pmutex_lock(pmutex_t pm)
{
#ifdef unix	
	if(pm==NULL)return 0;
	pthread_mutex_lock(pm);
	return 1;
#endif
#ifdef _WIN32	
	if(pm==NULL)return 0;
	EnterCriticalSection(pm);
	return 1;
#endif
	return 0;
}
int pmutex_unlock(pmutex_t pm)
{
#ifdef unix
	if(pm==NULL)return 0;
	pthread_mutex_unlock(pm);
	return 1;
#endif
#ifdef _WIN32	
	if(pm==NULL)return 0;
	LeaveCriticalSection(pm);
	return 1;
#endif
	return 0;
}
int pmutex_trylock(pmutex_t pm)
{
#ifdef unix	
    int res;
    res=pthread_mutex_trylock(pm);
    return !res;
#endif
#ifdef _WIN32	
    int res;
	res=TryEnterCriticalSection(pm);
	return res;
#endif
	return 0;
}

#if 0

#include "thread.h"

static pmutex_t pm;

void f(void* vp)
{
	printf("f()\n");
	pmutex_lock(pm);
	printf("f() locked\n");
	pmutex_sleep(5);
	printf("f() unlocked\n");
	pmutex_unlock(pm);	
	printf("f() exit\n");
}
main()
{
	thread_t* th;
	pm=pmutex_create();
	pmutex_lock(pm);
	printf("locked\n");
	th=thread_create(f,NULL);
	pmutex_sleep(5);
	printf("unlocked\n");
	pmutex_unlock(pm);
	thread_join(th);
	pmutex_destroy(pm);
	printf("exit\n");
}

#endif

#if 0


#include "thread.h"

static pmutex_t pm;

void f(void* vp)
{
	int res;
	printf("f()\n");
	res=pmutex_trylock(pm);
	printf("f() trylock %d\n",res);
	if(res)pmutex_unlock(pm);
	pmutex_sleep(3);

	res=pmutex_trylock(pm);
	printf("f() trylock %d\n",res);
	if(res)pmutex_unlock(pm);
	pmutex_sleep(3);

	res=pmutex_trylock(pm);
	printf("f() trylock %d\n",res);
	if(res)pmutex_unlock(pm);
	pmutex_sleep(3);

	res=pmutex_trylock(pm);
	printf("f() trylock %d\n",res);
	if(res)pmutex_unlock(pm);
	pmutex_sleep(3);

	printf("f() exit\n");
}
main()
{
	thread_t* th;
	pm=pmutex_create();
	pmutex_lock(pm);
	printf("locked\n");
	th=thread_create(f,NULL);
	pmutex_sleep(5);
	printf("unlocked\n");
	pmutex_unlock(pm);
	thread_join(th);
	pmutex_destroy(pm);
	printf("exit\n");
}

#endif

