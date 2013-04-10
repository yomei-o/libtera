/******* BEGIN LICENCE BLOCK *******

    Sony Digital Photo Frame Application
    Copyright (C) 2008 Sony Corporation.

    This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License,version 2, as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses>.

    Linking Sony Digital Photo Frame Application statically or dynamically with other modules is making a combined work based on Sony Digital Photo Frame Application. Thus, the terms and conditions of the GNU General Public License cover the whole combination.

    In addition, as a special exception, the copyright holders of Sony Digital Photo Frame Application give you permission to combine Sony Digital Photo Frame Application with free software programs or libraries that are released under the GNU LGPL and with code included in the standard release of Sony OS Abstraction Library (or modified versions of such code, with unchanged license). You may copy and distribute such a system following the terms of the GNU GPL for Sony Digital Photo Frame Application and the licenses of the other code concerned{, provided that you include the source code of that other code when and as the GNU GPL requires distribution of source code}.

    Note that people who make modified versions of Sony Digital Photo Frame Application are not obligated to grant this special exception for their modified versions; it is their choice whether to do so. The GNU General Public License gives permission to release a modified version without this exception; this exception also makes it possible to release a modified version which carries forward this exception.

******* END LICENCE BLOCK *******/



/* the following line is the modification history by RCS
   don't edit this line

$Log: kanji.c,v $
Revision 1.4  2008/06/09 01:53:20  mizuba
no message

Revision 1.3  2008/04/14 02:11:48  akatsuka
no message

Revision 1.2  2007/11/06 00:59:10  yomei
no message

Revision 1.1  2007/02/09 04:06:42  tooyama
no message

Revision 1.2  2006/10/31 11:06:27  hmura
modify comment

Revision 1.1.1.1  2006/08/17 09:13:20  yokoyama
no message

Revision 1.1  2006/07/27 06:29:27  tooyama
initial

Revision 1.10  2006/03/08 07:30:43  hmura
add kj_utf8strncpyz()

Revision 1.9  2006/01/24 06:10:30  hmura
add kj_xxxx_n() functions

Revision 1.8  2005/08/16 07:34:00  hmura
add wstrchr()

Revision 1.7  2005/01/27 05:38:12  hmura
add wstrncpy2toupper1()

Revision 1.6  2004/08/26 19:29:47  hmura
add wstrncpy()

Revision 1.5  2004/08/26 18:01:19  hmura
not modify

Revision 1.4  2004/07/20 01:52:20  tooyama
modify kj_cp932_to_unicode_c()

Revision 1.3  2003/10/07 07:02:11  tooyama
not to go to main()

Revision 1.2  2003/10/06 02:05:27  yomei
*** empty log message ***

Revision 1.1  2003/10/05 09:37:15  yomei
Initial revision


*/

#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kanji.h"
#include "kanjitbl.h"



unsigned short kj_cp932_to_unicode_c(unsigned short src)
{
		if(src<0x100)
		{
			return cp932touni0[src];
		}
		else if(src>=0x8000 && src<0xa000)
		{
			return cp932touni1[src-0x8000];
		}
		else if(src>=0xe000 && src<0xffff)
		{
			return cp932touni2[src-0xe000];
		}
		return 0;
}

unsigned short  kj_unicode_to_cp932_c(unsigned short src)
{
		if(src<0x1000)
		{
			return unitocp932_0[src];
		}
		else if(src>=0x2000 && src<0xa000)
		{
			return unitocp932_1[src-0x2000];
		}
		else if(src>=0xf000 && src<0xffff)
		{
			return unitocp932_2[src-0xf000];
		}
	return 0;
}



unsigned short  kj_jis_to_euc_c(unsigned short c)
{
	if(c<0x80)return c;
	return c|0x8080;
}

unsigned short  kj_euc_to_jis_c(unsigned short c)
{
	if(c<0x80)return c;
	return c&0x7f7f;
}

unsigned short  kj_jis_to_sjis_c(unsigned short c)
{
    int    c2,c1,o1,o2;

	if(c<0x80)return c;

    c2=(c>>8) & 255;
	c1=c&255;

    o2=((((c2 - 1) >> 1) + ((c2 <= 0x5e) ? 0x71 : 0xb1)));
    o1=((c1 + ((c2 & 1) ? ((c1 < 0x60) ? 0x1f : 0x20) : 0x7e)));
	
	return (o2<<8)|o1;
}





