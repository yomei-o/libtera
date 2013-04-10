/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/
#ifndef __PTHREAD_H__
#define __PTHREAD_H__

#ifdef __cplusplus
extern "C"
{
#endif


void my_pthread_create(void (*func)(void*), void *arg);

//ph_thread* my_pthread_create(void* func);
//void my_pthread_join( ph_thread *id );
//void my_pthread_exit(void);

#ifdef __cplusplus
}
#endif


#endif /* __PTHREAD_H__ */
