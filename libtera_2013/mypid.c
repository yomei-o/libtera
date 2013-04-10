/******* BEGIN LICENCE BLOCK *******

    Sony Nasne Application
    Copyright (C) 2012 Sony Corporation.
    Yomei.Otani@jp.sony.com

******* END LICENCE BLOCK *******/


#include <stdio.h>
#ifdef _WIN32
#include <process.h>
#endif
#ifdef unix
#include <unistd.h>
#endif


#include "mypid.h"

int mygetpid(void){
#ifdef _WIN32
	return _getpid(); 
#endif /* _WIN32 */
#ifdef unix
	return getpid();
#endif
	return -1;
}

