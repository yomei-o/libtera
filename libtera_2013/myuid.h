/******* BEGIN LICENCE BLOCK *******

    Sony Nasne Application
    Copyright (C) 2012 Sony Corporation.
    Yomei.Otani@jp.sony.com

******* END LICENCE BLOCK *******/


#ifndef df_MYUID_H_
#define df_MYUID_H_

#ifdef __cplusplus
extern "C"
{
#endif

int mygeteuid(void);
int mygetegid(void);
int mygetuid(void);
int mygetgid(void);

int mysetuid(int id);
int mysetgid(int id);
int myseteuid(int id);
int mysetegid(int id);

int mychown(char* name,int uid,int gid);


#ifdef __cplusplus
}
#endif


#endif
	