unsigned short kj_sjis_to_jis_c(unsigned short c)
{
    int    c2,c1;

	if(c<0x80)return c;

    c2=(c>>8) & 255;
	c1=c&255;



    c2 = c2 + c2 - ((c2 <= 0x9f) ? 0x00e1 : 0x0161);

    if (c1 < 0x9f)
        c1 = c1 - ((c1 > 0x7f) ? 0x20 : 0x1f);
    else {
        c1 = c1 - 0x7e;
        c2++;
    }

	return (c2<<8)|c1;

}





unsigned short kj_euc_to_unicode_c(unsigned short src)
{
		src=(int)kj_euc_to_jis_c(src);
		src=(int)kj_jis_to_sjis_c(src);
		return kj_cp932_to_unicode_c(src);
}

unsigned short kj_jis_to_unicode_c(unsigned short src)
{
		src=kj_jis_to_sjis_c(src);
		return kj_cp932_to_unicode_c(src);
}

unsigned short kj_sjis_to_unicode_c(unsigned short src)
{
		return kj_cp932_to_unicode_c(src);
}

unsigned short kj_unicode_to_euc_c(unsigned short src)
{
		src=kj_unicode_to_cp932_c(src);
		src=kj_sjis_to_jis_c(src);
		return kj_jis_to_euc_c(src);
}

unsigned short kj_unicode_to_jis_c(unsigned short src)
{
		src=kj_unicode_to_cp932_c(src);
		return kj_sjis_to_jis_c(src);
}

unsigned short kj_unicode_to_sjis_c(unsigned short src)
{
		return kj_unicode_to_cp932_c(src);
}






int kj_cp932_to_unicode_n(unsigned char* src,unsigned short *dst, int sz)
{
	sz -= 2;			/* nullの分引いておく */
	if(src==NULL || dst==NULL || sz < 0)return 0;

	while(*src && sz>=2)
	{
		if((*src<0x80)||(*src>=0xa0 && *src<0xe0))
		{
			*dst=kj_cp932_to_unicode_c(*src);
			if(*dst) { dst++; sz-=2; }
		}
		else
		{
			unsigned short tmp;
			tmp=*src++;
			if(*src==0)break;
			tmp<<=8;
			tmp|=*src;
			*dst=kj_cp932_to_unicode_c(tmp);
			if(*dst) { dst++; sz-=2; }
		}
		src++;
	}
	*dst=0;
	return 1;
}


int kj_cp932_to_unicode(unsigned char* src,unsigned short *dst)
{
	if(src==NULL || dst==NULL)return 0;

	while(*src)
	{
		if((*src<0x80)||(*src>=0xa0 && *src<0xe0))
		{
			*dst=kj_cp932_to_unicode_c(*src);
			if(*dst)dst++;
		}
		else
		{
			unsigned short tmp;
			tmp=*src++;
			if(*src==0)break;
			tmp<<=8;
			tmp|=*src;
			*dst=kj_cp932_to_unicode_c(tmp);
			if(*dst)dst++;
		}
		src++;
	}
	*dst=0;
	return 1;
}


int kj_euc_to_unicode(unsigned char* src,unsigned short *dst)
{
	if(src==NULL || dst==NULL)return 0;

	while(*src)
	{
		if(*src<0x80)
		{
			*dst=kj_cp932_to_unicode_c(*src);
			if(*dst)dst++;
		}
		else
		{
			unsigned short tmp;
			tmp=*src++;
			if(*src==0)break;
			tmp<<=8;
			tmp|=*src;
			*dst=kj_euc_to_unicode_c(tmp);
			if(*dst)dst++;
		}
		src++;
	}
	*dst=0;
	return 1;
}

int kj_jis_to_unicode(unsigned char* src,unsigned short *dst)
{
	if(src==NULL || dst==NULL)return 0;

	while(*src)
	{
		unsigned short  tmp;
		
		tmp=*src++;
		if(*src==0)break;
		tmp<<=8;
		tmp|=*src;
		*dst=kj_jis_to_unicode_c(tmp);
		if(*dst)dst++;
		src++;
	}
	*dst=0;
	return 1;
}

