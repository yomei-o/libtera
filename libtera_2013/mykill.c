/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/
 
#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif
 
#include<stdio.h>
#include<stdarg.h>

#ifdef _WIN32
#include<windows.h>
#endif

#ifdef unix
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#endif

#include "mykill.h"

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


static void in_mykill9(int pid)
{
#ifdef _WIN32
	HANDLE h;
	h=OpenProcess(PROCESS_TERMINATE,TRUE,pid);
	if(h)
	{
		TerminateProcess(h,0);
		CloseHandle(h);
	}
#endif
#ifdef unix
	kill(pid,SIGKILL);
#endif
}

void mykill(int pid,int n)
{
#if defined(_WIN32) && !defined(__CYGWIN__)
	void* ret;
	char buf[128];
	int wrsz;

	if(n==9){
		HANDLE h;
		h=OpenProcess(PROCESS_TERMINATE,TRUE,pid);
		if(h){
			TerminateProcess(h,0);
			CloseHandle(h);
		}
		return;
	}
	snprintf(buf,128,"\\\\.\\mailslot\\signal%d",pid);
	ret=CreateFile(buf, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(ret==INVALID_HANDLE_VALUE)return;
	buf[0]=n;
	WriteFile(ret, buf, 1, (DWORD*)&wrsz, NULL);
	CloseHandle(ret);
#endif
#ifdef unix
	kill(pid,n);
#endif
}

int mygetpid(void)
{
#if defined(_WIN32) && !defined(__CYGWIN__)
	return GetCurrentProcessId();
#endif
#ifdef unix
	return getpid();
#endif
	return -1;
}

void mymysleep(int s)
{
#if defined(_WIN32) && !defined(__CYGWIN__)
	Sleep(1000*s);
#endif
#ifdef unix
	sleep(s);
#endif
}



#if 0
int main()
{
	int pid;
	mysignal_init();
	printf("init()\n");
	pid=mygetpid();
	printf("pid=%d\n",pid);
	scanf("%d",&pid);
	printf("mykill(%d,%d)\n",pid,15);
	mykill(pid,15);
	printf("mymysleep(2)\n");
	mymysleep(2);
	printf("mymysleep(2)\n");
	mymysleep(2);
	printf("mymysleep(2)\n");

	mykill9(pid);
	mymysleep(2);
	printf("fini()\n");
	mysignal_fini();
}

#endif


