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
#include <string.h>
#include <stdlib.h>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#define I_USE_PATHOPEN

#ifdef I_USE_PATHOPEN

#include "pathopen.h"

#else

#define fpathopen(a,b) fopen(a,b)

#endif


#ifdef _WIN32
#define PATH_KUGIRI ';'
#else
#define PATH_KUGIRI ':'
#endif

#ifdef _WIN32
#define FILE_KUGIRI '\\'
#else
#define FILE_KUGIRI '/'
#endif


static char *get_path(void)
{
	char *p;
	char *ret;
	int len;
	
	p=getenv("PATH");
	if(p==NULL)return NULL;
	len=strlen(p);
	ret=malloc(len+1);
	if(ret==NULL)return NULL;
	strcpy(ret,p);
	return ret;
}

static char *next_path(char* p)
{

	if(p==NULL)return NULL;
	while(*p!=0 && *p!=PATH_KUGIRI)p++;
	if(*p==0)return NULL;
	p++;
	if(*p==0)return NULL;
	return p;
}

static void release_path(char *p)
{
	if(p==NULL)return;
	free(p);
}

static char* get_top_path(char* p,char* name)
{
	int plen,nlen;
	char *src;
	char *ret;

	if(p==NULL || name==NULL)return NULL;
	if(*p==0)return NULL;
	nlen=strlen(name);

	src=p;
	plen=0;
	while(*src!=0 && *src!=PATH_KUGIRI)
	{
		src++;
		plen++;
	}
	//printf("plen=%d\n",plen);

	ret=malloc(plen+nlen+4);
	if(ret==NULL)return NULL;

	memcpy(ret,p,plen);
	ret[plen]=FILE_KUGIRI;
	strcpy(ret+plen+1,name);
	return ret;
}

static void release_top_path(char* p)
{
	if(p)free(p);
}

FILE* fpathopen(char* name,char* mode)
{
	char *p,*q;
	char* n;
	FILE* fp;

	if(name==0 || name[0]==0)return NULL;
	fp=fopen(name,mode);
	if(fp)return fp;

	p=get_path();
	if(p==NULL)return NULL;

	q=p;
	while((n=get_top_path(q,name)))
	{
		//printf("PATH=##%s##\n",n);
		fp=fopen(n,mode);
		if(fp)
		{
			release_top_path(n);
			release_path(p);
			return fp;
		}
		release_top_path(n);
		q=next_path(q);

	}
	release_path(p);
	return NULL;
}

/*
** PATH変数からファイル名のファイルを探し、絶対パスを返す
** name: ファイル名
** mode: モード
** path: ファイル絶対パス
** pathsz: pathサイズ
** 戻り値: 0 OK, 0以外 NG
*/
int fpathpath(char* name,char* mode,char* path,int pathsz)
{
	char *p=NULL,*q=NULL;
	char* n=NULL;
	FILE* fp=NULL;
	int len;

	if(name==NULL || mode==NULL)
		return 1;
	if(path==NULL || pathsz<=0)
		return 1;
	*path=0;

	fp=fopen(name,mode);
	if(fp) {
		fclose(fp);
		len=strlen(name);
		if(len>=pathsz)
			len=pathsz-1;
		strncpy(path,name,len);
		path[len]=0;
		return 0;
	}

	p=get_path();
	if(p==NULL)return 1;

	q=p;
	while((n=get_top_path(q,name)))
	{
		//printf("PATH=##%s##\n",n);
		fp=fopen(n,mode);
		if(fp)
		{
			fclose(fp);
			len=strlen(n);
			if(len>=pathsz)
				len=pathsz-1;
			strncpy(path,n,len);
			path[len]=0;
			release_top_path(n);
			release_path(p);
			return 0;
		}
		release_top_path(n);
		q=next_path(q);

	}
	release_path(p);
	return 1;
}



static void gettmp( char *fol, char *buf )
{
	char*	p ;

#if 1				//	folder指定がフルパスの時
	if ( fol == NULL )
	{
		p = getenv( "TEMP" ) ;
		if ( p == NULL )
		{
			p = getenv( "TMP" ) ;
			if ( p == NULL )
			{
				strcpy( buf, "/tmp/" ) ;
				return ;
			}
		}
		strcpy( buf, p ) ;

		p = buf ;
		while (( p=strchr( p, '\\' )) != NULL )
		{
			*p = '/' ;
			p++ ;
		}
	}
	else
	{
		strcpy( buf, fol ) ;
	}
	p = strchr( buf, '\0' ) ;
	if ( *(p-1) != '/' )
	{
		strcat( buf, "/" ) ;
	}
#else				//	folder指定が TEMP以下の相対指定時
#if defined(_WIN32)
	strcpy( buf, "." ) ;
#else
	p = getenv( "TEMP" ) ;
	if ( p == NULL )
	{
		p = getenv( "TMP" ) ;
		if ( p == NULL )
		{
			strcpy( buf, StrTempDir ) ;
			return ;
		}
	}
	strcpy( buf, p ) ;

	p = buf ;
	while (( p=strchr( p, '\\' )) != NULL )
	{
		*p = '/' ;
		p++ ;
	}
#endif

	p = strchr( buf, '\0' ) ;
	if ( *(p-1) != '/' )
	{
		strcat( buf, "/" ) ;
	}

	if ( fol != NULL )
	{
		strcat( buf, fol ) ;
	}
	p = strchr( buf, '\0' ) ;
	if ( *(p-1) != '/' )
	{
		strcat( buf, "/" ) ;
	}
#endif
}


FILE* ftmpopen(char* name,char* mode)
{
	char tmp[4096];
	gettmp(NULL,tmp);
	strcat(tmp,name);
	return fopen(tmp,mode);
}


FILE* ftmppathopen(char* name,char* mode)
{
	FILE* ret;
	
	ret=ftmpopen(name,mode);
	if(ret)return ret;
	return fpathopen(name,mode);
}



#if 0
#ifdef _CONSOLE

main()
{
	FILE* fp;

#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//fp=fpathopen("bbb.txt","rb");
	fp=ftmppathopen("bbb.txt","rb");
	printf("FP=%p\n",fp);
	if(fp)fclose(fp);
}
#endif
#endif

#if 0

int main()
{
	int ret=0;
	char path[1024];

#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	ret=fpathpath("calc.exe","rb",path,sizeof(path));
	printf("ret=%d\n",ret);
	if(!ret){
	printf("path=[%s]\n",path);
	}
	getchar();
	return 0;
}
#endif


