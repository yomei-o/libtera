/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

 
#ifndef _DLL_CLIENT_H_
#define _DLL_CLIENT_H_


#define LIB_OK                0

#define LIB_ERR_INVALID_REQUEST     (-1)
#define LIB_ERR_INVALID_PARAMETER   (-2)
#define LIB_ERR_NOSERVICE           (-3)
#define LIB_ERR_NOREQUEST           (-4)


#if defined(_WIN32) && !defined(__GNUC__)
#define LIBEXT ".dll"
#else
#define LIBEXT ".so"
#endif

#define LIBNAME(a) #a LIBEXT


#ifdef __cplusplus
extern "C"
{
#endif


void *dll_load(char *dllname);
void *dll_access(void *pDllmod, char *funcname);
void dll_close(void *pDllmod);


#ifdef __cplusplus
}
#endif



#endif


