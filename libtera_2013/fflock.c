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

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <errno.h>
#endif

#ifdef unix
#include <unistd.h>
#endif

#include "fflock.h"
 


#ifdef _WIN32
int flock(int fd, int op)
{
         HANDLE hdl = (HANDLE) _get_osfhandle(fd);
         DWORD low = 1, high = 0;
         OVERLAPPED offset =
         {0, 0, 0, 0, NULL};
         if (hdl < 0)
                 return -1;                              /* error in file descriptor */
         /* bug for bug compatible with Unix */
         UnlockFileEx(hdl, 0, low, high, &offset);
         switch (op & ~LOCK_NB) {        /* translate to LockFileEx() op */
                 case LOCK_EX:                   /* exclusive */
                         if (LockFileEx(hdl, LOCKFILE_EXCLUSIVE_LOCK +
                                                 ((op & LOCK_NB) ? LOCKFILE_FAIL_IMMEDIATELY : 0),
                                                    0, low, high, &offset))
                                 return 0;
                         break;
                 case LOCK_SH:                   /* shared */
                         if (LockFileEx(hdl, ((op & LOCK_NB) ? LOCKFILE_FAIL_IMMEDIATELY : 0),
                                                    0, low, high, &offset))
                                 return 0;
                         break;
                 case LOCK_UN:                   /* unlock */
                         return 0;                       /* always succeeds */
                 default:                                /* default */
                         break;
         }
         errno = EINVAL;                         /* bad call */
         return -1;
}
#endif /* _WIN32 */



int fflock(FILE* fp,int op)
{
	int fd;
	fd=fileno(fp);
	return flock(fd,op);
}

void ffsleep(int s)
{
#ifdef _WIN32
	Sleep(s*1000);
#endif
#ifdef unix
	sleep(s);
#endif
}

#if 0

main()
{
	FILE* fp;



	fp=fopen("aaa.txt","wb");
	fflock(fp,LOCK_EX);
	printf("locled\n");
	ffsleep(10);
	fflock(fp,LOCK_UN);
	printf("unlocled\n");
	fclose(fp);



}


#endif


