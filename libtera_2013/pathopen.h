/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#ifndef _PATH_OPEN_H_
#define _PATH_OPEN_H_


#ifdef __cplusplus
extern "C"
{
#endif


FILE* fpathopen(char* name,char* mode);
int fpathpath(char* name,char* mode,char* path,int pathsz);
FILE* ftmpopen(char* name,char* mode);
FILE* ftmppathopen(char* name,char* mode);


#ifdef __cplusplus
}
#endif


#endif



