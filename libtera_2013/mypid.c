/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

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

