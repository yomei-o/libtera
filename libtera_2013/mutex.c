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

$Log: mutex.c,v $
Revision 1.4  2008/04/14 02:02:26  akatsuka
add LICENCE BLOCK

Revision 1.3  2007/11/06 00:52:31  yomei
no message

Revision 1.2  2007/03/27 05:06:44  yomei
no message

Revision 1.1  2007/02/09 04:05:23  tooyama
no message

Revision 1.2  2006/08/19 02:33:11  yomei
no message

Revision 1.1.1.1  2006/08/17 09:13:20  yokoyama
no message

Revision 1.3  2006/07/13 03:52:25  yomei
no message

Revision 1.2  2006/05/24 09:06:50  tooyama
no message

Revision 1.1  2006/05/22 09:03:53  tooyama
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

#include "mutex.h"

mutex_t create_mutex(char *name)
{
#if defined(unix) && !defined(__CYGWIN__)
	void* fd;
	char tmp[1024];
#endif

	if(name==NULL)return NULL;

#if defined(unix) && !defined(__CYGWIN__)
	strcpy(tmp,"/win32mutex_");
	strcat(tmp,name);
#endif

#if defined(unix) && !defined(__CYGWIN__)
	fd=(void*)shm_open(tmp,O_CREAT | O_RDWR,0666);
	if((int)fd==-1)return NULL;
	return fd;
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
	return CreateMutex(NULL,FALSE,name);
#endif

	return NULL;
}



void close_mutex(mutex_t vp)
{
	if(vp==NULL)return;
#if defined(unix) && !defined(__CYGWIN__)
	close((int)vp);
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
	CloseHandle((HANDLE)vp);
#endif
	return;
}



void  get_mutex(mutex_t vp)
{
	if(vp==NULL)return;

#if defined(unix) && !defined(__CYGWIN__)
	flock((int)vp,LOCK_EX);
#endif	
#if defined(_WIN32) || defined(__CYGWIN__)
	WaitForSingleObject((HANDLE)vp,INFINITE);
#endif
}

void release_mutex(mutex_t vp)
{
	if(vp==NULL)return;
#if defined(unix) && !defined(__CYGWIN__)
	flock((int)vp,LOCK_UN);
#endif	
#if defined(_WIN32) || defined(__CYGWIN__)
	ReleaseMutex((HANDLE)vp);
#endif
}


void sleep_mutex(int s)
{
#if defined(_WIN32) || defined(__CYGWIN__)
	Sleep(s*1000);
#endif
#if defined(unix) && !defined(__CYGWIN__)
	sleep(s);
#endif
}


int is_signal_mutex(mutex_t vp)
{
	int ret;

	if(vp==NULL)return 0;

#if defined(unix) && !defined(__CYGWIN__)
	ret=flock((int)vp,LOCK_EX | LOCK_NB);
	if(ret!=0)
	{
		return 0;
	}
	flock((int)vp,LOCK_UN);
	return 1;
#endif	
#if defined(_WIN32) || defined(__CYGWIN__)
	ret=WaitForSingleObject((HANDLE)vp,0);
	if(ret!=WAIT_TIMEOUT)
	{
		ReleaseMutex((HANDLE)vp);
		return 1;
	}
#endif
	return 0;
}


/*

テストコード

*/


#if 0

int main(int argc,char *argv[])
{
	void *h=NULL;
	char* fname="mutextest";
	int ret;

	h=create_mutex(fname);
	if(h==NULL)
	{
		printf("fail to create mutex!!\n");
		return 0;
	}
	ret=is_signal_mutex(h);
	printf("is_signal_mutex=%d\n",ret);
	get_mutex(h);
	printf("got mutex\n");

	sleep_mutex(10);

	printf("released mutex\n");
	release_mutex(h);

	close_mutex(h);
	return 1;
}


#endif
