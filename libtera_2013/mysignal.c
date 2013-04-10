/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#include <stdio.h>
#include <signal.h>
#include <stdint.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#include <process.h>
#endif

#include "mysignal.h"

#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif

#if defined(_MSC_VER) && (_MSC_VER<1500)
static int snprintf(char * buf,size_t n,const char *format,...)
{
	int ret;
	va_list ap;
    va_start(ap, format);
    ret = vsprintf(buf,format, ap);
    va_end(ap);
	return ret;
}
#endif

static int signal_flag=0;
#if defined(_WIN32) && !defined(__CYGWIN__)
static int stop_flag=0;
static uintptr_t thread_id=0;
#endif



static void signal_func(int code){
	fprintf(stderr,"signalfunc() %d \n",code);
	signal_flag=1;
}


#if defined(_WIN32) && !defined(__CYGWIN__)

static unsigned __stdcall msg_thread(void* vp)
{
	char buf[128];
	int pid;
	int ret,readsz;
	void* h=NULL;
	int sign;
	pid=GetCurrentProcessId();
	//printf("pid=%d\n",pid);
	snprintf(buf,128,"\\\\.\\mailslot\\signal%d",pid);
	h=CreateMailslot( buf, 0, 1000, NULL);

	while(stop_flag==0){
		//printf("Sleeping...\n");
		ret=ReadFile(h, buf, sizeof(buf), (DWORD*)&readsz, NULL);
		if(ret==1){
			sign=buf[0]&127;
			//printf("raise(%d)\n",sign);
			raise(sign);
		}
		Sleep(1000);
	}
	if(h && h!=INVALID_HANDLE_VALUE ){
		CloseHandle(h);
	}
	return 0;
}
#endif

void mysignal_fini(void)
{
#if defined(_WIN32) && !defined(__CYGWIN__)
	stop_flag=1;
	if(thread_id){
		WaitForSingleObject((HANDLE)thread_id, INFINITE);
		CloseHandle((HANDLE)thread_id);
	}
	thread_id=0;
#endif
}
void mysignal_init(void)
{

#if defined(_WIN32) && !defined(__CYGWIN__)
	if(thread_id==0)
	{
		int id;
		thread_id=_beginthreadex(0,0,msg_thread,NULL,0,&id);
	}
#endif /* _WIN32 */
#ifdef unix
	signal( SIGPIPE , SIG_IGN ); 
	signal( SIGCHLD , SIG_IGN ); 
#endif

	signal( SIGINT , signal_func ); 
	signal( SIGTERM , signal_func ); 
	signal( SIGABRT , signal_func ); 
}

int mysignal_is_abort(void)
{
	return signal_flag;
}


mysighandler_t mysignal(int signum, mysighandler_t sighandler)
{
	return signal(signum,sighandler);
}

int myraise(int sig)
{
	return raise(sig);
}

