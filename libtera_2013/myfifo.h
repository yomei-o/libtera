/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/


#ifndef df_MYFIFO_H_
#define df_MYFIFO_H_

#ifdef __cplusplus
extern "C"
{
#endif

void* myfifo_open_read(char* name);
void* myfifo_open_write(char* name);
void myfifo_close(void* vp);
int myfifo_write(void* vp,void* buf,int sz);
int myfifo_read(void* vp,void* buf,int sz);
void myfifo_sleep(int s);

#ifdef __cplusplus
}
#endif


#endif
	




