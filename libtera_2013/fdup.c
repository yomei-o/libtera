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