int kj_jis_to_unicode_m(unsigned char* src,unsigned short *dst)
{
	int flag=0;

	if(src==NULL || dst==NULL)return 0;

	while(*src)
	{
		unsigned short  tmp;

		tmp=*src++;
		if(*src==0)break;

		if ( flag==0)
		{
			if((tmp==0x1b)&&(*src==0x24)&&(*(src+1)==0x42))
			{
				flag=1;
				src+=2;
			}
			else
			{
				*dst=kj_jis_to_unicode_c(tmp);
				if(*dst)dst++;
			}
		}
		else
		{
			if((tmp==0x1b)&&(*src==0x28)&&(*(src+1)==0x42))
			{
				flag=0;
				src+=2;
			}
			else
			{
				tmp<<=8;
				tmp|=*src;
				*dst=kj_jis_to_unicode_c(tmp);
				if(*dst)dst++;
				src++;
			}
		}
	}
	*dst=0;
	return 1;
}


int kj_sjis_to_unicode_n(unsigned char* src,unsigned short *dst,int sz)
{
	return kj_cp932_to_unicode_n(src,dst,sz);
}

int kj_sjis_to_unicode(unsigned char* src,unsigned short *dst)
{
	return kj_cp932_to_unicode(src,dst);
}



int kj_unicode_to_cp932_n(unsigned short* src,unsigned char *dst, int sz)
{
	unsigned short tmp;
	sz--;			/* nullの分引いておく */
	if(src==NULL || dst==NULL || sz < 0)return 0;

	while(*src)
	{
		tmp=kj_unicode_to_cp932_c(*src);
		if(tmp)
		{
			if(tmp<0x100)
			{
				if(sz < 1) {
					break;
				}
				*dst++=(unsigned char)tmp;
				sz--;
			}
			else 
			{
				if(sz < 2) {
					break;
				}
				*dst++=(tmp>>8)&255;
				*dst++=tmp&255;
				sz-=2;
			}
		}
		src++;
	}
	*dst=0;
	return 1;
}


int kj_unicode_to_cp932(unsigned short* src,unsigned char *dst)
{
	unsigned short tmp;
	if(src==NULL || dst==NULL)return 0;

	while(*src)
	{
		tmp=kj_unicode_to_cp932_c(*src);
		if(tmp)
		{
			if(tmp<0x100)
			{
				*dst++=(unsigned char)tmp;
			}
			else 
			{
				*dst++=(tmp>>8)&255;
				*dst++=tmp&255;
			}
		}
		src++;
	}
	*dst=0;
	return 1;
}


int kj_unicode_to_euc(unsigned short* src,unsigned char *dst)
{
	unsigned short tmp;
	if(src==NULL || dst==NULL)return 0;

	while(*src)
	{
		tmp=kj_unicode_to_euc_c(*src);
		if(tmp)
		{
			if(tmp<0x100)
			{
				*dst++=(unsigned char)tmp;
			}
			else 
			{
				*dst++=(tmp>>8)&255;
				*dst++=tmp&255;
			}
		}
		src++;
	}
	*dst=0;
	return 1;
}


int kj_unicode_to_jis(unsigned short* src,unsigned char *dst)
{
	unsigned short tmp;
	if(src==NULL || dst==NULL)return 0;

	while(*src)
	{
		tmp=kj_unicode_to_jis_c(*src);
		if(tmp)
		{
			if(tmp<0x100)
			{
				*dst++=(unsigned char)tmp;
			}
			else 
			{
				*dst++=(tmp>>8)&255;
				*dst++=tmp&255;
			}
		}
		src++;
	}
	*dst=0;
	return 1;
}

int kj_unicode_to_sjis_n(unsigned short* src,unsigned char *dst, int sz)
{
	return kj_unicode_to_cp932_n(src,dst,sz);
}

int kj_unicode_to_sjis(unsigned short* src,unsigned char *dst)
{
	return kj_unicode_to_cp932(src,dst);
}



int kj_unicode_to_utf8_c(unsigned short src)
{
	int r1,r2,r3;

	if(src<0x80)return src;
	if(src<0x800)
	{
		r1=(src>>6)|0xc0;
		r2=(src&0x3f)|0x80;
		return (r1<<8)|r2;

	}

	r1=(src>>12)|0xe0;
	r2=((src>>6)&0x3f)|0x80;
	r3=(src&0x3f)|0x80;

	return (r1<<16)|(r2<<8)|r3;
}

