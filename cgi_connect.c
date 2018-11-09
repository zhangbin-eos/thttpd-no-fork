/******************************************************************************
  Copyright (C) 2018 Liguo Ltd. All rights reserved.

  LIG SOURCE FILE
 ******************************************************************************
  File Name     : cgi_connect.c
  Version       : Initial Draft
  Author        : ZhangBin
  Created       : 9/11/2018
  Last Modified :
  Description   :
  Function List :

  History       :
  1.Date        : 9/11/2018
    Author      : ZhangBin
    Modification: Created file

**************************************************************************/

/*=============include================================*/
#include "cgi_connect.h"
#include <sys/socket.h> 
#include <sys/un.h>     
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/*=============STATIC DEF=============================*/

#define pr_log(lvl,fmt,args...)      syslog(lvl|LOG_USER,"func=[%s]"fmt,__FUNCTION__,##args)

#define NEW(t,n) ((t*) malloc( sizeof(t) * (n) ))



/*=============STATIC ENUM============================*/


/*=============STATIC STRUCT==========================*/


/*=============STATIC FUNC_DEF========================*/


/*=============STATIC VALUE===========================*/


//=============STATIC INLINLE=========================*/


/*=============EXTERN VALUE===========================*/
extern int errno;


/*=============EXTERN FUNC============================*/

int cgi_connect_thttpd_open(char * objname,void * arg)
{
	struct sockaddr_un objaddr;
	int fd=0,res=0;

	fd = socket(AF_UNIX,SOCK_STREAM, 0);    // liu式套接字
	if(fd==-1)
	{
	        pr_log(LOG_ERR,"socket error,%s",strerror(errno));
	        return -1;
	}
	memset((void*)&objaddr,0,sizeof(objaddr));
	objaddr.sun_family = AF_UNIX;
	strncpy(&(objaddr.sun_path[0]), objname,(sizeof(objaddr.sun_path)-1));

	if(connect(fd,(struct sockaddr *)&objaddr,sizeof(struct sockaddr_un))<0)
	{
	        pr_log(LOG_ERR,"connect -->%s error,%s",objaddr.sun_path,strerror(errno));
	        return -1;
	}
	return fd;
}


int cgi_connect_read(int fd, void*arg, void *buf, size_t nbytes)
{

	int r;

	r = read(fd, (char *)buf , nbytes);
	if (r < 0 && (errno == EINTR || errno == EAGAIN))
	{
		return 0;
	}
	return r; 
}

int cgi_connect_write(int fd,void*arg, const char *buf, size_t nbytes)
{
	int r;
	r = write(fd, buf, nbytes);
	if (r < 0 && (errno == EINTR || errno == EAGAIN))
	{
		return 0;
	}
	return r; 
}

int cgi_connect_read_fully(int fd, void*arg, void *buf, size_t nbytes)
{
	int nread;

	nread = 0;
	while (nread < nbytes)
	{
		int r;

		r = read(fd, (char *)buf + nread, nbytes - nread);
		if (r < 0 && (errno == EINTR || errno == EAGAIN))
		{
			sleep(1);
			continue;
		}
		if (r < 0)
			return r;
		if (r == 0)
			break;
		nread += r;
	}
	return nread;
}

int cgi_connect_write_fully(int fd,void*arg, const char *buf, size_t nbytes)
{
	int nwritten;

	nwritten = 0;
	while (nwritten < nbytes)
	{
		int r;

		r = write(fd, buf + nwritten, nbytes - nwritten);
		if (r < 0 && (errno == EINTR || errno == EAGAIN))
		{
			sleep(1);
			continue;
		}
		if (r < 0)
			return r;
		if (r == 0)
			break;
		nwritten += r;
	}

	return nwritten;
}

/*=============STATIC FUNC----===================== ==*/



//end cgi_connect.c

