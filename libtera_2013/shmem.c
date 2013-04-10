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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
#include <winbase.h>
#endif

#if defined(unix) && !defined(__CYGWIN__)
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/file.h>
#include <errno.h>
#include <unistd.h>
#endif

#define I_USE_NO_UNLINK

#include "shmem.h"



#if defined(unix) && !defined(__CYGWIN__)

struct my_shmem
{
	int fd;
	int sz;
	void* ptr;
	char name[1024];
};

#endif


#if defined(_WIN32) || defined(__CYGWIN__)

struct my_shmem
{
	HANDLE sm;
	HANDLE mutex;
	void* ptr;
};

#endif

 


//共有メモリを作る
share_t create_share(char *name,int sz)
{
	void* vp;
	char tmp[1024];

#if defined(_WIN32) || defined(__CYGWIN__)
	HANDLE h;
#endif
#if defined(unix) && !defined(__CYGWIN__)
	int fd;
#endif


	if(name==NULL || sz<=0)return NULL;

#if defined(unix) && !defined(__CYGWIN__)
	strcpy(tmp,"/sharemem_");
	strcat(tmp,name);
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
	h=CreateFileMapping( INVALID_HANDLE_VALUE,NULL, PAGE_READWRITE /*| GENERIC_READ | GENERIC_WRITE*/,0,sz,name);
	if(h==NULL)return NULL;
	vp=malloc(sizeof(struct my_shmem));
	if(vp==NULL)
	{
		CloseHandle(h);
	}
	strcpy(tmp,"sharemem_");
	strcat(tmp,name);
	((struct my_shmem*)vp)->ptr=NULL;
	((struct my_shmem*)vp)->sm=h;
	((struct my_shmem*)vp)->mutex=CreateMutex(NULL,FALSE,tmp);
	if(((struct my_shmem*)vp)->mutex==NULL)
	{
		free(vp);
		CloseHandle(h);
	}
	return vp;

#endif

#if defined(unix) && !defined(__CYGWIN__)
	fd=shm_open(tmp,O_CREAT | O_RDWR,0666);
	if(fd==-1)return NULL;
	if(ftruncate(fd,sz)==-1)
	{
#ifndef I_USE_NO_UNLINK
		shm_unlink(tmp);
#endif
		return NULL;
	}

	vp=malloc(sizeof(struct my_shmem));
	if(vp==NULL)
	{
#ifndef I_USE_NO_UNLINK
		shm_unlink(tmp);
#endif
		return NULL;
	}
	strcpy(((struct my_shmem*)vp)->name,tmp);
	((struct my_shmem*)vp)->fd=fd;
	((struct my_shmem*)vp)->sz=sz;
	return vp;
#endif
	return NULL;
}



//すでに作られている共有メモリのハンドルを返す
share_t open_share(char *name,int sz)
{
	char tmp[1024];
	void* vp;
#if defined(_WIN32) || defined(__CYGWIN__)
	HANDLE h;
#endif
#if defined(unix) && !defined(__CYGWIN__)
	int fd;
#endif
	if(name==NULL || sz<=0)return NULL;


#if defined(unix) && !defined(__CYGWIN__)
	strcpy(tmp,"/sharemem_");
	strcat(tmp,name);
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
	h=CreateFileMapping( INVALID_HANDLE_VALUE,NULL, PAGE_READWRITE,0,sz,name);
	if(h==NULL)return NULL;

	//すでに作られているエラーではないならばエラーにする
	if(GetLastError()!=ERROR_ALREADY_EXISTS)
	{
		CloseHandle(h);
		return NULL;
	}

	vp=malloc(sizeof(struct my_shmem));
	if(vp==NULL)
	{
		CloseHandle(h);
	}
	strcpy(tmp,"sharemem_");
	strcat(tmp,name);
	((struct my_shmem*)vp)->ptr=NULL;
	((struct my_shmem*)vp)->sm=h;
	((struct my_shmem*)vp)->mutex=CreateMutex(NULL,FALSE,tmp);
	if(((struct my_shmem*)vp)->mutex==NULL)
	{
		free(vp);
		CloseHandle(h);
	}
	return vp;
#endif

#if defined(unix) && !defined(__CYGWIN__)
	fd=shm_open(tmp,O_RDWR,0666);
	if(fd==-1)return NULL;
	if(ftruncate(fd,sz)==-1)
	{
#ifndef I_USE_NO_UNLINK
		shm_unlink(tmp);
#endif
		return NULL;
	}

	vp=malloc(sizeof(struct my_shmem));
	if(vp==NULL)
	{
#ifndef I_USE_NO_UNLINK
		shm_unlink(tmp);
#endif
		return NULL;
	}
	strcpy(((struct my_shmem*)vp)->name,tmp);
	((struct my_shmem*)vp)->fd=fd;
	((struct my_shmem*)vp)->sz=sz;
	return vp;
#endif
	return NULL;
}



