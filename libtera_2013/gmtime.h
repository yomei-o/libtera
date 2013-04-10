/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/
#ifndef  _GMTIME_H_
#define _GMTIME_H_


/*
** 
** gmtime は tm 型の構造体へのポインタを返します。返された構造体のフィールドには timer 引数が評価された値が、現地時刻ではなく、万国標準時 (UTC) で保持されます。この構造体には int 型の以下のフィールドがあります。
** 
** tm_sec
**    秒 (0～59) 
** tm_min
**    分 (0～59) 
** tm_hour
**    時間 (0～23) 
** tm_mday
**    日 (1～31) 
** tm_mon
**    月 (0～11；1 月を 0 とする) 
** tm_year
**    年 (現在の年から 1900 を引いた値) 
** tm_wday
**    曜日 (0～6；日曜日を 0 とする) 
** tm_yday
**    年初からの通算日数 (0～365；1 月 1 日を 0 とする) 
** tm_isdst
**    gmtime 関数の場合には常に0
** gmtime、mktime、localtime の各関数は、静的に割り当てられた単一の構造体を使って結果を保持します。これらの関数を呼び出すごとに、前の呼び出しの結果が破壊されます。timer が 1970 年 1 月 1 日より前の日付を表す場合は、gmtime 関数は NULL を返します。返すべきエラー値はありません。
** 
*/

/*
** timer
** 
**    格納された時刻へのポインタ。時刻は、万国標準時 (UTC) 1970 年 1 月 1 日 00：00：00 からの時間を秒数で表します。
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


/* 一貫秒から時間構造体を取得 :::::::::::::::::::::::::::::::::::::::::::*/
void          gmtime_reltime2datetime(unsigned long rt, struct mytm *dt);
/* 時間構造体から一貫秒を取得 :::::::::::::::::::::::::::::::::::::::::::*/
unsigned long gmtime_datetime2reltime(struct mytm *dt);
/* 文字列14桁の年月日時分秒から一貫秒を取得 :::::::::::::::::::::::::::::*/
unsigned long gmtime_stringdatetime2reltime(char *strdt);


/* 時間構造体から整数への変換 */
unsigned int mytimegm(struct mytm* tm);
/* 整数から時間構造体への変換 ANSI互換 (2036年問題有) */
struct mytm *mygmtime(int *timer);
/* 整数から時間構造体への変換 UNIX互換 (2036年問題有) */
struct mytm *mygmtime_r(int *timer, struct mytm *result);


/* 整数から時間構造体への変換 2100年まで有効版 */
struct mytm *mygmutime(unsigned int *timer);
/* 整数から時間構造体への変換 2100年まで有効版 */
struct mytm *mygmutime_r(unsigned int *timer, struct mytm *result);

/* ANSI準拠時間構造体を表示しやすいように、年を西暦に、月を1から12に変換 */
struct mytm *myadjusttime(struct mytm *result);

/* 表示しやすい時間構造体をANSI準拠の時間構造体に変換 */
struct mytm *myreadjusttime(struct mytm *result);




/* 時間構造体から整数への変換 */
unsigned int mytimelocal(struct mytm* tm);
/* 整数から時間構造体への変換 ANSI互換 (2036年問題有) */
struct mytm *mylocaltime(int *timer);
/* 整数から時間構造体への変換 UNIX互換 (2036年問題有) */
struct mytm *mylocaltime_r(int *timer, struct mytm *result);


/* 整数から時間構造体への変換 2100年まで有効版 */
struct mytm *mylocalutime(unsigned int *timer);
/* 整数から時間構造体への変換 2100年まで有効版 */
struct mytm *mylocalutime_r(unsigned int *timer, struct mytm *result);


int mytime(int *timer);
char *myasctime(const struct mytm *tp);




#ifdef __cplusplus
}
#endif


#endif /* _GMTIME_H_ */
