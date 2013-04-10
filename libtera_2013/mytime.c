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

$Log: mytime.c,v $
Revision 1.7  2008/04/14 02:11:48  akatsuka
no message

Revision 1.6  2008/01/18 02:38:28  yomei
no message

Revision 1.5  2008/01/18 01:02:33  yomei
no message

Revision 1.4  2008/01/18 00:48:35  yomei
no message

Revision 1.3  2007/11/06 00:59:19  yomei
no message

Revision 1.2  2007/07/27 00:47:44  tooyama
modify myclock() for UNIX

Revision 1.1  2007/02/09 04:06:42  tooyama
no message

Revision 1.1.1.1  2006/08/17 09:13:20  yokoyama
no message

Revision 1.5  2006/07/18 08:10:34  tooyama
no message

Revision 1.4  2006/07/13 04:03:22  tooyama
no message

Revision 1.3  2006/07/13 02:43:13  tooyama
no message

Revision 1.2  2006/07/12 09:23:45  tooyama
always count up

Revision 1.1  2006/07/06 04:50:14  tooyama
no message


*/
#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif


#include <string.h>
#include "mytime.h"


#ifdef _WIN32

int myclock()
{
	return -((int)clock());
}

#endif


#ifdef unix

/*
** get clock
*/
static unsigned int get_clock()
{
	struct timeval tv;
	struct timezone tz;
	unsigned int t=0;

	if(!gettimeofday(&tv, &tz)) {
		t  = (unsigned int)tv.tv_sec * myclock_sec();
		t += (unsigned int)tv.tv_usec / (1000 * 1000 / myclock_sec());
	}

	return t;
}

//tatic int myclock_flag=0;

/*
** countdown system clock emuration
*/
int myclock()
{
	static int offset = 0;
	static unsigned int last = 0;
	unsigned int t;

	t = get_clock();
	/* adjust time */
	if((t - offset)<last || (t - offset)>(last+myclock_sec()*10)) {
		//printf("hosei!\n");
		offset = t - last - myclock_sec();
	}
	last = t - offset;
	return -last;
}
#endif


#if (defined _SH2)||(defined _SH4)

int myclock()
{
	return GetFreeRun10m000();
}

#endif



#if 0


main()
{
	int i;


	while(1)
	{
			i=myclock();
			printf("%08d\n",-i);
			ymsleep(1000);
	}



}

#endif


