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

$Log: kanji.h,v $
Revision 1.3  2008/06/23 03:08:25  mizuba
no message

Revision 1.2  2008/04/14 02:12:05  akatsuka
no message

Revision 1.1  2007/02/09 04:06:27  tooyama
no message

Revision 1.1.1.1  2006/08/17 09:13:20  yokoyama
no message

Revision 1.1  2006/07/27 06:29:15  tooyama
initial

Revision 1.8  2006/03/08 07:30:43  hmura
add kj_utf8strncpyz()

Revision 1.7  2006/01/24 06:10:30  hmura
add kj_xxxx_n() functions

Revision 1.6  2005/08/16 07:30:09  hmura
add functino prototype

Revision 1.5  2005/01/27 05:38:12  hmura
add wstrncpy2toupper1()

Revision 1.4  2004/08/26 19:29:47  hmura
add wstrncpy()

Revision 1.3  2003/10/07 07:05:22  tooyama
*** empty log message ***

Revision 1.2  2003/10/06 02:05:27  yomei
*** empty log message ***

Revision 1.1  2003/10/05 09:37:16  yomei
Initial revision


*/

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

