/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

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
	




