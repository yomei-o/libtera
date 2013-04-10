/******* BEGIN LICENCE BLOCK *******

    Sony Digital Photo Frame Application
    Copyright (C) 2008 Sony Corporation.

    This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License,version 2, as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses>.

    Linking Sony Digital Photo Frame Application statically or dynamically with other modules is making a combined work based on Sony Digital Photo Frame Application. Thus, the terms and conditions of the GNU General Public License cover the whole combination.

    In addition, as a special exception, the copyright holders of Sony Digital Photo Frame Application give you permission to combine Sony Digital Photo Frame Application with free software programs or libraries that are released under the GNU LGPL and with code included in the standard release of Sony OS Abstraction Library (or modified versions of such code, with unchanged license). You may copy and distribute such a system following the terms of the GNU GPL for Sony Digital Photo Frame Application and the licenses of the other code concerned{, provided that you include the source code of that other code when and as the GNU GPL requires distribution of source code}.

    Note that people who make modified versions of Sony Digital Photo Frame Application are not obligated to grant this special exception for their modified versions; it is their choice whether to do so. The GNU General Public License gives permission to release a modified version without this exception; this exception also makes it possible to release a modified version which carries forward this exception.

******* END LICENCE BLOCK *******/


 
/* the following line is the modification history by RCS
   don't edit this line

$Log: shmem.c,v $
Revision 1.4  2008/04/14 02:02:26  akatsuka
add LICENCE BLOCK

Revision 1.3  2007/11/06 00:52:49  yomei
no message

Revision 1.2  2007/03/27 05:09:04  yomei
no message

Revision 1.1  2007/02/09 04:05:23  tooyama
no message

Revision 1.2  2006/08/19 02:32:52  yomei
no message

Revision 1.1.1.1  2006/08/17 09:13:20  yokoyama
no message

Revision 1.5  2006/07/13 03:50:45  yomei
no message

Revision 1.4  2006/05/30 03:53:00  yomei
no message

Revision 1.3  2006/05/30 03:49:04  yomei
no message

Revision 1.2  2006/05/24 09:07:15  tooyama
no message

Revision 1.1  2006/05/22 09:12:09  tooyama
no message


*/

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
