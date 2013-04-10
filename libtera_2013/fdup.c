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
#include <process.h>
#include <io.h>
#include <fcntl.h>
#endif

#ifdef unix
#include <unistd.h>
#include <fcntl.h>
#endif

#include "fdup.h"


FILE* fdup(FILE* fp,char*mode)
{
	int fd;
	fd=dup(fileno(fp));
	return fdopen(fd,mode);
}


FILE *fdup2( FILE *oldfp, FILE *newfp )
{
	int oldfd, newfd;
	int res;

	if( oldfp==NULL || newfp==NULL ) return NULL;

	oldfd = fileno( oldfp );
	newfd = fileno( newfp );
	
	res = dup2( oldfd, newfd );

	if( res==-1) return NULL;
	else return newfp;
}


void change_stdout(FILE* fp)
{
	int fd;
	fd=fileno(fp);
	dup2(fd,1);
}


void change_stdin(FILE* fp)
{
	int fd;
	fd=fileno(fp);
	dup2(fd,0);
}


void change_stderr(FILE* fp)
{
	int fd;
	fd=fileno(fp);
	dup2(fd,2);
}

int fpipe( FILE **fpipe_r, FILE **fpipe_w )
{
	int pipe_[2];

	if( fpipe_r==NULL || fpipe_w==NULL ) return -1;

#ifdef _WIN32
	if( _pipe( pipe_, 65536, O_BINARY | _O_NOINHERIT )== -1) return -1;
#endif
#ifdef unix
	if( pipe(pipe_)== -1 ) return -1;
#endif


#ifdef unix
	fcntl(pipe_[0], F_SETFD, FD_CLOEXEC);
	fcntl(pipe_[1], F_SETFD, FD_CLOEXEC);
	//fprintf(stderr,"### kita minami\n");
#endif

	*fpipe_r = fdopen( pipe_[0], "rb" );
	*fpipe_w = fdopen( pipe_[1], "wb" );

	return 0;
}


#if 0

main()
{
	FILE *fpf,*fpc;

	fpc=fdup(stdout,"wt");
	printf("\n---000---\n");	
	fflush(stdout);

	fpf=fopen("aaaa.txt","wt");
	change_stdout(fpf);
	printf("\n---111---\n");
	fflush(stdout);
	fclose(fpf);
	printf("\n---222---\n");
	fflush(stdout);
#ifdef _WIN32
	spawnlp(P_WAIT,"cmd","cmd",NULL);
#endif
	change_stdout(fpc);
	printf("\n---333---\n");
	fflush(stdout);
	fclose(fpc);
	printf("\n---444---\n");
	fflush(stdout);
}
#endif
