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

$Log: mytime.h,v $
Revision 1.2  2008/04/14 02:12:05  akatsuka
no message

Revision 1.1  2007/02/09 04:06:28  tooyama
no message

Revision 1.1.1.1  2006/08/17 09:13:20  yokoyama
no message

Revision 1.3  2006/07/18 08:10:20  tooyama
no message

Revision 1.2  2006/07/13 04:01:07  tooyama
no message

Revision 1.1  2006/07/06 04:50:03  tooyama
no message

Revision 1.3  2006/06/28 11:55:02  tooyama
no message

Revision 1.2  2006/05/23 12:25:35  tooyama
no message

Revision 1.1  2006/05/23 10:50:34  tooyama
no message


*/


/*
**
**  カウントダウンシステムクロックエミュレーション
**
**　このライブラリはカウントダウンシステムクロックタイマのエミュレーション
**  ライブラリです。ここを機種ごとに変更することにより機種固有時間を求めます。
**
**  作者 yomei@sm.sony.co.jp
*/

#ifndef MYTIME_H
#define MYTIME_H


#ifdef _WIN32

#include <time.h>
/*1秒あたりのクロック数*/
#define myclock_sec() (CLK_TCK)

#endif

#ifdef unix

#include <time.h>
#include <sys/time.h>
/*1秒あたりのクロック数*/
#define myclock_sec() (1000)

#endif

#if (defined _SH2)||(defined _SH4)

#include "timer.h"
/*1秒あたりのクロック数*/
#define myclock_sec() (TIME1SOF10M000)

#endif


#ifdef __cplusplus
extern "C"
{
#endif

/*カウントダウンシステムクロックのエミュレーション*/
int myclock();

#ifdef __cplusplus
}
#endif


#endif /* MYTIME_H */
