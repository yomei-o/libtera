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


/*

$Id: sock_local.h,v 1.2 2008/04/14 02:03:05 akatsuka Exp $

*/

/*****************************************************************************
自動的なCプリプロセッサ定義

#define         プラットフォーム:
-------         ----------
ARM_ULIB        Huw Rogers による Acorn Archimedes 用のフリー Unix ライブラリ
AZTEC_C         Aztec_C 5.2a
__CYGWIN__      Cygwin
_DCC            Dice C (AMIGA 上)
__GNUC__        Gnu CC (および DJGPP)
__EMX__         Gnu C 移植版 (gcc/emx 0.8e) (OS/2 2.0 への移植版)
__HIGHC__       MetaWare High C
__IBMC__        C-Set++ (OS/2 2.1 上)
_MSC_VER        MS VisualC++ 4.2
MWC             Mark Williams C (COHERENT 上)
__MWERKS__      Metrowerks Compiler; Macintosh および WIN32 (?)
_POSIX_SOURCE   ??
_QC             Microsoft QuickC
__STDC__        ANSI C 準拠
__TURBOC__      Turbo C と Borland C
__USE_POSIX     ??
__WATCOMC__     Watcom C (MS-DOS 上)
__ZTC__         Zortech C

_AIX            AIX オペレーティング システム
__APPLE__       Apple Darwin
AMIGA           SAS/C 5.10 または Dice C (AMIGA 上)
__amigaos__     Gnu CC (AMIGA 上)
atarist         ATARI-ST under Gnu CC
__FreeBSD__     FreeBSD
GNUDOS          DJGPP (version 1.08 では obsolete)
__GO32__        DJGPP (future?)
hpux            HP-UX
linux           Linux
macintosh       Macintosh (THINK_C and __MWERKS__ define)
MCH_AMIGA       Aztec_c 5.2a (AMIGA 上)
__MACH__        Apple Darwin
MSDOS           Microsoft C 5.10 および 6.00A
__MSDOS__       Turbo C, Borland C, DJGPP
nosve           Control Data NOS/VE
SVR2            System V Revision 2.
__SVR4          SunOS
THINK_C         Macintosh 用の開発環境
ultrix          VAX with ULTRIX オペレーティング システム
unix            ほとんどの Unix およびその類似のシステム、および DJGPP (!?)
__unix__        Gnu CC と DJGPP
_UNICOS         Cray オペレーティング システム
vaxc            VAX C コンパイラ
VAXC            VAX C コンパイラ
vax11c          VAX C コンパイラ
VAX11           VAX C コンパイラ
_Windows        Borland C 3.1 コンパイラ for Windows
_WIN32          MS VisualC++ 4.2 と Cygwin (Win32 API)
vms             (and VMS) VAX-11 C under VMS.

__alpha         DEC Alpha プロセッサ
__alpha__       DEC Alpha プロセッサ
hp9000s800      HP RISC プロセッサ
__i386__        DJGPP
i386            DJGPP
MULTIMAX        Encore computer
ppc             PowerPC
__ppc__         PowerPC
pyr             Pyramid 9810 プロセッサ
__sgi__         Silicon Graphics Inc.
sparc           SPARC プロセッサ
sequent         Sequent computer
tahoe           CCI Tahoe プロセッサ
vax             VAX プロセッサ
*****************************************************************************/


/*
**
** ここでsocket関数の機種固有の処理を定義します。
** 作者:yomei@sm.sony.co.jp
**
*/
#ifndef SOCK_LOCAL_H
#define SOCK_LOCAL_H


#include <stdio.h>
#include <string.h>

/*ウインドウズ系の処理*/
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
//#include <Iphlpapi.h>
#endif	/* _WIN32 */


/*SHの処理*/
#if (defined _SH2)||(defined _SH4)
#include <trsocket.h>

#define closesocket(s) tfClose(s)
#endif


/*UNIX系の処理*/
#if defined(unix) || defined(ANDROID_NDK)
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/tcp.h>

#ifdef __FreeBSD__
#include <sys/filio.h>
#endif	/* __FreeBSD__ */

#if defined(linux) || defined(ANDROID_NDK)
#include <asm/ioctls.h>
#endif	/* linux */


typedef struct in_addr *LPIN_ADDR;
#define closesocket(s) close(s)

#endif	/* unix */


#ifdef _WIN32
typedef int socklen_t;
#endif

/*SH&UNIX系の処理*/
#if (defined _SH2) || (defined _SH4) || (defined unix)
#define SOCKET int

#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

#ifndef	INADDR_ANY
#define INADDR_ANY              (u_long)0x00000000
#endif
#ifndef	INADDR_NONE
#define INADDR_NONE             0xffffffff
#endif

typedef struct sockaddr* LPSOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void winsock_init(void);
void winsock_done(void);
void winsock_asyncmode(SOCKET s);
void winsock_syncmode(SOCKET s);
int winsock_getwaitingbytes(SOCKET s);
int winsock_getlasterror(SOCKET s);

struct st_mac_address{
	unsigned int hi;
	unsigned int lo;
};

typedef struct st_mac_address STMACADDRESS;
typedef struct st_mac_address* LPSTMACADDRESS;


int  winsock_getipnettable(LPSTMACADDRESS mac_address,unsigned int ip_address);



#ifdef __cplusplus
}
#endif


#endif /* SOCK_LOCAL_H */
