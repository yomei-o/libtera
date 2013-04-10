/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fblocking.h"
//#include "fdup.h"

#define I_USE_CYGWIN_PIPE


#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif

#ifdef unix
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#endif

#if defined(_WIN32) && !defined(__GNUC__)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


void fset_nonblocking(FILE* fp)
{
#if defined(linux) || defined(__GNUC__)
	int fd;
	int val;

	if(fp==NULL)return;
	fd=fileno(fp);
	if(fd==-1)return;

	if ((val = fcntl(fd, F_GETFL)) == -1)
		return;
	if (!(val & O_NONBLOCK)) {
		val |= O_NONBLOCK;
		fcntl(fd, F_SETFL, val);
	}
#endif
#ifdef _WIN32 
	int fd;
	unsigned long mode=PIPE_NOWAIT;
	HANDLE hPipe;

	if(fp==NULL)return;
	fd=_fileno(fp);
	if(fd==-1)return;
	hPipe=(void*)_get_osfhandle(fd);

	SetNamedPipeHandleState(hPipe,&mode,NULL,NULL);
#endif
}

/* Set a fd into blocking mode. */
void fset_blocking(FILE* fp)
{
#if defined(linux) || defined(__GNUC__)
	int fd;
	int val;


	if(fp==NULL)return;
	fd=fileno(fp);
	if(fd==-1)return;

	if ((val = fcntl(fd, F_GETFL)) == -1)
		return;
	if (val & O_NONBLOCK) {
		val &= ~O_NONBLOCK;
		fcntl(fd, F_SETFL, val);
	}
#endif

#ifdef _WIN32
	int fd;
	unsigned long mode=PIPE_WAIT;
	HANDLE hPipe;

	if(fp==NULL)return;
	fd=_fileno(fp);
	if(fd==-1)return;
	hPipe=(void*)_get_osfhandle(fd);

	SetNamedPipeHandleState(hPipe,&mode,NULL,NULL);
#endif

}


int fcygpipe_read(void*buf,size_t a,size_t b,FILE* fp)
{
#if defined(I_USE_CYGWIN_PIPE) && !defined(__GNUC__)

	OVERLAPPED ol;
	int len=0,ret=0;

	if(fp==NULL)return 0;
	if(_fileno(fp)==-1)return 0;

	memset(&ol,0,sizeof(ol));
	ret=ReadFile((void*)_get_osfhandle(fileno(fp)),buf,a*b,&len,&ol);
	fprintf(stderr,"readfile ret=%d\n",ret);
	if(ret!=0)return ret;
	GetOverlappedResult((void*)_get_osfhandle(fileno(fp)),&ol,&ret,TRUE);
	return ret;
#else
	fread(buf,a,b,fp);
#endif
}
int fcygpipe_write(void*buf,size_t a,size_t b,FILE* fp)
{
#if defined(I_USE_CYGWIN_PIPE) && !defined(__GNUC__)
	OVERLAPPED ol;
	int len=0,ret=0;
	if(fp==NULL)return 0;
	if(_fileno(fp)==-1)return 0;

	memset(&ol,0,sizeof(ol));
	ret=WriteFile((void*)_get_osfhandle(fileno(fp)),buf,a*b,&len,&ol);
	if(ret!=0)return ret;
	GetOverlappedResult((void*)_get_osfhandle(fileno(fp)),&ol,&ret,TRUE);
	return ret;
#else
	fwrite(buf,a,b,fp);
#endif
}

int fcygpipe_flush(FILE* fp)
{
#if defined(I_USE_CYGWIN_PIPE) && !defined(__GNUC__)
	return 0;
#else
	return 0;
#endif
}


#if 0

int main(int argc,char* argv[]){
	FILE *fi,*fo;
	char buf[1024];
#if defined(_WIN32) && !defined(__GNUC__)
//	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_WNDW);
//	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
//	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
#endif

	printf("Non-blocking I/O test code\n");

	fi=NULL;
	fo=NULL;

	fpipe(&fi,&fo);
	if(fi==NULL || fo==NULL){
		printf("pipe is null\n");
		return 1;
	}

	memset(buf,0,1024);
	fprintf(fo,"test\n");
	fflush(fo);
	
	printf("nonblocking\n");
	fset_nonblocking(fi);
	//printf("blocking\n");
	//fset_blocking(fi);

	printf("fget_1\n");
	memset(buf,0,1024);
	fgets(buf,1024,fi);


	printf("fget_2\n");
	memset(buf,0,1024);
	fgets(buf,1024,fi);

	if(fi!=NULL)fclose(fi);
	if(fo!=NULL)fclose(fo);


	return 0;
}

#endif

