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
#include <stdlib.h>
#include <string.h>


#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#endif

#ifdef unix
#include <unistd.h>
#endif

#include "fastpopen.h"
#include "spawn.h"
#include "fdup.h"
#include "mykill.h"

#include "pathopen.h"

#define MAXLEN_OF_CMDPATH 1024
#define MAXNUM_OF_CMDPARAM 64
#define MAXNUM_OF_STREAM 10
#define R	0
#define W	1

#define I_USE_FULLPATH

static FILE* list_stream[MAXNUM_OF_STREAM];
static int list_pid[MAXNUM_OF_STREAM];



//#define I_USE_DEBUG_PRINT
static void unescape_token(char *argv[])
{
#ifdef unix
	int i;

	for(i=0;;i++)
	{
		char *src,*dst;
		if(argv[i]==NULL)break;
		src=argv[i];
		dst=argv[i];

		while(*src)
		{
			if(*src=='\\')src++;
			if(*src==0)break;
			*dst++=*src++;
		}
		*dst=0;
	}
#endif
}

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
more:
			while( *cmd!='\"' && *cmd!=0) cmd++;
			if(*(cmd-1)=='\\')
			{
				cmd++;
				if(*cmd==0)break;
				goto more;
			}
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



FILE *fastpopen( char *cmd, char *mode )
{
	FILE *fpipe_r	=NULL;
	FILE *fpipe_w	=NULL;
	FILE *fp		=NULL;
	char *command	=NULL;
	char *argv[MAXNUM_OF_CMDPARAM];
	void *m_handle	= NULL;
	int flag;
	int pid;
	int i;
	FILE* ret=NULL;
#ifdef I_USE_FULLPATH
	char *p=NULL,*q=NULL;
	char cpath[MAXLEN_OF_CMDPATH],cfpath[MAXLEN_OF_CMDPATH];
#endif

	if( cmd==NULL ) return NULL;
	if( mode[0]=='w' ) flag = W;
	else if( mode[0]=='r' ) flag = R;
	else return NULL;



//	semlock();
//	fprintf( stderr, "[fastpopen] in ----------------------------------------------\n" ) ;

#ifdef I_USE_FULLPATH
	p=cmd;
	q=cpath;
	*q=0;
	while(*p && *p<=0x20)p++;
	while(*p && *p>0x20)*q++=*p++;
	*q=0;
	cfpath[0]=0;
	fpathpath( cpath, "rb", cfpath, sizeof(cfpath) );
#ifdef _WIN32
	if(!cfpath[0]) {
		strcpy( cfpath, cpath );
	}
#endif

	command = (char *)malloc( strlen(cfpath)+strlen(cmd)+1 );
#else

	command = (char *)malloc( strlen(cmd)+1 );
#endif
	if( command==NULL ) goto end;
	
	command[0] = 0;
#ifdef I_USE_FULLPATH
	strncpy( command, cfpath, strlen(cfpath) );
	strncpy( &command[strlen(cfpath)], p, strlen(p) );
	command[strlen(cfpath)+strlen(p)] = 0;
#else
	strncpy( command, cmd, strlen(cmd) );
	command[strlen(cmd)] = 0;
#endif

#ifdef I_USE_DEBUG_PRINT
	fprintf(stderr, "%s\n", command );
#endif
	check_command( command, command );

	if( fpipe( &fpipe_r, &fpipe_w )==-1 ){
		fprintf(stderr, "fpipe error\n");
		goto end;
	}

	fflush( stdin );
	fflush( stdout );

	if( flag==W ) {
		if( (fp = fdup( stdin, "rb"))==NULL ){
			fprintf(stderr,"fdup error\n");
			goto end;
		}
		fdup2( fpipe_r, stdin );
		fclose( fpipe_r );
		fpipe_r=NULL;

	}else if( flag==R ){
		if( (fp = fdup( stdout, "wb"))==NULL ){
			fprintf(stderr,"fdup error\n");
			goto end;
			}
		fdup2( fpipe_w, stdout );
		fclose( fpipe_w );
		fpipe_w=NULL;

	}
//	fprintf( stderr, "[fpipe_r:%p] [fpipe_w:%p] [fp:%p]\n", fpipe_r, fpipe_w, fp ) ;

	get_token( command, argv, MAXNUM_OF_CMDPARAM );
	unescape_token(argv);
#ifdef I_USE_DEBUG_PRINT
	for(i=0; i<MAXNUM_OF_CMDPARAM && argv[i]!=NULL; i++)
		fprintf(stderr, "argv[%d]=>>>%s<<<\n", i, argv[i] );
#endif
	mk_cmdparameter( argv, MAXNUM_OF_CMDPARAM );
#ifdef I_USE_DEBUG_PRINT
	for(i=0; i<MAXNUM_OF_CMDPARAM && argv[i]!=NULL; i++)
		fprintf(stderr, "argv[%d]=>>>%s<<<\n", i, argv[i] );
#endif
	pid=(int)spawnvp( P_NOWAIT, argv[0]?argv[0]:"", (const char **)argv );

	free(command);
	command=NULL;

	for(i=0;i<MAXNUM_OF_STREAM;i++)
	{
		if(list_stream[i]==0)
		{
			if(flag==W)
				list_stream[i]=fpipe_w;
			else 
				list_stream[i]=fpipe_r;

//			fprintf( stderr, "fastpopen i:%d pid:%d\n", i, pid ) ;
			list_pid[i]=pid;
			break;
		}
	}


	if( flag==W ){
		/* 標準入力を元に戻す */
		fdup2( fp, stdin );
		fclose( fp );
		ret= fpipe_w;
		fpipe_w=NULL;
	}else if( flag==R ){
		/* 標準出力を元に戻す */
		fdup2( fp, stdout );
		fclose( fp );
		ret= fpipe_r;
		fpipe_r=NULL;
	}else
		ret= NULL;

end:


	if(fpipe_r)fclose(fpipe_r);		
	if(fpipe_w)fclose(fpipe_w);
	if(command)
	{
		free(command);
		command=NULL;
	}
	return ret;
}

