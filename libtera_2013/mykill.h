/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#ifndef _MYKILL_H_
#define _MYKILL_H_


#ifdef __cplusplus
extern "C"
{
#endif

#define mykill9(pid) mykill(pid,9)
#define mykill15(pid) mykill(pid,15)

void mykill(int pid,int n);
int mygetpid(void);
void mymysleep(int s);

#ifdef __cplusplus
}
#endif


#endif


