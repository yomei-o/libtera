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


#if (defined _SH4)||(defined PJ_CANALTMR)
#include "api.h"
#else
#include <time.h>
#endif

#include "gmtime.h"


#define _DAY_SEC           (24L * 60L * 60L)    /* secs in a day */
#define _YEAR_SEC          (365L * _DAY_SEC)    /* secs in a year */
#define _FOUR_YEAR_SEC     (1461L * _DAY_SEC)   /* secs in a 4 year interval */
#define _DEC_SEC           315532800L           /* secs in 1970-1979 */
#define _BASE_YEAR         70L                  /* 1970 is the base year */
#define _BASE_DOW          4                    /* 01-01-70 was a Thursday */
#define _LEAP_YEAR_ADJUST  17L                  /* Leap years 1900 - 1970 */
#define _MAX_YEAR          138L                 /* 2038 is the max year */

#define _BASE_YEAR_FULL 1970

/*
static int _niti[]={
	0,
	31,
	31+28,
	31+28+31,
	31+28+31+30,
	31+28+31+30+31,
	31+28+31+30+31,
	31+28+31+30+31+30,
	31+28+31+30+31+30+31,
	31+28+31+30+31+30+31+31,
	31+28+31+30+31+30+31+31+30,
	31+28+31+30+31+30+31+31+30+31,
	31+28+31+30+31+30+31+31+30+31+30,
};
*/

