/******* BEGIN LICENCE BLOCK *******

    Sony Nasne Application
    Copyright (C) 2012 Sony Corporation.
    Yomei.Otani@jp.sony.com

******* END LICENCE BLOCK *******/

#ifndef df_MYSIGNAL_H_
#define df_MYSIGNAL_H_


#ifdef __cplusplus
extern "C"
{
#endif


typedef void (*mysighandler_t)(int);


void mysignal_init(void);
void mysignal_fini(void);

mysighandler_t mysignal(int signum, mysighandler_t sighandler);  
int myraise(int sig);

int mysignal_is_abort(void);


#ifdef __cplusplus
}
#endif


#endif /* _FDUP_H_ */


