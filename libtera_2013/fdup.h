/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

 
#ifndef _FDUP_H_
#define _FDUP_H_

#include <stdio.h>


#ifdef __cplusplus
extern "C"
{
#endif


FILE* fdup(FILE* fp,char*mode);
FILE *fdup2( FILE *oldfp, FILE *newfp );
void change_stdout(FILE* fp);
void change_stdint(FILE* fp);
void change_stderr(FILE* fp);
int fpipe( FILE **fpipe_r, FILE **fpipe_w );


#ifdef __cplusplus
}
#endif


#endif /* _FDUP_H_ */


