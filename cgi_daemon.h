/******************************************************************************
 LIG HEAD FILE
 ******************************************************************************
  File Name     : cgi_daemon.h
  Version       : Initial Draft
  Author        : zhangbin
  Created       : 6/11/2018
  Last Modified :
  Description   :
  Function List :

  History       :
  1.Date        : 6/11/2018
    Author      : zhangbin
    Modification: Created file

**************************************************************************/
#ifndef __CGI_DAEMON_H__
#define __CGI_DAEMON_H__
#ifdef __cplusplus
extern "C" {
#endif

/*=============include================================*/
#include <stddef.h>
/*=============EXTERN DEF=============================*/
typedef  void * cgi_daemon_handle_t;
/*=============EXTERN ENUM============================*/

/*=============EXTERN STRUCT==========================*/

/*=============EXTERN VALUE===========================*/

/*=============EXTERN FUNC============================*/


/*******************************
-Function Name : cgi_daemon_open
-Description   : 打开cgi_daemon链接的句柄
-Call          : 
-Called By     : 
-Input  Param  : 传入一个特征字符,用于标识链接
-Return Value  : 
-Author        : zhangbin.eos@foxmail.com 6/11/2018
*********************************/
cgi_daemon_handle_t cgi_daemon_open(const char * name);

/*******************************
-Function Name : cgi_daemon_close
-Description   : 关闭cgi_daemon链接的句柄
-Call          : 
-Called By     : 
-Input  Param  : 
-Return Value  : 
-Author        : zhangbin.eos@foxmail.com 6/11/2018
*********************************/
int cgi_daemon_close(cgi_daemon_handle_t  handle);


int cgi_daemon_write(cgi_daemon_handle_t  handle,const char * data,size_t len );


int cgi_daemon_read(cgi_daemon_handle_t  handle, char * const buff,size_t len );

#ifdef __cplusplus
}
#endif

#endif //__CGI_DAEMON_H__

