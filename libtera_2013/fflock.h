/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/
 
#ifndef _FFLOCK_H_
#define _FFLOCK_H_

#ifdef unix
#include <sys/file.h>
#endif

#if 0

#define fsync _commit
#ifndef ftruncate
#define ftruncate chsize
#endif
#endif

#ifdef _WIN32

/* For flock() emulation */
#define LOCK_SH 1
#define LOCK_EX 2
#define LOCK_NB 4
#define LOCK_UN 8
 
int flock(int fd, int op);

#endif

#endif /* _FFLOCK_H_ */
