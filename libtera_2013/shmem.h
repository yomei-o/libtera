/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#ifndef _SHMEM_H_
#define _SHMEM_H_


#ifdef __cplusplus
extern "C"
{
#endif

typedef void* share_t;

share_t create_share(char *name,int sz);
share_t open_share(char *name,int sz);
int close_share(share_t h);

share_t map_share(share_t h);
void unmap_share(share_t vp);

int lock_share(share_t vp);
int unlock_share(share_t vp);



#ifdef __cplusplus
}
#endif


#endif /* _SHMEM_H_ */