static int _lpdays[] = {
        -1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

static int _days[] = {
        -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364
};

static struct mytm mytm_tm;





/*****************************************************************************/
/* convert relative-time to date-time                                        */
/*****************************************************************************/
void gmtime_reltime2datetime(unsigned long rt, struct mytm *dt)
{
    if(dt == NULL) { return; }
    mygmutime_r((void *)&rt, dt);
    myadjusttime(dt);
}

/*****************************************************************************/
/* convert date-time to relative-time                                        */
/*****************************************************************************/
unsigned long gmtime_datetime2reltime(struct mytm *dt)
{
    if(dt == NULL) { return 0; }
    myreadjusttime(dt);
    return mytimegm(dt);
}

/*****************************************************************************/
/* convert string date-time to relative-time                                 */
/* format is "20051231235959"                                                */
/*****************************************************************************/
unsigned long gmtime_stringdatetime2reltime(char *strdt)
{
    struct mytm dt;

    char  buf[5];
    char *ptr;
    int   len;

    if(strdt == NULL) { return 0; }

    ptr = strdt;
    len=4; memcpy(buf,ptr,len); buf[len]='\0'; dt.tm_year=atoi(buf); ptr+=len;
    len=2; memcpy(buf,ptr,len); buf[len]='\0'; dt.tm_mon =atoi(buf); ptr+=len;
    len=2; memcpy(buf,ptr,len); buf[len]='\0'; dt.tm_mday=atoi(buf); ptr+=len;
    len=2; memcpy(buf,ptr,len); buf[len]='\0'; dt.tm_hour=atoi(buf); ptr+=len;
    len=2; memcpy(buf,ptr,len); buf[len]='\0'; dt.tm_min =atoi(buf); ptr+=len;
    len=2; memcpy(buf,ptr,len); buf[len]='\0'; dt.tm_sec =atoi(buf); ptr+=len;
    myreadjusttime(&dt);
    return mytimegm(&dt);;
}





unsigned int mytimegm(struct mytm* tm)
{
	unsigned int day_dst,day_src,y,t;


	if(tm->tm_year<70 || tm->tm_year>199)return -1;
	if(tm->tm_mon<0 || tm->tm_mon>11)return -1;
	if(tm->tm_mday<1 || tm->tm_mday>31)return -1;

	if(tm->tm_hour<0 || tm->tm_hour>23)return -1;
	if(tm->tm_min<0 || tm->tm_min>59)return -1;
	if(tm->tm_sec<0 || tm->tm_sec>59)return -1;


	y=tm->tm_year-1+1900;
	day_dst=365*y+y/4-y/100+y/400;
	day_src=365*(_BASE_YEAR_FULL-1)+(_BASE_YEAR_FULL-1)/4
		-(_BASE_YEAR_FULL-1)/100+(_BASE_YEAR_FULL-1)/400;
	t=day_dst-day_src;
	//t+=niti[tm->tm_mon];
	t+=_days[tm->tm_mon]+1;
	if( (tm->tm_mon)>=2 && (tm->tm_year+1900)%4==0)
	{
		t++;
		if( (tm->tm_year+1900)%100==0 && (tm->tm_year+1900)%400!=0 )t--;
	}
	t+=tm->tm_mday-1;
	t*=86400;

	t+=tm->tm_hour*3600;
	t+=tm->tm_min*60;
	t+=tm->tm_sec;
	return t;
}


struct mytm *mygmtime(int *timp)
{
	return mygmtime_r(timp,&mytm_tm);
}



struct mytm *mygmtime_r(int *timp, struct mytm *result)
{
        long caltim;  /* calendar time to convert */
        int islpyr;    /* is-current-year-a-leap-year flag */
        int tmptim;
        int *mdays;                /* pointer to days or lpdays */
		struct mytm *ptb;

		if(result==NULL || timp==NULL)return NULL;


        caltim = *timp;            /* calendar time to convert */
        islpyr = 0;                 /* is-current-year-a-leap-year flag */


        ptb = result;

        if(caltim<0)return NULL;

        /*
         * Determine years since 1970. First, identify the four-year interval
         * since this makes handling leap-years easy (note that 2000 IS a
         * leap year and 2100 is out-of-range).
         */
        tmptim = (int)(caltim / _FOUR_YEAR_SEC);
        caltim -= ((long)tmptim * _FOUR_YEAR_SEC);

        /*
         * Determine which year of the interval
         */
        tmptim = (tmptim * 4) + 70;         /* 1970, 1974, 1978,...,etc. */

        if ( caltim >= _YEAR_SEC ) {

            tmptim++;                       /* 1971, 1975, 1979,...,etc. */
            caltim -= _YEAR_SEC;

            if ( caltim >= _YEAR_SEC ) {

                tmptim++;                   /* 1972, 1976, 1980,...,etc. */
                caltim -= _YEAR_SEC;

                /*
                 * Note, it takes 366 days-worth of seconds to get past a leap
                 * year.
                 */
                if ( caltim >= (_YEAR_SEC + _DAY_SEC) ) {

                        tmptim++;           /* 1973, 1977, 1981,...,etc. */
                        caltim -= (_YEAR_SEC + _DAY_SEC);
                }
                else {
                        /*
                         * In a leap year after all, set the flag.
                         */
                        islpyr++;
                }
            }
        }

        /*
         * tmptim now holds the value for tm_year. caltim now holds the
         * number of elapsed seconds since the beginning of that year.
         */
        ptb->tm_year = tmptim;

        /*
         * Determine days since January 1 (0 - 365). This is the tm_yday value.
         * Leave caltim with number of elapsed seconds in that day.
         */
        ptb->tm_yday = (int)(caltim / _DAY_SEC);
        caltim -= (long)(ptb->tm_yday) * _DAY_SEC;

        /*
         * Determine months since January (0 - 11) and day of month (1 - 31)
         */
        if ( islpyr )
            mdays = _lpdays;
        else
            mdays = _days;


        for ( tmptim = 1 ; mdays[tmptim] < ptb->tm_yday ; tmptim++ ) ;

        ptb->tm_mon = --tmptim;

        ptb->tm_mday = ptb->tm_yday - mdays[tmptim];

        /*
         * Determine days since Sunday (0 - 6)
         */
        ptb->tm_wday = ((int)(*timp / _DAY_SEC) + _BASE_DOW) % 7;

        /*
         *  Determine hours since midnight (0 - 23), minutes after the hour
         *  (0 - 59), and seconds after the minute (0 - 59).
         */
        ptb->tm_hour = (int)(caltim / 3600);
        caltim -= (long)ptb->tm_hour * 3600L;

        ptb->tm_min = (int)(caltim / 60);
        ptb->tm_sec = (int)(caltim - (ptb->tm_min) * 60);

        ptb->tm_isdst = 0;
        return ptb;

}




struct mytm *mygmutime(unsigned int *timp)
{
	return mygmutime_r(timp,&mytm_tm);
}



struct mytm *mygmutime_r(unsigned int *timp, struct mytm *result)
{
        unsigned long caltim;  /* calendar time to convert */
        unsigned int islpyr;    /* is-current-year-a-leap-year flag */
        unsigned int tmptim;
        unsigned int *mdays;                /* pointer to days or lpdays */
		struct mytm *ptb;

		if(result==NULL || timp==NULL)return NULL;

        caltim = *timp;            /* calendar time to convert */
        islpyr = 0;                 /* is-current-year-a-leap-year flag */


        ptb = result;




        /*
         * Determine years since 1970. First, identify the four-year interval
         * since this makes handling leap-years easy (note that 2000 IS a
         * leap year and 2100 is out-of-range).
         */
        tmptim = (unsigned int)(caltim / _FOUR_YEAR_SEC);
        caltim -= ((unsigned long)tmptim * _FOUR_YEAR_SEC);

        /*
         * Determine which year of the interval
         */
        tmptim = (tmptim * 4) + 70;         /* 1970, 1974, 1978,...,etc. */

        if ( caltim >= _YEAR_SEC ) {

            tmptim++;                       /* 1971, 1975, 1979,...,etc. */
            caltim -= _YEAR_SEC;

            if ( caltim >= _YEAR_SEC ) {

                tmptim++;                   /* 1972, 1976, 1980,...,etc. */
                caltim -= _YEAR_SEC;

                /*
                 * Note, it takes 366 days-worth of seconds to get past a leap
                 * year.
                 */
                if ( caltim >= (_YEAR_SEC + _DAY_SEC) ) {

                        tmptim++;           /* 1973, 1977, 1981,...,etc. */
                        caltim -= (_YEAR_SEC + _DAY_SEC);
                }
                else {
                        /*
                         * In a leap year after all, set the flag.
                         */
                        islpyr++;
                }
            }
        }

        /*
         * tmptim now holds the value for tm_year. caltim now holds the
         * number of elapsed seconds since the beginning of that year.
         */
        ptb->tm_year = tmptim;

        /*
         * Determine days since January 1 (0 - 365). This is the tm_yday value.
         * Leave caltim with number of elapsed seconds in that day.
         */
        ptb->tm_yday = (int)(caltim / _DAY_SEC);
        caltim -= (unsigned long)(ptb->tm_yday) * _DAY_SEC;

        /*
         * Determine months since January (0 - 11) and day of month (1 - 31)
         */
        if ( islpyr )
            mdays = (unsigned int *)_lpdays;
        else
            mdays = (unsigned int *)_days;


        for ( tmptim = 1 ; (int)(mdays[tmptim]) < ptb->tm_yday ; tmptim++ ) ;

        ptb->tm_mon = --tmptim;

        ptb->tm_mday = ptb->tm_yday - mdays[tmptim];

        /*
         * Determine days since Sunday (0 - 6)
         */
        ptb->tm_wday = ((int)(*timp / _DAY_SEC) + _BASE_DOW) % 7;

        /*
         *  Determine hours since midnight (0 - 23), minutes after the hour
         *  (0 - 59), and seconds after the minute (0 - 59).
         */
        ptb->tm_hour = (int)(caltim / 3600);
        caltim -= (long)ptb->tm_hour * 3600L;

        ptb->tm_min = (int)(caltim / 60);
        ptb->tm_sec = (int)(caltim - (ptb->tm_min) * 60);

        ptb->tm_isdst = 0;
        return ptb;

}

struct mytm *myadjusttime(struct mytm *result)
{
	if(result==NULL)return NULL;

	result->tm_year+=1900;
	result->tm_mon+=1;

	return result;
}


struct mytm *myreadjusttime(struct mytm *result)
{
	if(result==NULL)return NULL;

	result->tm_year-=1900;
	result->tm_mon-=1;

	return result;
}




/* 時間構造体から整数への変換 */
unsigned int mytimelocal(struct mytm* tm)
{
	return mytimegm(tm);
}
/* 整数から時間構造体への変換 ANSI互換 (2036年問題有) */
struct mytm *mylocaltime(int *timer)
{
	return mygmtime(timer);
}
/* 整数から時間構造体への変換 UNIX互換 (2036年問題有) */
struct mytm *mylocaltime_r(int *timer, struct mytm *result)
{
	return mygmtime_r(timer,result);
}

/* 整数から時間構造体への変換 2100年まで有効版 */
struct mytm *mylocalutime(unsigned int *timer)
{
	return mygmutime(timer);
}
/* 整数から時間構造体への変換 2100年まで有効版 */
struct mytm *mylocalutime_r(unsigned int *timer, struct mytm *result)
{
	return mygmutime_r(timer,result);
}


int mytime(int *timer)
{
#if (defined _SH4)||(defined PJ_CANALTMR)
	int ret,val;
	ret=CanalApi(CNLAPI_SERV_EVENT,CNLAPI_REQ_GET_RTC,&val,sizeof(val));
	if(ret!=CNL_OK)
	{
		if(timer)*timer=0;
		return 0;
	}
	if(timer)*timer=val;
	return val;
#else
	return (int)(time((time_t *)timer));	/* Time Zoneを加算 */
#endif /* _SH4 */
}

static const char *ab_day_name[] =
{"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static const char *ab_month_name[] =
{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
static const char format[] = "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n";
static char result[	         3+1+ 3+1+20+1+20+1+20+1+20+1+20+1 + 1];

char *myasctime(const struct mytm *tp)
{
	if (tp == NULL)
	{
		return NULL;
	}
	
	if (sprintf (result, format,
		(tp->tm_wday < 0 || tp->tm_wday >= 7 ?
		"???" : ab_day_name[tp->tm_wday]),
		(tp->tm_mon < 0 || tp->tm_mon >= 12 ?
		"???" : ab_month_name[tp->tm_mon]),
		tp->tm_mday, tp->tm_hour, tp->tm_min,
		tp->tm_sec, 1900 + tp->tm_year) < 0)
	{
		return NULL;
	}
	return result;
}


/*
** テストパターン
*/

#if 0



main()
{
	unsigned int c,ret;
	struct mytm a;
	struct mytm *b;
	int i;


	for(i=1970;i<2150;i++)
	{
		memset(&a,0,sizeof(struct mytm));
		
		a.tm_year=i-1900;
		a.tm_mon=0;
		a.tm_mday=1;
		c=mytimegm(&a);
		b=mygmutime(&c);
		printf("year=%d  ",i);
		if(b){
		printf("year=%4d  ",b->tm_year+1900);
		printf("mon=%2d  ",b->tm_mon);
		printf("day=%2d  ",b->tm_mday);
		printf("hour=%2d  ",b->tm_hour);
		printf("min=%2d  ",b->tm_min);
		printf("sec=%2d  ",b->tm_sec);
		}
		printf("\n");
	}
}


#endif

#if 0

#include <time.h>

main()
{
	unsigned int c,ret;
	struct mytm a;
	struct tm *b;
	int i;


	for(i=1970;i<2150;i++)
	{
		memset(&a,0,sizeof(struct mytm));
		
		a.tm_year=i-1900;
		a.tm_mon=2;
		a.tm_mday=1;
		c=mytimegm(&a);
		b=gmtime(&c);
		printf("year=%d  ",i);
		if(b){
		printf("year=%4d  ",b->tm_year+1900);
		printf("mon=%2d  ",b->tm_mon);
		printf("day=%2d  ",b->tm_mday);
		printf("hour=%2d  ",b->tm_hour);
		printf("min=%2d  ",b->tm_min);
		printf("sec=%2d  ",b->tm_sec);
		}
		printf("\n");
	}
}


#endif
