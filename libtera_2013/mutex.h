/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#ifndef __MUTEX_H__
#define __MUTEX_H__



#ifdef __cplusplus
extern "C"
{
#endif

typedef void* mutex_t;

mutex_t create_mutex(char *name);
void close_mutex(mutex_t vp);
void  get_mutex(mutex_t vp);
void release_mutex(mutex_t vp);
void sleep_mutex(int s);
int is_signal_mutex(mutex_t vp);

#ifdef __cplusplus
}
#endif


#endif /* __MUTEX_H__ */

