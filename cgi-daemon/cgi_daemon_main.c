/******************************************************************************
  Copyright (C) 2018 Liguo Ltd. All rights reserved.

  LIG SOURCE FILE
 ******************************************************************************
  File Name     : cgi_daemon_main.c
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
//#define pr_log(lvl,fmt,args...)      printf("func=[%s]_lv=%d"fmt,__FUNCTION__,lvl,##args)

#define CGI_REQUEST_MEMORY_LEN    512

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#define NEW(t,n) ((t*) malloc( sizeof(t) * (n) ))
#define RENEW(o,t,n) ((t*) realloc( (void*) o, sizeof(t) * (n) ))

/*=============STATIC ENUM============================*/

/*=============STATIC STRUCT==========================*/
struct cgi_request
{
	int flag;		//是否时动态标记
	char *memstart;		//内存地址
	size_t memlen;		//内存长度
	size_t datalen;		//内存中的有效长度

	char *env[50];
	size_t envcnt;

	char *arg[50];
	size_t argcnt;

	size_t each_rcv;	//每条数据的接收计数,会在每条数据开始计数时清空
	size_t end_stat;	//;;\r\n 对应4==b100
};

typedef struct cgi_request cgi_request_t;

typedef enum
{ false, true } bool;

/*=============STATIC FUNC_DEF========================*/

bool cgi_request_is_once_end(cgi_request_t * obj, char data)
{
	if ((obj->end_stat == 0 && data == ';') ||
	    (obj->end_stat == 1 && data == ';') ||
	    (obj->end_stat == 2 && data == '\r') ||
	    (obj->end_stat == 3 && data == '\n'))
		obj->end_stat += 1;
	else
	{
		obj->end_stat = 0;
		return false;
	}

	if (obj->end_stat == 4)
		return true;
	else
		return false;
}

void cgi_request_clean(cgi_request_t * obj)
{

	obj->datalen = 0;
	obj->envcnt = 0;
	obj->argcnt = 0;
	obj->each_rcv = 0;
	obj->end_stat = 0;

	memset((void *)obj->env, 0, sizeof(obj->env));
	memset((void *)obj->arg, 0, sizeof(obj->arg));
	return;
}

cgi_request_t *cgi_request_init(cgi_request_t * obj)
{
	if (obj == NULL)
	{
		obj = NEW(cgi_request_t, 1);
		obj->flag = 1;	//动态的
	}
	else
	{
		obj->flag = 0;	//静态的
	}

	obj->memstart = NEW(char, CGI_REQUEST_MEMORY_LEN);
	obj->memlen = CGI_REQUEST_MEMORY_LEN;
	cgi_request_clean(obj);
	return obj;
}

void cgi_request_destory(cgi_request_t * obj)
{
	if (obj == NULL)
	{
		return;
	}

	/*释放资源 */
	free(obj->memstart);

	if (obj->flag == 0)
	{
		return;
	}
	else
	{
		free(obj);
	}

}

char *cgi_request_unpack(cgi_request_t * obj, char data)
{
	obj->memstart[obj->datalen++] = data;

	obj->each_rcv++;

	if (obj->datalen > obj->memlen)
	{
		obj->memstart = RENEW(obj->memstart, char, obj->memlen + 128);
		obj->memlen += 128;
	}

	if (cgi_request_is_once_end(obj, data))
	{
		//end
		obj->datalen -= 4;
		obj->each_rcv -= 4;
		obj->memstart[obj->datalen] = '\0';
		//是否是env
		char *tmp = &(obj->memstart[obj->datalen - obj->each_rcv]);
		pr_log(LOG_NOTICE, "get [%s];;", tmp);
		if (strstr(tmp, "="))
			obj->env[obj->envcnt++] = tmp;
		else
			obj->arg[obj->argcnt++] = tmp;

		if (obj->envcnt == 49)
			obj->env[obj->envcnt] = NULL;
		if (obj->argcnt == 49)
			obj->arg[obj->argcnt] = NULL;

		obj->each_rcv = 0;

		obj->datalen++;	//指向下一个字节
		return tmp;
	}
	return NULL;
}

/*=============STATIC VALUE===========================*/

//=============STATIC INLINLE=========================*/

/*=============EXTERN VALUE===========================*/
extern int errno;

/*=============EXTERN FUNC============================*/

int cgi_daemon_process(int fd)
{
	cgi_request_t *cgi_rqt_info;
	char *tmp;
	char buff[512];
	int i = 0, res;
	/*可以使用线程实现 */
	cgi_rqt_info = cgi_request_init(NULL);

	memset(buff, 0, sizeof(buff));
	while (1)
	{
		res = cgi_connect_read(fd, NULL, buff, sizeof(buff));
		if (res < 0)
		{
			pr_log(LOG_ERR, "cgi_connect_read error:%s",
			       strerror(errno));
			break;
		}
		else
		{
			for (i = 0; i < res; i++)
			{
				tmp = cgi_request_unpack(cgi_rqt_info, buff[i]);
				if (tmp != NULL)
				{
					printf
					    ("cgi_request_unpack get=[%s] \n",
					     tmp);
					printf("envcnt=%d  argcnt=%d\n",
					       cgi_rqt_info->envcnt,
					       cgi_rqt_info->argcnt);
					if (cgi_rqt_info->argcnt == 1)
					{
						cgi_request_clean(cgi_rqt_info);
						sprintf(buff, "%s","Content-type: application/json; charset=utf-8\r\n");
						cgi_connect_write(fd, NULL, buff, strlen(buff));
						sprintf(buff, "%s", "Cache-Control: no-cache\r\n");
						cgi_connect_write(fd, NULL, buff, strlen(buff));
						sprintf(buff, "%s", "Pragma: no-cache\r\n");
						cgi_connect_write(fd, NULL, buff, strlen(buff));
						sprintf(buff, "%s", "Expires: 0\r\n\r\n");
						cgi_connect_write(fd, NULL, buff, strlen(buff));
						sprintf(buff, "%s", "{\"status\":\"success\"}\n\n;;\r\n");
						cgi_connect_write(fd, NULL, buff, strlen(buff));
						break;
					}
				}
			}
		}
	}
	cgi_request_destory(cgi_rqt_info);
	return 0;
}

int main(int argc, char **argv)
{
	struct sockaddr_un selfaddr;
	int svfd, client;

	svfd = cgi_connect_cgi_daemon_open(CGIDAEMON_LINK_NAME, NULL);

	while (1)
	{
		client = cgi_connect_cgi_daemon_accept(svfd, NULL);
		if (client == -1)
		{
			pr_log(LOG_ERR, "%s accept error:%s",
			       CGIDAEMON_LINK_NAME, strerror(errno));
		}
		pr_log(LOG_NOTICE, "get a connet\nread:\n");
		/*新建一个解释器 */
		cgi_daemon_process(client);
		close(client);
	}

	close(svfd);
	return 0;
}

/*=============STATIC FUNC----===================== ==*/

//end cgi_daemon_main.c
