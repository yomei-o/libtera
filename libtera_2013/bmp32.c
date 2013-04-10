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
#include <stdlib.h>
#include <string.h>
#include "bmp32.h"

/*
 * This function is save a RGB8888 formated bitmap data to a standard BMP file.  
 */


int savebmp32(char* fname,int x,int y,char* pdata)
{
	FILE* fp;
	char data[64];
	int sz;


	if(fname==NULL || data==NULL || x<=0 || y<=0)return 0;

	memset(data,0,sizeof(data));

	//14 bytes of header
	data[0]='B';
	data[1]='M';

	sz=x*y*4+54;

	data[2]=sz&255;
	data[3]=(sz>>8)&255;
	data[4]=(sz>>16)&255;
	data[5]=(sz>>24)&255;

	data[10]=54;

	//40 bytes of header
	data[14]=40;

	sz=x;
	data[18]=sz&255;
	data[19]=(sz>>8)&255;
	data[20]=(sz>>16)&255;
	data[21]=(sz>>24)&255;

	sz=-y;
	data[22]=sz&255;
	data[23]=(sz>>8)&255;
	data[24]=(sz>>16)&255;
	data[25]=(sz>>24)&255;

	data[26]=1;
	data[28]=32;

	data[30]=0;
	data[34]=0;
	data[38]=0;
	data[42]=0;

	data[46]=0;
	data[50]=0;
	
	fp=fopen(fname,"wb");
	if(fp==NULL)return 0;


	sz=x*y*4;
	fwrite(data,1,54,fp);
	fwrite(pdata,1,sz,fp);
	fclose(fp);

	return 1;
}



#if 0

main()
{
	unsigned char *data,*p;
	int x=300;
	int y=255;
	int i,j;


	data=malloc(x*y*4);
	if(data==NULL)return 1;


	for(i=0;i<y;i++)
	{
		p=data+4*i*x;
		for(j=0;j<x;j++)
		{
			*p++=i;
			*p++=0;
			*p++=0;
			*p++=0;
		}
	}
	savebmp32("test.bmp",x,y,data);

	free(data);

}

#endif