unsigned short kj_utf8_to_unicode_c(int src)
{
	int r1,r2,r3;
	if(src<0x100)return src & 0x7f;
	if(src<0x10000)
	{
		r1=(src>>8)&0x1f;
		r2=src&0x03f;
		return (r1<<6)|r2;
	}

	r1=(src>>16)&0x0f;
	r2=(src>>8)&0x03f;
	r3=(src)&0x03f;
	return (r1<<12)|(r2<<6)|r3;
}



int kj_unicode_to_utf8(unsigned short* src,unsigned char* dst)
{
	int ret;

	if(src==NULL || dst==NULL)return 0;
	while(*src)
	{
		ret=kj_unicode_to_utf8_c(*src);
		if(ret<0x100)
		{
			*dst++=(unsigned char)ret;
		}
		else if(ret<0x10000)
		{
			*dst++=(unsigned char)((ret>>8)&255);
			*dst++=(unsigned char)(ret&255);
		}
		else
		{
			*dst++=(unsigned char)((ret>>16)&255);
			*dst++=(unsigned char)((ret>>8)&255);
			*dst++=(unsigned char)(ret&255);
		}
		src++;
	}
	*dst=0;
	return 1;
}


int kj_utf8_to_unicode_n(unsigned char* src,unsigned short* dst, int sz)
{
	int ret;
	int result = 1;

	sz -= 2;			/* nullの分引いておく */
	if(src==NULL || dst==NULL || sz < 0)return 0;
	while(*src)
	{
		ret=0;
		if(*src<0x80)ret=*src;
		else if(*src<0xe0)
		{
			ret=*src++;
			if(*src==0)
			{
				result = 0;
				goto exit_function;
			}
			ret<<=8;
			ret|=*src;
		}
		else
		{
			ret=*src++;
			if(*src==0)
			{
				result = 0;
				goto exit_function;
			}
			ret<<=8;
			ret|=*src++;
			if(*src==0)
			{
				result = 0;
				goto exit_function;
			}
			ret<<=8;
			ret|=*src;
		}
		ret=kj_utf8_to_unicode_c(ret);
		if(ret) {
			if(sz<2) {
				goto exit_function;
			}
			*dst++=(unsigned short)ret;
			sz-=2;
		}
		src++;
	}
exit_function:
	*dst=0;
	return result;
}


int kj_utf8_to_unicode(unsigned char* src,unsigned short* dst)
{
	int ret;

	if(src==NULL || dst==NULL)return 0;
	while(*src)
	{
		ret=0;
		if(*src<0x80)ret=*src;
		else if(*src<0xe0)
		{
			ret=*src++;
			if(*src==0)
			{
				*dst=0;
				return 0;
			}
			ret<<=8;
			ret|=*src;
		}
		else
		{
			ret=*src++;
			if(*src==0)
			{
				*dst=0;
				return 0;
			}
			ret<<=8;
			ret|=*src++;
			if(*src==0)
			{
				*dst=0;
				return 0;
			}
			ret<<=8;
			ret|=*src;
		}
		ret=kj_utf8_to_unicode_c(ret);
		if(ret)*dst++=(unsigned short)ret;
		src++;
	}
	*dst=0;
	return 1;
}



int kj_unicode_to_utf8_n(unsigned short* src,unsigned char* dst,int n)
{
	int ret;

	if(src==NULL || dst==NULL || n<=0)return 0;

	while(*src)
	{
		ret=kj_unicode_to_utf8_c(*src);
		if(ret<0x100)
		{
			n--;
			if(n>0)
			{
				*dst++=(unsigned char)ret;
			}
			else
			{
				goto end;
			}
		}
		else if(ret<0x10000)
		{
			n-=2;
			if(n>0)
			{
				*dst++=(unsigned char)((ret>>8)&255);
				*dst++=(unsigned char)(ret&255);
			}
			else
			{
				goto end;
			}
		}
		else
		{
			n-=3;
			if(n>0)
			{
				*dst++=(unsigned char)((ret>>16)&255);
				*dst++=(unsigned char)((ret>>8)&255);
				*dst++=(unsigned char)(ret&255);
			}
			else
			{
				goto end;
			}
		}
		src++;
	}

end:
	*dst=0;
	return 1;
}


