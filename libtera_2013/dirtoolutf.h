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


 
/* the following line is the modification history by RCS
   don't edit this line

$Log: dirtool.h,v $
Revision 1.13  2008/04/14 02:12:05  akatsuka
no message

Revision 1.12  2008/01/28 10:01:46  tooyama
my_utime

Revision 1.11  2007/10/18 03:48:33  tooyama
add my_rm
add my_wrm
add myrename

Revision 1.10  2007/07/31 06:42:12  matsu
no message

Revision 1.9  2007/07/12 12:59:19  matsu
no message

Revision 1.8  2007/07/11 09:26:13  matsu
no message

Revision 1.7  2007/07/11 09:11:56  matsu
no message

Revision 1.6  2007/07/11 08:09:40  matsu
no message

Revision 1.5  2007/07/11 01:59:55  yomei
no message


*/


#ifndef _DIRTOOL_H_
#define _DIRTOOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#endif

#ifdef unix
#include <sys/types.h>
#include <sys/stat.h> 
#include <dirent.h>

#include <unistd.h>
#include <sys/param.h>
#endif

#include <time.h>

#define MYDIR_FLAG_FIRST 1
#define MYDIR_FLAG_NEXT  0

struct mydir
{
	int flag;
#ifdef _WIN32
	int h;
	struct _finddata_t fileinfo;
#endif
#ifdef unix
	DIR* dir;
#endif
};

typedef struct mydir MYDIR;
typedef struct mydir* LPMYDIR;

struct mystat
{
	unsigned short	mode;
	unsigned int	size;
	unsigned int	atime;
	unsigned int	mtime;
	unsigned int	ctime;
};
typedef struct mystat MYSTAT;

struct myutimbuf
{
	unsigned int	actime;
	unsigned int	modtime;
};
typedef struct myutimbuf MYUTIMBUF;

/* mystat.modeの定義 */
#ifdef _WIN32	 
#define MYS_IFMT	_S_IFMT		/* ファイル種別を示すビット領域を表すビットマスク */
#define MYS_IFDIR	_S_IFDIR	/* ディレクトリ */
#define MYS_IFCHR	_S_IFCHR	/* キャラクター・デバイス */
#define MYS_IFIFO	_S_IFIFO	/* FIFO */
#define MYS_IFREG	_S_IFREG	/* 通常のファイル */
#define MYS_IRUSR	_S_IREAD	/* 所有者の読み込み許可 */
#define MYS_IWUSR	_S_IWRITE	/* 所有者の書き込み許可 */
#define MYS_IXUSR	_S_IEXEC	/* 所有者の実行許可 */
#endif
#ifdef unix
#define MYS_IFMT	S_IFMT		/* ファイル種別を示すビット領域を表すビットマスク */
#define MYS_IFDIR	S_IFDIR		/* ディレクトリ */
#define MYS_IFCHR	S_IFCHR		/* キャラクター・デバイス */
#define MYS_IFIFO	S_IFIFO		/* FIFO */
#define MYS_IFREG	S_IFREG		/* 通常のファイル */
#define MYS_IRUSR	S_IRUSR		/* 所有者の読み込み許可 */
#define MYS_IWUSR	S_IWUSR		/* 所有者の書き込み許可 */
#define MYS_IXUSR	S_IXUSR		/* 所有者の実行許可 */
#endif

	 
/* mystat.modeのマクロ */
#define	MYS_ISTYPE(mode, mask)	(((mode) & MYS_IFMT) == (mask))
#define	MYS_ISDIR(mode)	 MYS_ISTYPE((mode), MYS_IFDIR)	/* ディレクトリか?    */
#define	MYS_ISCHR(mode)	 MYS_ISTYPE((mode), MYS_IFCHR)	/* キャラクター・デバイスか? */
#define	MYS_ISREG(mode)	 MYS_ISTYPE((mode), MYS_IFREG)	/* 通常のファイルか?  */
#define MYS_ISFIFO(mode)	MYS_ISTYPE((mode), MYS_IFIFO)	/* FIFO (名前付きパイプ) か? */

	 
#ifdef __cplusplus
extern "C"
{
#endif


MYDIR* my_open_dir(char* filen);
void my_close_dir(MYDIR* vp);
int my_read_dir(MYDIR* vp,char* buf,int len);
int my_read_dir2(MYDIR* vp,char* buf,int len,int* flag);
int my_isown_dir(MYDIR *vp,char* buf);
int my_isparent_dir(MYDIR *vp,char*buf);
int my_ischid_dir(MYDIR *vp,char*buf);
int my_mkdir( char *dirname );
int my_rmdir( char *dirname );

int my_isdir( char *path );
int my_chdir( char *dirname );
int my_dirupper( char *dirname );
int my_stat( char *path, MYSTAT *buf );
int my_fstat( int filedes, MYSTAT *buf );

int my_rm( char *filename );
int myrename( char *oldname, char *newname );

#ifdef __cplusplus
}
#endif


#endif /* _DIRTOOL_H_ */
