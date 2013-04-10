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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#endif

#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif

#include "fastsystem.h"
#include "spawn.h"
#include "fdup.h"

#define MAXNUM_OF_CMDPARAM 64
#define MAXNUM_OF_PID	   10

//#define I_USE_DEBUG_PRINT


static int get_token( char *cmd, char *argv[], int sz )
{
	int i;

	if( cmd==NULL || sz <= 0 ) return -1;

	/* argv初期化 */
	for(i=0; i<sz; i++)argv[i]=NULL;

	for(i=0; i<sz-1; i++){  /* 最後のargvは必ずNULLが入るようにする */
		/* 先頭を探す */
		while( *cmd==' ' && *cmd!=0 ) cmd++;
		if( *cmd==0 ) break;
		/* ""で括られている場合は一つのパラメータとみなす */
		if( *cmd=='\"' ){
			argv[i] = cmd;
			cmd++;
			while( *cmd!='\"' && *cmd!=0 ) cmd++;
			if( *cmd==0 ) break;
			if( *cmd=='\"' )cmd++;
			if( *cmd==0 )break;
			*cmd = 0;
			cmd++;
		}else{
			argv[i] = cmd;
			while( *cmd!=' ' && *cmd!=0 ) cmd++;
			if( *cmd==0 ) break;
			*cmd = 0;
			cmd++;
		}
	}
	return 0;
}

/* spawnvp に渡す引数を作成する関数
   unixの場合は、""をはずす */
static int mk_cmdparameter( char *argv[], int sz )
{
#ifdef unix
	int i, len;
	char *p;

	for(i=0; i<sz && argv[i]!=NULL; i++){

		len = strlen(argv[i]);
		p = argv[i];

		if( argv[i][0]=='"' ){
			p = &argv[i][1];
		}
		if( argv[i][len-1]=='"' ){
			argv[i][len-1] = 0;
		}
		argv[i] = p;
	}
#endif
	return 0;
}


/* コマンド内にタブがあったら空白に置き換える */
static void check_command( char *src, char *dst )
{
	if(dst==NULL || src==NULL)return;
	dst = src;

	while( *src && *src!=0 ){
		if( *src=='\t' ) *src = ' ';
		src++;
	}
}

/* コマンド１つを実行する関数
　 入力元・出力先を引数で指定する */
static int exec_internal_fastsystem( char *command, FILE *in, FILE *out )
{
	FILE *fpin	=NULL;
	FILE *fpout	=NULL;
	char *argv[MAXNUM_OF_CMDPARAM];
	int ret = -1;
#ifdef I_USE_DEBUG_PRINT
	int i;
#endif

	if( command==NULL || in==NULL || out==NULL ) return -1;
	
	if( (fpin = fdup( stdin, "rb" ))==NULL ) return -1;
	if( (fpout = fdup( stdout, "wb" ))==NULL ){
		fclose(fpin);	
		return -1;
	}

	/* 標準入力・標準出力を指定されたinとoutに設定する */
	fdup2( out, stdout );
	fdup2( in, stdin );

	get_token( command, argv, MAXNUM_OF_CMDPARAM );
#ifdef I_USE_DEBUG_PRINT
	for(i=0; i<MAXNUM_OF_CMDPARAM && argv[i]!=NULL; i++)
		fprintf(stderr, "argv[%d]=>>>%s<<<\n", i, argv[i] );
#endif
	mk_cmdparameter( argv, MAXNUM_OF_CMDPARAM );
#ifdef I_USE_DEBUG_PRINT
	for(i=0; i<MAXNUM_OF_CMDPARAM && argv[i]!=NULL; i++)
		fprintf(stderr, "argv[%d]=>>>%s<<<\n", i, argv[i] );
#endif
	ret = (int)spawnvp( P_NOWAIT, argv[0]?argv[0]:"", (const char **)argv );

#ifdef I_USE_DEBUG_PRINT
	fprintf(stderr, "pid=%d\n", ret );
#endif

	/* 標準入力・標準出力を元に戻す */
	fdup2( fpout, stdout );
	fdup2( fpin, stdin );

	if(fpin) fclose( fpin );
	if(fpout) fclose( fpout );

	return ret;
}