//共有メモリを閉じる
int close_share(share_t h)
{
	if(h==NULL)return 0;

#if defined(_WIN32) || defined(__CYGWIN__)
	CloseHandle(((struct my_shmem*)h)->sm);
	CloseHandle(((struct my_shmem*)h)->mutex);
	free(h);
	return 1;
#endif
#if defined(unix) && !defined(__CYGWIN__)
	close(((struct my_shmem*)h)->fd);
#ifndef I_USE_NO_UNLINK
	shm_unlink(((struct my_shmem*)h)->name);
#endif
	free(h);
#endif
	return 0;
}



//共有メモリハンドルをポインタに変換してメモリをロック
share_t map_share(share_t h)
{
#if defined(_WIN32) || defined(__CYGWIN__)
	void* ptr;
#endif
	if(h==NULL)return NULL;
#if defined(_WIN32) || defined(__CYGWIN__)
	ptr=(void*)MapViewOfFile(((struct my_shmem*)h)->sm, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	((struct my_shmem*)h)->ptr=ptr;
	return ptr;
#endif
#if defined(unix) && !defined(__CYGWIN__)

	((struct my_shmem*)h)->ptr=(void*)mmap(NULL,((struct my_shmem*)h)->sz,
		PROT_READ | PROT_WRITE,MAP_SHARED,
		((struct my_shmem*)h)->fd,0);

	if(((struct my_shmem*)h)->ptr==(void*)-1)
	{
		((struct my_shmem*)h)->ptr=NULL;
		return NULL;
	}
	return ((struct my_shmem*)h)->ptr;
#endif
	return NULL;
}

//ポインタに対応するメモリをアンロック
void unmap_share(share_t vp)
{
	if(vp==NULL)return;
#if defined(_WIN32) || defined(__CYGWIN__)
	UnmapViewOfFile(((struct my_shmem*)vp)->ptr);
#endif
#if defined(unix) && !defined(__CYGWIN__)
	if(((struct my_shmem*)vp)->ptr)
	{
		munmap(((struct my_shmem*)vp)->ptr,((struct my_shmem*)vp)->sz);
	}
#endif
}


int  lock_share(share_t vp)
{
	if(vp==NULL)return -1;

#if defined(unix) && !defined(__CYGWIN__)
	return flock(((struct my_shmem*)vp)->fd,LOCK_EX);
#endif	
#if defined(_WIN32) || defined(__CYGWIN__)
	WaitForSingleObject(((struct my_shmem*)vp)->mutex,INFINITE);	
#endif
	return -1;
}

int unlock_share(share_t vp)
{
	if(vp==NULL)return -1;
#if defined(unix) && !defined(__CYGWIN__)
	return flock(((struct my_shmem*)vp)->fd,LOCK_UN);
#endif	
#if defined(_WIN32) || defined(__CYGWIN__)
	ReleaseMutex(((struct my_shmem*)vp)->mutex);
#endif
	return -1;
}

void sleep_share(int s)
{
#if defined(_WIN32) || defined(__CYGWIN__)
	Sleep(s*1000);
#endif
#if defined(unix) && !defined(__CYGWIN__)
	sleep(s);
#endif
}


/*

テストコード

*/



#if 0

int main(int argc,char *argv[])
{
	void *h=NULL,*p=NULL;
	char *a=NULL,*b=NULL;
	char* fname="test00b";
//	int i;

	h=open_share(fname,65536);
	if(h==NULL)
	{
		void *vp;
		printf("fail to read share memory!!\n");
		h=create_share(fname,65536);
		if(h==NULL)
		{
			printf("fail to create memory\n");
		}
		if(h!=NULL)
		{
			printf("zero clear memory\n");
			vp=map_share(h);
			if(vp==NULL)
			{
				printf("fail map_share\n");
				goto end;
			}
			memset(vp,0,65536);
			unmap_share(vp);
		}
	}
	lock_share(h);
	printf("locked\n");
	printf("step 1\n");
	p=open_share(fname,65536);
	if(p==NULL)goto end;

	printf("step 2\n");
	a=map_share(h);
	if(a==NULL)goto end;

	printf("step 3\n");
	b=map_share(p);
	if(b==NULL)goto end;


	printf("success!!!!\n");

	a[0]=0x12;
	a[1]=0x34;

	printf("%02x %02x\n",b[0],b[1]);

end:
	sleep_share(10);
	printf("unlocked\n");
	unlock_share(h);
	if(a)unmap_share(h);
	if(b)unmap_share(p);

	if(p)close_share(p);
	if(h)close_share(h);

	printf("end\n");
	sleep_share(10);
	return 1;
}


#endif
