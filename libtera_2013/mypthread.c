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

#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif


/* �X���b�h�쐬���C�u����
�iLinux p_thread�x�[�X�j*/
#include <stdio.h>

#ifdef unix
#include <unistd.h>
#include <pthread.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#endif

#include "mypthread.h"

#ifndef PTHREAD_TEST

void my_pthread_create(void (*func)(void*), void *arg)
{
#ifdef _WIN32
	_beginthread(func, 0, arg);
#endif
#ifdef unix
	pthread_t ph;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); 
	pthread_create(&ph, &attr, (void*(*)(void*))func, arg);
	pthread_attr_destroy(&attr);
#endif
}
#endif /* PTHREAD_TEST */



#ifdef PTHREAD_TEST

void mysleep(int s)
{
#ifdef unix
	sleep(s);
#endif
#ifdef _WIN32
	Sleep(s*1000);
#endif
}

/* �T�u�X���b�h */
void subthread(void* vp)
{
	int i;

	printf("sub thread start\n");

	for(i=0; i<5; i++){
		printf("-\n");
		mysleep(1);
	}
	
	printf("sub thread end\n");	
}

int main()
{
	my_pthread_create(subthread, NULL);

	mysleep(2);
	printf("main thread end\n");
}

#endif /* PTHREAD_TEST */