/*****************************************************************************/
/* function    : kj_utf8strncpyz()                                           */
/* description : UTF8文字列単位のstrncpy() ただし null terminateは保障される */
/* parameter   : unsigned char *dst ( o ) コピー先バッファ                   */
/*               unsigned char *src ( i ) コピー元文字列                     */
/*               int            sz  ( i )                                    */
/* result      : none                                                        */
/*****************************************************************************/
unsigned char *kj_utf8strncpyz(unsigned char* dst, unsigned char* src, int sz)
{
	int ret;

	sz -= 1;			/* nullの分引いておく */
	if(src==NULL || dst==NULL || sz < 0)return 0;
	while(*src)
	{
		ret=0;
		if(*src<0x80)
		{
			sz-=1;
			if((sz<0))
			{
				goto exit_function;
			}
			*dst++ = *src++;
		}
		else if(*src<0xe0)
		{
			sz-=2;
			if((sz<0) || (*(src + 1)==0))
			{
				goto exit_function;
			}
			*dst++ = *src++;
			*dst++ = *src++;
		}
		else
		{
			sz-=3;
			if((sz<0) || (*(src + 1)==0) || (*(src + 2)==0))
			{
				goto exit_function;
			}
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
		}
	}
exit_function:
	*dst=0;
	return dst;
}


/*
 * name:      IsSJISKanji()
 * synopsis:  unsigned short IsSJISKanji(unsigned char)
 *            unsigned short IsSJISKanji1st(unsigned char)
 *            unsigned short IsSJISKanji2nd(unsigned char)
 * input value:  none
 * output value: none
 *
 * description: Is this code KANJI
 *
 * change log:
 *
 * Dec 19, 1999  buggy
 *  1st edition
 */
int
IsSJISKanji1st(unsigned char c)
{
  if (0x81 <= c && c <= 0x9f) return 1;
  return 0;
}


int
IsSJISKanji2nd(unsigned char c)
{
  if (0xe0 <= c && c <= 0xfc) return 1;
  return 0;
}


int
IsSJISKanji(unsigned char c)
{
  if (0x81 <= c && c <= 0x9f) return 1;
  if (0xe0 <= c && c <= 0xfc) return 1;
  return 0;
}


/*
 * function    : wstrchr()
 * description : 全角が混ざった文字列から該当するcharのポインタを返す
 * parameter   : char * str   : search string pointer (null terminate)
 *               char   ccode : find character code
 * change log  : Aug 16, 2005  hmura
 *               1st edition
 */
char *wstrchr(char *str, char ccode)
{
    char *ptr = str;
    while(*ptr) {
      if(IsSJISKanji(*ptr)) {
        ptr++;
        if(*ptr == '\0') {
          break;
        }
      } else {
        if(*ptr == ccode) {
          return ptr;
        }
      }
      ptr++;
    }
    return NULL;
}


/*
 * name:      wstrncpy()
 * synopsis:  char *wstrncpy(unsigned char)
 * input value:  char *dst, char *src, size_t len
 * output value: char *
 *
 * description: Is this code KANJI
 *    The wstrncpy() copies not more than len characters into dst, appending
 *    `\0' characters if src is less than len characters long, and not termi-
 *    nating dst if src is more than len characters long. And if *(dst + len)'s
 *    character is the first SJIS kanji, its character is not copied and put
 *    null character there
 *
 * change log:
 *
 * Dec 19, 1999  buggy
 *  1st edition
 */
char *
wstrncpy(char *dst, char *src, size_t len)
{
  char  *d;
  size_t cnt;

  d = dst;
  if(len) {
    d = strncpy(dst, src, len);

    for(cnt = 0; cnt < len; cnt++)
    {
      if(IsSJISKanji((unsigned char)(*(dst + cnt)))) {
        cnt++;
        if(cnt >= len) {
          *(dst + len - 1) = '\0';
        }
      }
    }
  }
  return d;
}


/*
 * function    : wstrncpy2toupper1()
 * description : 可能な限り全角を半角にし、toupperを行い dstに出力する
 * parameter   : char * dst : dst string pointer (null terminate)
 *               char * src : src string pointer (must be null terminate)
 *               size_t len : dst buffer size
 * change log  : Jan 26, 2005  hmura
 *               1st edition
 */
