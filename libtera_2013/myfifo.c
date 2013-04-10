/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(unix) && !defined(__CYGWIN__)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
#endif

#include "myfifo.h"


#define TIMEOUT 1

#if defined(unix) && !defined(__CYGWIN__)
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#endif

static int mymkname(char* buf,int sz,char* name)
{
#if defined(unix) && !defined(__CYGWIN__)
	if(name==NULL || buf==NULL || sz<1)return -1;
	buf[0]=0;
	if(strlen(name)>sz+32)return -1;
	
	strncpy(buf,"/dev/shm/",sz-1);
	strncat(buf,name,sz-1);
	buf[sz-1]=0;
	//printf("mkname name=>>%s<<\n",buf);
#else
	if(name==NULL || buf==NULL || sz<1)return -1;
	buf[0]=0;
	if((int)strlen(name)>sz+32)return -1;
	
	strncpy(buf,"\\\\.\\mailslot\\",sz-1);
	strncat(buf,name,sz-1);
	buf[sz-1]=0;
	//printf("mkname name=>>%s<<\n",buf);
#endif
	return 0;
}


#if defined(unix) && !defined(__CYGWIN__)
static int mymkfifo(char* name)
{
	int ret=0;
	char tmp[1024];

	if(name==NULL)return -1;
	if(strlen(name)>200)return -1;
	mymkname(tmp,sizeof(tmp),name);
	ret=mkfifo(tmp,FILE_MODE);
	//printf("mymkfifo ret=%d\n",ret);
	return ret;
}
#endif


#if defined(unix) && !defined(__CYGWIN__)
static int waitfd(int s,int timeout){
	fd_set readfds;
	struct timeval tv;
	int n;

	FD_ZERO(&readfds);
	FD_SET(s, &readfds);

	memset(&tv,0,sizeof(tv));
	tv.tv_sec=timeout/1000;
	tv.tv_usec=(timeout%1000)*1000;

	n = select(s+1,&readfds,NULL, NULL, &tv);
	//timeout
	if(n==0)return 0;
	//active
	if(n>0)return 1;
	//error
	return -1;
}
#endif


void* myfifo_open_read(char* name)
{
#if defined(unix) && !defined(__CYGWIN__)
	int ret=-1;
	char tmp[1024];
	if(name==NULL)return NULL;
	mymkfifo(name);
	mymkname(tmp,sizeof(tmp),name);
	//printf("open name=%s\n",tmp);
	ret=open(tmp,O_RDONLY | O_NONBLOCK,FILE_MODE);
	//printf("open ret=%d\n",ret);
	if(ret==-1)return NULL;
	return (void*)ret;
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
	void* ret;
	char tmp[1024];
	if(name==NULL)return NULL;
	mymkname(tmp,sizeof(tmp),name);
	ret=CreateMailslot( tmp, 0, 1000*TIMEOUT, NULL);
	if(ret==INVALID_HANDLE_VALUE)return NULL;
	return ret;
#endif
	return NULL;
}

void* myfifo_open_write(char* name)
{
#if defined(unix) && !defined(__CYGWIN__)
	int ret=-1;
	int f;
	char tmp[1024];
	if(name==NULL)return NULL;
	mymkfifo(name);
	mymkname(tmp,sizeof(tmp),name);
	//printf("open name=%s\n",tmp);

	signal(SIGPIPE,SIG_IGN);

	ret=open(tmp,O_WRONLY | O_NONBLOCK,FILE_MODE);
	//printf("open ret=%d\n",ret);
	f=fcntl(ret,F_GETFL,0);
	if(f!=-1 && (f&O_NONBLOCK)){
		//printf("##### set non block !!!!");
		fcntl(ret,F_SETFL, f & (~O_NONBLOCK));
	}
	if(ret==-1)return NULL;
	return (void*)ret;
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
	void* ret;
	char tmp[1024];
	if(name==NULL)return NULL;
	mymkname(tmp,sizeof(tmp),name);
	ret=CreateFile(tmp, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(ret==INVALID_HANDLE_VALUE)return NULL;
	return ret;
#endif
	return NULL;
}

void myfifo_close(void* vp)
{
#if defined(unix) && !defined(__CYGWIN__)
	if(vp==NULL)return;
	close((int)vp);	
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
	if(vp==NULL)return;
	CloseHandle(vp);
#endif
}
 

int myfifo_write(void* vp,void* buf,int sz)
{
#if defined(unix) && !defined(__CYGWIN__)
	int ret;
	//if(sz>424)return -1;
	ret=write((int)vp,buf,sz);
	//printf("myfifo_write ret=%d\n",ret);
	return ret;
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
	int ret,wrsz;
	if(vp==NULL)return -1;
	//if(sz>424)return -1;
	ret=WriteFile(vp, buf, sz, (DWORD*)&wrsz, NULL);
	if(ret==0)return -1;
	return wrsz;
#endif
	return -1;
}

int myfifo_read(void* vp,void* buf,int sz)
{
#if defined(unix) && !defined(__CYGWIN__)
	int ret,r;
	r=waitfd((int)vp,1000*TIMEOUT);
	//  0 timeout
	// -1 error
	if(r<=0)return r;

	ret=read((int)vp,buf,sz);
	if(r>0 && ret==0){
		//closed write fd
		myfifo_sleep(1);
	}
	//printf("myfifo_read ret=%d\n",ret);
	return ret;
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
	int ret,readsz;
	if(vp==NULL)return -1;
	ret=ReadFile(vp, buf, sz, (DWORD*)&readsz, NULL);
	if(ret==0){
		if(GetLastError()==ERROR_SEM_TIMEOUT)return 0;
		return -1;
	}
	return readsz;
#endif

	return -1;
}



void myfifo_sleep(int s)
{
#if defined(unix) && !defined(__CYGWIN__)
	sleep(s);
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
	Sleep(s*1000);
#endif
}


#if 0

void dowrite(int tmode)
{
	void* h;
	char* buf="0123456789";
	int ret;
	int i;

	if(tmode)buf="abcdefghijklmn";

	printf("mtfifo write mode!\n");
	h=myfifo_open_write("ass");
	if(h==NULL){
		printf("fifo write open NG!\n");
		return;
	}

	printf("fifo write open OK!\n");

	for(i=0;i<12000;i++){
		printf("write\n");
		ret=myfifo_write(h,buf,10);
		printf("write(%d) ret=%d\n",i,ret);
		//myfifo_sleep(1);
	}
	printf("fifo write close\n");
	myfifo_close(h);

	printf("fifo write end\n");
}


void doread(void)
{
	void* h;
	int ret;
	char buf[256];
	int i;

	printf("mtfifo read mode!\n");
	h=myfifo_open_read("ass");
	if(h==NULL){
	printf("mtfifo read open NG!\n");
		return;
	}
	printf("mtfifo read open OK!\n");

	for(i=0;i<30;i++){
		printf("reading...\n");
		memset(buf,0,256);
		ret=myfifo_read(h,buf,256);
		if(ret>0)printf("fifo read _OK! ret=%d\n >>%s<<\n",ret,buf);
		myfifo_sleep(1);
	}
	printf("fifo read close\n");
	myfifo_close(h);

	printf("fifo read end\n");
}

int main(int argc,char* argv[])
{
	int readmode=1;
	int tmode=0;
	int i;
	for(i=1;i<argc;i++)if(strstr(argv[i],"-w"))readmode=0;
	for(i=1;i<argc;i++)if(strstr(argv[i],"-t"))tmode=1;

	if(readmode){
		doread();
	}else
	{
		dowrite(tmode);
	}
}

#endif
	




