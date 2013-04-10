/******* BEGIN LICENCE BLOCK *******

    Sony Nasne Application
    Copyright (C) 2012 Sony Corporation.
    Yomei.Otani@jp.sony.com

******* END LICENCE BLOCK *******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(unix) && !defined(__CYGWIN__)
#include <unistd.h>
#endif

#include "myuid.h"

#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif

int mygetuid(void)
{
#if defined(unix) && !defined(__CYGWIN__)
	return getuid();
#endif
	return -1;
}

int mygetgid(void)
{
#if defined(unix) && !defined(__CYGWIN__)
	return getgid();
#endif
	return -1;
}

int mygeteuid(void)
{
#if defined(unix) && !defined(__CYGWIN__)
	return geteuid();
#endif
	return -1;
}

int mygetegid(void)
{
#if defined(unix) && !defined(__CYGWIN__)
	return getegid();
#endif
	return -1;
}


int mysetuid(int id)
{
#if defined(unix) && !defined(__CYGWIN__)
	return setuid(id);
#endif
	return -1;
}

int mysetgid(int id)
{
#if defined(unix) && !defined(__CYGWIN__)
	return setgid(id);
#endif
	return -1;
}

int myseteuid(int id)
{
#if defined(unix) && !defined(__CYGWIN__)
	return seteuid(id);
#endif
	return -1;
}

int mysetegid(int id)
{
#if defined(unix) && !defined(__CYGWIN__)
	return setegid(id);
#endif
	return -1;
}

int mychown(char* name,int uid,int gid)
{
#if defined(unix) && !defined(__CYGWIN__)
	return chown(name,uid,gid);
#endif
	return -1;
}