char *
wstrncpy2toupper1(char *dst, char *src, size_t len)
{
#ifdef I_USE_SJIS2TOUPPER1
  unsigned char *d = (unsigned char *)dst;
  unsigned char *s = (unsigned char *)src;
  size_t    cnt;
  SJIS2TO1 *sjistbl;
  unsigned short us;

  if((dst == NULL) || (len < 1)) {
    return dst;
  }
  if(src == NULL) {
    *dst = 0;
    return dst;
  }

  cnt = len-1; /* decliment null terminate */
  while((*s) && cnt) {
    if(IsSJISKanji(*s)) {
      if(cnt < 2) {
        break;
      }

      us = (((unsigned short)(*s)) << 8) + *(s+1);
      if(us >= 0x8140 && us <= 0x839E) {
        sjistbl = &sjis2toupper1[us - 0x8140];
        *d++ = sjistbl->chr[0];
        *d   = sjistbl->chr[1];
        if(*d) {
          *d++;
        }
        s+=2;
      } else {
        *d++ = *s++;
        *d++ = *s++;
      }
      cnt-=2;
    } else {  /* harf character code */
      *d = *s;
      if(*d >= 'a' && *dst <= 'z') {
        (*d) -= 'a'-'A';
      }
      s++;
      d++;
      cnt--;
    }
  }
  *d = 0;
#endif /* I_USE_SJIS2TOUPPER1 */
  return dst;
}


#if 0

unsigned short* ptn[]={
	L"",
	L"1",
	L"12",
	L"a\x611b+",
	(unsigned short*)"a\x00\xb1\x00+\x00\x00",
	L"",
	NULL
};

unsigned char* ptn2[]={
//	"",
//	"1",
//	"12",
//	"\x61\xc2",
//	"\x61\xc2\xb1",
//	"\x61\xc2\xb1\x2b",
//	"\x61\xe6",
//	"\x61\xe6\x84",
	"\x61\xe6\x84\x9b",
	"\x61\xe6\x84\x9b\x2b",
	NULL
};

int main()
{
	int i,j;
	unsigned char buf[256];
	unsigned short buf2[256];
	



	for(i=0;ptn[i]!=NULL;i++)
	{
		printf("src=");
		for(j=0;ptn[i][j]!=0;j++)
		{
			printf("%04x ",ptn[i][j]);
		}
		printf("\n");
		kj_unicode_to_utf8(ptn[i],buf);
		printf("dst=");
		for(j=0;buf[j]!=0;j++)
		{
			printf("%02x ",buf[j]);
		}
		printf("\n\n");
	}

	printf("\n#######\n\n");

	for(i=0;ptn2[i]!=NULL;i++)
	{
		printf("src=");
		for(j=0;ptn2[i][j]!=0;j++)
		{
			printf("%02x ",ptn2[i][j]);
		}
		printf("\n");
		kj_utf8_to_unicode(ptn2[i],buf2);
		printf("dst=");
		for(j=0;buf2[j]!=0;j++)
		{
			printf("%04x ",buf2[j]);
		}
		printf("\n\n");
	}
}

#endif



#if 0
int main()
{
//	printf("%04x\n",kj_jis_to_sjis_c(0x2422));
//	printf("%04x\n",kj_jis_to_euc_c(0x2422));
	
//	printf("%04x\n",kj_sjis_to_jis_c(0x82a0));
//	printf("%04x\n",kj_euc_to_jis_c(0xa4a2));

//	printf("%04x\n",kj_jis_to_sjis_c(0x31));
//	printf("%04x\n",kj_jis_to_euc_c(0x31));
	
//	printf("%04x\n",kj_sjis_to_jis_c(0x31));
//	printf("%04x\n",kj_euc_to_jis_c(0x31));

//	printf("%06x\n",kj_unicode_to_utf8_c(0x7f));
//	printf("%06x\n",kj_unicode_to_utf8_c(0x80));

//	printf("%06x\n",kj_unicode_to_utf8_c(0x7ff));
//	printf("%06x\n",kj_unicode_to_utf8_c(0x800));
//	printf("%06x\n",kj_unicode_to_utf8_c(0x611b));

	printf("\n");
	printf("%04X\n",kj_utf8_to_unicode_c(0x7f));
	printf("%04X\n",kj_utf8_to_unicode_c(0x80));
	printf("%04X\n",kj_utf8_to_unicode_c(0xc280));
	printf("%04X\n",kj_utf8_to_unicode_c(0xffff));
	printf("%04X\n",kj_utf8_to_unicode_c(0xe0a080));
	printf("%04X\n",kj_utf8_to_unicode_c(0x00ffffff));
	printf("%04X\n",kj_utf8_to_unicode_c(0xe6849b));
	return 0;
}
#endif

