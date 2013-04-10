/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#ifndef st_FBLOCKING_H_
#define st_FBLOCKING_H_

#include <stdio.h>


#ifdef __cplusplus
extern "C"
{
#endif

void fset_nonblocking(FILE* f_in);
void fset_blocking(FILE* f_out);

int fcygpipe_read(void*buf,size_t a,size_t b,FILE* fp);
int fcygpipe_write(void*buf,size_t a,size_t b,FILE* fp);
int fcygpipe_flush(FILE* fp);

#ifdef __cplusplus
}
#endif




#endif /* st_FBLOCKING_H_ */