void fastpclose( FILE *fp )
{
	int i;

//	fprintf( stderr, "fastpclose in\n" ) ;

	if(fp==NULL)return;

	fclose(fp);
	
	for(i=0;i<MAXNUM_OF_STREAM;i++)
	{
		if(list_stream[i]==fp)
		{

//			fprintf( stderr, "fastpclose i:%d pid:%d\n", i, list_pid[i] ) ;
			mywait(list_pid[i]);

			list_stream[i]=NULL;
			list_pid[i]=0;
			break;
		}
	}

}

void fastpabort( FILE *fp )
{
	int i,pid;

//	fprintf( stderr, "fastpabort in\n" ) ;

	if(fp==NULL)return;
	for(i=0;i<MAXNUM_OF_STREAM;i++)
	{
		if(list_stream[i]==fp)
		{
//			fprintf( stderr, "fastpclose i:%d pid:%d\n", i, list_pid[i] ) ;

			pid=handle_2_pid(list_pid[i]);
			mykill15(pid);
		}
	}
}
void fastpkill( FILE *fp )
{
	int i,pid;
	if(fp==NULL)return;
	for(i=0;i<MAXNUM_OF_STREAM;i++)
	{
		if(list_stream[i]==fp)
		{
			pid=handle_2_pid(list_pid[i]);
			mykill9(pid);

		}
	}

}


#if 0
/* テストプログラム */
int main(void)
{
	FILE *fpin, *fpout;
	char buf[256];

	if( (fpin = fastpopen( "ls	-l", "r" ))==NULL ) {
		fprintf(stderr, "mypopen error\n");
		return 0;
	}
	if( (fpout = fastpopen( "grep \".dll\"" , "w" ))==NULL ){
		fprintf(stderr, "mypopen error\n");
		return 0;
	}

	while( fgets( buf, sizeof(buf), fpin) !=NULL ){
		fputs( buf, fpout );
	}

	fastpclose(fpin);
	fastpclose(fpout);
}
#endif


#if 0
/* テストプログラム */
int main(void)
{
	FILE *fpin;

	if( (fpin = fastpopen( "ls_argv -l aaaaa bbbbb ccccc \"aaaaa bbbbbb\" \"aaaaa bbbbbb", "r" ))==NULL ) {
		fprintf(stderr, "mypopen error\n");
		return 0;
	}

	fastpclose(fpin);
}
#endif

#if 0
/* parse_commandテストプログラム */
int main()
{
	char cmd[6][128] = { "aaaaa bbbbb cccccc ddddd eeeeee", "\"aaaaa bbbbbb\" cccccc ddddddd",
			"asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf aaaaa bbbbbbb",
			"\"aaaaaaaaaaaaaaaaaaaaaabbb ---------- cccccccccccc dddddddddddddddddddddddddd\"",
			"1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32"};
	char *argv[32];
	int i, j;

	for(j=0; j<5; j++){
		get_token( cmd[j], argv, 32 );
		printf("\nj=%d\n", j);
		for(i=0; i<32; i++){
			if(argv[i]!=NULL) printf("%s\n", argv[i]);
			else break;
		}
	}
}
#endif




#if 0
/* テストプログラム */
int main(int argc,char* argv[])
{

	FILE *fpin;
	char buf[1024];
	if( (fpin = fastpopen( "smbclient //43.8.218.171/mizuba -U mizuba%sui2989 -c \"cd \\\"work\\\";ls \\\"yosemite\\\""
, "rb" ))==NULL ) {
		fprintf(stderr, "mypopen error\n");
		return 0;
	}


	while(1)
	{
		if(NULL==fgets(buf,1024,fpin))break;
		printf(">>%s<<\n",buf);
	}
printf("before pclose\n");
	fastpclose(fpin);
printf("after pclose\n");
}
#endif


#if 0
int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow){

	FILE* fp;
	char buf[1024];


	fp=fastpopen("hello_test \" aaaaa \\\" cccc \\\" \"  ","rb");
	fgets(buf,1024,fp);
	fastpkill(fp);
}


#endif



