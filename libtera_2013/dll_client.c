/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif


#include <stdio.h>
#ifdef WIN32			/* For Windows */
#include <windows.h>
#endif /* WIN32 */
#if unix
#include <dlfcn.h>
#endif /* WIN32 */

#include "dll_client.h"


//#define ERR_PRINT

#ifdef ERR_PRINT
 #define EPRINT(a) fprintf(stderr,a)
 #define EPRINT2(a,b) fprintf(stderr,a,b)
#else
 #define EPRINT(a) do; while(0)
 #define EPRINT2(a,b) do; while(0)
#endif


/*----------------------------------------------------------*/
/*						Load DLL							*/
/*----------------------------------------------------------*/
void *dll_load(char *dllname)
{
	void *pFunclib=NULL;

	if(dllname==NULL) {
		EPRINT("### dllname is NULL\n");
		return NULL;
	}
#ifdef _WIN32
	pFunclib = LoadLibrary(dllname);
#endif

#ifdef unix
	pFunclib = dlopen(dllname, RTLD_LAZY);
#endif
	if(!pFunclib) {
		EPRINT2("### cannot dll_load >%s<\n",dllname);
	}
	return pFunclib;
}

/*----------------------------------------------------------*/
/*						Access DLL							*/
/*----------------------------------------------------------*/
void *dll_access(void *pDllmod, char *funcname)
{
	void *pDll_func=NULL;

	if(pDllmod==NULL) {
		EPRINT("### dll_access handle is NULL\n");
		return NULL;
	}
	if(funcname==NULL) {
		EPRINT("### dll_access funcname is NULL\n");
		return NULL;
	}
#ifdef _WIN32
	pDll_func = GetProcAddress((HMODULE)pDllmod, funcname); 
#endif

#ifdef unix
	pDll_func = dlsym(pDllmod, funcname);
#endif
	if(!pDll_func) {
		EPRINT2("### cannot dll_access >%s<\n",funcname);
	}
	return pDll_func;
}

/*----------------------------------------------------------*/
/*						Close DLL							*/
/*----------------------------------------------------------*/
void dll_close(void *pDllmod)
{
	if(pDllmod==NULL)return;
#ifdef _WIN32
	FreeLibrary(pDllmod);
#endif

#ifdef unix
	dlclose(pDllmod);
#endif
	return;
}


