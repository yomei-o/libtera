/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/
 
#ifndef _GETOPT_H
#define _GETOPT_H


#ifdef	__cplusplus
extern "C" {
#endif

extern char *myoptarg;
extern int myoptind;
extern int myopterr;
extern int myoptopt;

struct myoption
{
  const char *name;
  int has_arg;
  int *flag;
  int val;
};


#define	no_argument		0
#define required_argument	1
#define optional_argument	2

extern int mygetopt (int argc, char *const *argv, const char *shortopts);



#ifdef	__cplusplus
}
#endif

#endif /* _GETOPT_H */

