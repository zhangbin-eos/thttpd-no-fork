/******************************************************************************
 LIG HEAD FILE
 ******************************************************************************
  File Name     : cgi_connect.h
  Version       : Initial Draft
  Author        : zhangbin
  Created       : 9/11/2018
  Last Modified :
  Description   :
  Function List :

  History       :
  1.Date        : 9/11/2018
    Author      : zhangbin
    Modification: Created file

**************************************************************************/
#ifndef __CGI_CONNECT_H__
#define __CGI_CONNECT_H__
#ifdef __cplusplus
extern "C" {
#endif

/*=============include================================*/
#include <stddef.h>
/*=============EXTERN DEF=============================*/

#define THTTPD_LINK_NAME "/tmp/THTTPD_LINK_NAME"
#define CGIDAEMON_LINK_NAME "/tmp/CGIDAEMON_LINK_NAME"

/*=============EXTERN ENUM============================*/

/*=============EXTERN STRUCT==========================*/

/*=============EXTERN VALUE===========================*/

/*=============EXTERN FUNC============================*/


/*******************************
-Function Name :cgi_connect_thttpd_open
-Description   :
-Call	       :
-Called By     :
-Input	Param  :
-Return Value  :
-Author        : zhangbin.eos@foxmail.com 2018/9/7
*********************************/
int cgi_connect_thttpd_open(char * objname,void * arg);


	/* Copy from libhttp.c 
	int httpd_read_fully(int fd, void *buf, size_t nbytes)
	int httpd_write_fully(int fd, void *buf, size_t nbytes)
	*/

/* Read the requested buffer completely, accounting for interruptions. */
int cgi_connect_read(int fd, void*arg, void *buf, size_t nbytes);


/* Write the requested buffer completely, accounting for interruptions. */
int cgi_connect_write(int fd,void*arg, const char *buf, size_t nbytes);



#ifdef __cplusplus
}
#endif

#endif //__CGI_CONNECT_H__

