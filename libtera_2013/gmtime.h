/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/
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