#if defined(_WINDOWS) || defined(_USRDLL)
static void myallocconsole(void)
{
	DWORD*	pflags = (DWORD*) 0x00020068; /* private memory */
	WORD*	pshow  = (WORD*)  0x0002006C;
	DWORD	backup_flags = *pflags;
	WORD	backup_show  = *pshow;STARTUPINFO si;
	static int flag=1;


	GetStartupInfo(&si);
	if(si.dwFlags == backup_flags && si.wShowWindow == backup_show) {
		*pflags |= STARTF_USESHOWWINDOW;
		*pshow  = SW_HIDE;
	}

 	AllocConsole();

	*pflags = backup_flags;
	*pshow  = backup_show;


	/* check for STDIN is invalid */
	//if( _get_osfhandle( 0 ) == -1 )
	if(flag)
	{
		freopen("NUL","rb",stdin);
		SetStdHandle(STD_INPUT_HANDLE,(HANDLE)_get_osfhandle(fileno(stdin)));
	}
	/* check for STDOUT is invalid */
	//if( _get_osfhandle( 1 ) == -1 )
	if(flag)
	{
		freopen("NUL","wb",stdout);
		SetStdHandle(STD_OUTPUT_HANDLE,(HANDLE)_get_osfhandle(fileno(stdout)));
	}
	/* check for STDERR is invalid */
	//if( _get_osfhandle( 2 ) == -1 )
	if(flag)
	{
		freopen("NUL","wb",stderr);
		SetStdHandle(STD_ERROR_HANDLE,(HANDLE)_get_osfhandle(fileno(stderr)));
	}

	flag=0;

}

#endif



static int internal_fastsystem( char *command, FILE *in, FILE *out, int* pid_list, int num_pid_list )
{
	FILE *fpipe_r	=NULL;
	FILE *fpipe_w	=NULL;
	char *p;
	int ret;

#if defined(_WINDOWS) || defined(_USRDLL)
	myallocconsole();
#endif

	if( command==NULL || in==NULL || out==NULL ) return -1;

	/* パイプでつながれているかチェックする */
	p = strchr( command, '|' );

	/*　後ろに |（パイプ）がある場合 */
	if( p!=NULL )
	{
		if( fpipe( &fpipe_r, &fpipe_w )==-1 ){
			fprintf(stderr, "fpipe error\n");
			goto END;
		}

		*p = 0;
		p++;

		ret = internal_fastsystem( command, in, fpipe_w, pid_list, (num_pid_list>0)?1:0 );
		fclose( fpipe_w );
		if( ret==-1 ) goto END;
	
		ret = internal_fastsystem( p, fpipe_r, out, pid_list?(pid_list+1):NULL, num_pid_list-1 );
		goto END;
	}
	
	/* 後ろに |（パイプ）が無い場合 */
	ret = exec_internal_fastsystem( command, in, out );
	if( pid_list && num_pid_list > 0 ) *pid_list = ret;

END:
	if( fpipe_r ) fclose( fpipe_r );

	return 0;
}


#if 0

/* 高速 system() 関数（NO_WAIT） */
int fast_nwsystem( const char *cmd )
{
	char *command	=NULL;
	int ret = -1;

	if( cmd==NULL ) goto END;

	command = (char *)malloc( strlen(cmd)+1 );
	if( command==NULL ) goto END;;
	
	command[0] = 0;
	strncpy( command, cmd, strlen(cmd) );
	command[strlen(cmd)] = 0;

	check_command( command, command );

	ret = internal_fastsystem( command, stdin, stdout, NULL, 0 );

	free( command );
END:
	return ret;
}

#endif

/* 高速 system() 関数（WAITする） */
int fast_system( const char *cmd )
{
	char *command	=NULL;
	int ret = -1;
	int pid_list[MAXNUM_OF_PID];

	if( cmd==NULL ) goto END;

	memset( pid_list, 0, sizeof(pid_list) );

	command = (char *)malloc( strlen(cmd)+1 );
	if( command==NULL ) goto END;;
	
	command[0] = 0;
	strncpy( command, cmd, strlen(cmd) );
	command[strlen(cmd)] = 0;

	check_command( command, command );

	ret = internal_fastsystem( command, stdin, stdout, pid_list, MAXNUM_OF_PID );
	
	{
		int i;
		for(i=0; i<MAXNUM_OF_PID; i++)
		{
			if( pid_list[i] ) ret=mywait( pid_list[i] );
#ifdef I_USE_DEBUG_PRINT
			fprintf(stderr, "pid_list[%d]=%d\n", i, pid_list[i] );
#endif
		}
	}
	
	free( command );
END:
	return ret;
}


#if 0
int main()
{
//	char cmd[] = "ls -l | grep \".dll\"";
//	char cmd[] = "ls -l";
	char cmd[] = "ls -l | more | more | more | grep \".\" | grep \".dll\" | grep \"2005\"";
//	char cmd[] = "dummy 5";
	//	char cmd[] = "ls -l | grep \".\" | grep \".dll\"";
//	char cmd[] = "wget \"http://43.8.218.191/sample2.mp3\" -O - | lame --decode --mp3input - - | waveplay -i -";
	int ret;

//	ret = fast_nwsystem( cmd );

	ret = fast_system( cmd );
	fprintf(stderr, "main ret=%d\n", ret );

	printf("aaa\n");
	return 0;
}
#endif

