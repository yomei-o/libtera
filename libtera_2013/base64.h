/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/
#ifndef _BASE64_H_
#define _BASE64_H_


#ifdef __cplusplus
extern "C"
{
#endif

int b64enc(const unsigned char *src, int len, char *dest);
int b64dec(const unsigned char *src, int len, char *dest);

#ifdef __cplusplus
}
#endif


#endif	/* _BASE64_H_ */
