/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/
#ifndef _FASTSYSTEM_H_
#define _FASTSYSTEM_H_

#ifdef __cplusplus
extern "C"
{
#endif

int fast_system( const char *cmd );
int fast_nwsystem( const char *cmd );

#ifdef __cplusplus
}
#endif


#endif /* _FASTSYSTEM_H_ */


