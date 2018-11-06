/******************************************************************************
  Copyright (C) 2018 Liguo Ltd. All rights reserved.

  LIG SOURCE FILE
 ******************************************************************************
  File Name     : cgi_daemon.c
  Version       : Initial Draft
  Author        : ZhangBin
  Created       : 6/11/2018
  Last Modified :
  Description   :
  Function List :

  History       :
  1.Date        : 6/11/2018
    Author      : ZhangBin
    Modification: Created file

**************************************************************************/

/*=============include================================*/
#include "cgi_daemon.h"
#include <sys/socket.h> 
#include <sys/un.h>     

/*=============STATIC DEF=============================*/
#define CGI_DAEMON_LINK_NAME_MAX UNIX_PATH_MAX


/*=============STATIC ENUM============================*/


/*=============STATIC STRUCT==========================*/

struct cgi_daemon
{
	int	fd;
	int	fflag;	//文件的io属性标记
	char	link_name[CGI_DAEMON_LINK_NAME_MAX];
};

typedef  struct cgi_daemon cgi_daemon_t ;

/*=============STATIC FUNC_DEF========================*/
#define pr_log(lvl,fmt,args...)      syslog(lvl|LOG_USER,"func=[%s]"fmt,__FUNCTION__,##args)


/*=============STATIC VALUE===========================*/


//=============STATIC INLINLE=========================*/


/*=============EXTERN VALUE===========================*/


/*=============EXTERN FUNC============================*/
cgi_daemon_handle_t cgi_daemon_open(char * name )
{
        struct sockaddr_un saddr;
        int fd;
        cgi_daemon_t * handle;
        
        if ( NULL==name )
        {
                pr_log(LOG_ERR,"name NULL!!");
                return NULL;
        }
        if ( strlen(name)>=(sizeof(saddr.sun_path)-1) )
        {
                pr_log(LOG_ERR,"name TOO LONG!!");
                return NULL;
        }
        
        saddr.sun_family = AF_UNIX;
        sun_path[0]=0;
        strcpy(&(saddr.sun_path[1]), name);
        
        fd = socket(AF_UNIX,SOCK_DGRAM, 0);    // 数据包式套接字
                
        if( bind(fd, (struct sockaddr *)&saddr, sizeof(saddr))==-1 )
        {
                pr_log(LOG_ERR,"bind -->%s error !!",name);
                return NULL;
        }
        handle = NEW(cgi_daemon_t,1);
        
        handle->fd=fd;
        
        snprintf(handle->link_name,sizeof(handle->link_name),"%s",name);
        
        pr_log(LOG_NOTICE,"link_name=[%s]",handle->link_name);
        /*如果是server,需要listen*/
        return handle;
}

int cgi_daemon_getcnct(cgi_daemon_handle_t handle  )
{
	/*accept*/
	
	return 0;
}

int cgi_daemon_close(cgi_daemon_handle_t handle  )
{
        
        close(((cgi_daemon_t *)handle)->fd);
        remove(((cgi_daemon_t *)handle)->link_name);
        free(handle);
        return 0;
}

int cgi_daemon_set_ctrl(cgi_daemon_handle_t handle,int flag)
{
	
        return 0;
}
int cgi_daemon_get_ctrl(cgi_daemon_handle_t handle,int * flag)
{
	
        return 0;
}

int cgi_daemon_write(cgi_daemon_handle_t handle,const char * data,size_t len )
{
        struct sockaddr_un saddr;
        
        saddr.sun_family = AF_UNIX;
        
        return sendto ( ((cgi_daemon_t *)handle)->fd, 
                        (void*)data,
                        len,
                        ((cgi_daemon_t *)handle)->fflag,
                        (struct sockaddr *) &saddr,
                        (socklen_t)sizeof(saddr));  
}

int cgi_daemon_read(cgi_daemon_handle_t handle, char * const buff,size_t len )
{
        struct sockaddr_un saddr;
        int res;
        socklen_t saddr_len;
        if ( NULL==handle )
        {
                pr_log(LOG_ERR,"handle NULL!!");
                return -1;
        }
        saddr_len =  sizeof(saddr);
        
        return = recvfrom( ((cgi_daemon_t *)handle)->fd, 
                        buff, 
                        len, 
			((cgi_daemon_t *)handle)->fflag,
                        (struct sockaddr *) &saddr,
                        &saddr_len);
}


/*=============STATIC FUNC----===================== ==*/



//end cgi_daemon.c

