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

$Log: gmtime.h,v $
Revision 1.3  2008/08/28 12:21:35  mizuba
no message

Revision 1.2  2008/04/14 02:12:05  akatsuka
no message

Revision 1.1  2007/02/09 04:06:27  tooyama
no message

Revision 1.1.1.1  2006/08/17 09:13:20  yokoyama
no message

Revision 1.2  2006/07/13 04:00:50  tooyama
no message

Revision 1.1  2006/05/25 07:42:11  tooyama
initial

Revision 1.5  2005/06/09 05:21:59  hmura
add gmtime_xxxxxx functions

Revision 1.4  2004/11/09 05:30:34  yokoyama
add myasctime()

Revision 1.3  2003/12/15 09:14:18  yomei
*** empty log message ***

Revision 1.2  2003/12/15 08:59:34  yomei
*** empty log message ***

Revision 1.1  2003/09/17 13:23:04  yomei
Initial revision


*/

#ifndef  _GMTIME_H_
#define _GMTIME_H_


/*
** 
** gmtime �� tm �^�̍\���̂ւ̃|�C���^��Ԃ��܂��B�Ԃ��ꂽ�\���̂̃t�B�[���h�ɂ� timer �������]�����ꂽ�l���A���n�����ł͂Ȃ��A�����W���� (UTC) �ŕێ�����܂��B���̍\���̂ɂ� int �^�̈ȉ��̃t�B�[���h������܂��B
** 
** tm_sec
**    �b (0�`59) 
** tm_min
**    �� (0�`59) 
** tm_hour
**    ���� (0�`23) 
** tm_mday
**    �� (1�`31) 
** tm_mon
**    �� (0�`11�G1 ���� 0 �Ƃ���) 
** tm_year
**    �N (���݂̔N���� 1900 ���������l) 
** tm_wday
**    �j�� (0�`6�G���j���� 0 �Ƃ���) 
** tm_yday
**    �N������̒ʎZ���� (0�`365�G1 �� 1 ���� 0 �Ƃ���) 
** tm_isdst
**    gmtime �֐��̏ꍇ�ɂ͏��0
** gmtime�Amktime�Alocaltime �̊e�֐��́A�ÓI�Ɋ��蓖�Ă�ꂽ�P��̍\���̂��g���Č��ʂ�ێ����܂��B�����̊֐����Ăяo�����ƂɁA�O�̌Ăяo���̌��ʂ��j�󂳂�܂��Btimer �� 1970 �N 1 �� 1 �����O�̓��t��\���ꍇ�́Agmtime �֐��� NULL ��Ԃ��܂��B�Ԃ��ׂ��G���[�l�͂���܂���B
** 
*/

/*
** timer
** 
**    �i�[���ꂽ�����ւ̃|�C���^�B�����́A�����W���� (UTC) 1970 �N 1 �� 1 �� 00�F00�F00 ����̎��Ԃ�b���ŕ\���܂��B
** 
*/


struct mytm
{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};



#ifdef __cplusplus
extern "C"
{
#endif


/* ��ѕb���玞�ԍ\���̂��擾 :::::::::::::::::::::::::::::::::::::::::::*/
void          gmtime_reltime2datetime(unsigned long rt, struct mytm *dt);
/* ���ԍ\���̂����ѕb���擾 :::::::::::::::::::::::::::::::::::::::::::*/
unsigned long gmtime_datetime2reltime(struct mytm *dt);
/* ������14���̔N���������b�����ѕb���擾 :::::::::::::::::::::::::::::*/
unsigned long gmtime_stringdatetime2reltime(char *strdt);


/* ���ԍ\���̂��琮���ւ̕ϊ� */
unsigned int mytimegm(struct mytm* tm);
/* �������玞�ԍ\���̂ւ̕ϊ� ANSI�݊� (2036�N���L) */
struct mytm *mygmtime(int *timer);
/* �������玞�ԍ\���̂ւ̕ϊ� UNIX�݊� (2036�N���L) */
struct mytm *mygmtime_r(int *timer, struct mytm *result);


/* �������玞�ԍ\���̂ւ̕ϊ� 2100�N�܂ŗL���� */
struct mytm *mygmutime(unsigned int *timer);
/* �������玞�ԍ\���̂ւ̕ϊ� 2100�N�܂ŗL���� */
struct mytm *mygmutime_r(unsigned int *timer, struct mytm *result);

/* ANSI�������ԍ\���̂�\�����₷���悤�ɁA�N�𐼗�ɁA����1����12�ɕϊ� */
struct mytm *myadjusttime(struct mytm *result);

/* �\�����₷�����ԍ\���̂�ANSI�����̎��ԍ\���̂ɕϊ� */
struct mytm *myreadjusttime(struct mytm *result);




/* ���ԍ\���̂��琮���ւ̕ϊ� */
unsigned int mytimelocal(struct mytm* tm);
/* �������玞�ԍ\���̂ւ̕ϊ� ANSI�݊� (2036�N���L) */
struct mytm *mylocaltime(int *timer);
/* �������玞�ԍ\���̂ւ̕ϊ� UNIX�݊� (2036�N���L) */
struct mytm *mylocaltime_r(int *timer, struct mytm *result);


/* �������玞�ԍ\���̂ւ̕ϊ� 2100�N�܂ŗL���� */
struct mytm *mylocalutime(unsigned int *timer);
/* �������玞�ԍ\���̂ւ̕ϊ� 2100�N�܂ŗL���� */
struct mytm *mylocalutime_r(unsigned int *timer, struct mytm *result);


int mytime(int *timer);
char *myasctime(const struct mytm *tp);




#ifdef __cplusplus
}
#endif


#endif /* _GMTIME_H_ */
