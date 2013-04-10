/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#ifndef _KANJI_H_
#define  _KANJI_H_


#ifdef __cplusplus
extern "C"
{
#endif


unsigned short kj_cp932_to_unicode_c(unsigned short src);
unsigned short  kj_unicode_to_cp932_c(unsigned short src);

unsigned short  kj_jis_to_euc_c(unsigned short c);
unsigned short  kj_euc_to_jis_c(unsigned short c);

unsigned short  kj_jis_to_sjis_c(unsigned short c);
unsigned short  kj_sjis_to_jis_c(unsigned short c);

unsigned short kj_euc_to_unicode_c(unsigned short src);
unsigned short kj_jis_to_unicode_c(unsigned short src);
unsigned short kj_sjis_to_unicode_c(unsigned short src);

unsigned short kj_unicode_to_euc_c(unsigned short src);
unsigned short kj_unicode_to_jis_c(unsigned short src);
unsigned short kj_unicode_to_sjis_c(unsigned short src);

int kj_cp932_to_unicode(unsigned char* src,unsigned short *dst);
int kj_cp932_to_unicode_n(unsigned char* src,unsigned short *dst, int sz);
int kj_euc_to_unicode(unsigned char* src,unsigned short *dst);
int kj_jis_to_unicode(unsigned char* src,unsigned short *dst);
int kj_jis_to_unicode_m(unsigned char* src,unsigned short *dst);
int kj_sjis_to_unicode_n(unsigned char* src,unsigned short *dst,int sz);
int kj_sjis_to_unicode(unsigned char* src,unsigned short *dst);

int kj_unicode_to_cp932_n(unsigned short* src,unsigned char *dst, int sz);
int kj_unicode_to_cp932(unsigned short* src,unsigned char *dst);
int kj_unicode_to_euc(unsigned short* src,unsigned char *dst);
int kj_unicode_to_jis(unsigned short* src,unsigned char *dst);
int kj_unicode_to_sjis_n(unsigned short* src,unsigned char *dst, int sz);
int kj_unicode_to_sjis(unsigned short* src,unsigned char *dst);
int kj_utf8_to_unicode_n(unsigned char* src,unsigned short* dst, int sz);
int kj_utf8_to_unicode(unsigned char* src ,unsigned short* dst);
int kj_unicode_to_utf8(unsigned short* src,unsigned char* dst);

unsigned short kj_utf8_to_unicode_c(int src);
int kj_unicode_to_utf8_c(unsigned short src);


int kj_unicode_to_utf8(unsigned short* src,unsigned char* dst);
int kj_utf8_to_unicode(unsigned char* src,unsigned short* dst);
int kj_unicode_to_utf8_n(unsigned short* src,unsigned char* dst,int n);

unsigned char *kj_utf8strncpyz(unsigned char* dst, unsigned char* src, int sz);

int IsSJISKanji1st(unsigned char c);
int IsSJISKanji2nd(unsigned char c);
int IsSJISKanji(unsigned char c);
char *wstrchr(char *str, char ccode);
char *wstrncpy(char *dst, char *src, size_t len);
char *wstrncpy2toupper1(char *dst, char *src, size_t len);



#ifdef __cplusplus
}
#endif


#endif /* _KANJI_H*/

