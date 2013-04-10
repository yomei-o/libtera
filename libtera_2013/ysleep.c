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



#define __YSLEEP_C__

#ifdef unix
#include <unistd.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#endif

#include "ysleep.h"


void
ysleep(unsigned int s)
{
#ifdef unix
	sleep(s);
#else
#ifdef _WIN32
	Sleep(s * 1000);
#endif
#endif
	return;
}


void
ymsleep(unsigned int ms)
{
#ifdef unix
	sleep(ms / 1000);
	usleep((ms % 1000) * 1000);
#else
#ifdef _WIN32
	Sleep(ms);
#endif
#endif
	return;
}


void
yusleep(unsigned int us)
{
#ifdef unix
	usleep(us);
#else
#ifdef _WIN32
	Sleep(1);
#endif
#endif
	return;
}
