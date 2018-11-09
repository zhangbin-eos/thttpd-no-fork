


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

//#define pr_log(lvl,fmt,args...)      syslog(lvl|LOG_USER,"func=[%s]"fmt,__FUNCTION__,##args)
#define pr_log(lvl,fmt,args...)      printf("func=[%s]_lv=%d"fmt,__FUNCTION__,lvl,##args)

extern int errno;

int main(int argc,char **argv)
{
        struct sockaddr_un selfaddr;
        int svfd;
        
        char *selfname=CGIDAEMON_LINK_NAME;
        char buff[512];
        int res=0;
        if ( strlen(selfname)>=(sizeof(selfaddr.sun_path)-1) )
        {
                pr_log(LOG_ERR,"selfname TOO LONG!!");
                return -1;
        }
        //设置自己的地址
        memset((void*)&selfaddr,0,sizeof(selfaddr));
        selfaddr.sun_family = AF_UNIX;
        strncpy(&(selfaddr.sun_path[0]), selfname,(sizeof(selfaddr.sun_path)-1));

        svfd = socket(AF_UNIX,SOCK_STREAM, 0);    // liu式套接字
        
        if( remove(selfname)==-1 && errno != ENOENT)
        {
                pr_log(LOG_ERR,"remove-%s error:%s",selfname,strerror(errno));
                return -1;
        }
        
        if( bind(svfd, (struct sockaddr *)&selfaddr, sizeof(struct sockaddr_un ))==-1 )
        {
                pr_log(LOG_ERR,"bind %s error:%s",selfname,strerror(errno));
                return -1;
        }        

        if( listen(svfd,32)== -1 )
        {
                pr_log(LOG_ERR,"listen %s error:%s",selfname,strerror(errno));
                return -1;
        }
	pr_log(LOG_NOTICE,"listening ...\n");
        while(1)
        {
                int client = accept(svfd,NULL,NULL);
                if(client==-1)
                {
                        pr_log(LOG_ERR,"%s accept error:%s",selfname,strerror(errno));
                }
		pr_log(LOG_NOTICE,"get a connet\nread:\n");
                /*可以使用线程实现*/
                memset(buff,0,sizeof(buff));
                while(1)
                {
	                res = cgi_connect_read(client,NULL,buff,sizeof(buff));
	                if(res<0)
	                {
				pr_log(LOG_ERR,"cgi_connect_read error:%s",strerror(errno));
				break;
	                }
	                else
	                {
	                        printf("%s",buff);
	                        memset(buff,0,sizeof(buff));
	                }
                }
                close(client);
        }
       
        close(svfd);
        return 0;

}

