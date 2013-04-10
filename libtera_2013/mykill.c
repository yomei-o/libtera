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


