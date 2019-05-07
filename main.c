#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<ctype.h>
#include<strings.h>
#include<string.h>
#include<sys/stat.h>
#include<pthread.h>
#include<sys/wait.h>
#include<stdlib.h>

int startup(int*port);
int error_die(const char*sc);
void*accept_request(void*from_client);
int get_line(int sock,char*buf,int size);

//读一行http报文
int get_line(int sock,char*buf,int size)
{
    int i=0;
    char c='\0';
    int n;
    while((i<size-1)&&(c!='\n'))
    {
	n=read(sock,&n,1);
	if(n>0)
	{
	    if(c=='\r')
	    {
		n=read(sock,&c,1);
		if((n>0)&&(c=='\n'))
		    read(sock,&c,1);
		else
		    c='\n';
	    }
	    buf[i]=c;
	    i++;
	}
	else
	    c='\n' 
    }
    buf[i]='\0';
    return i;
}

//接收客户端的连接
void*accept_request(void*from_client)
{
    int client=*(int*)from_client;
    char buf[1024];
    int numchars;
    char method[255];
    char url[255];
    char path[512];
    size_t i,j;
    //struct stat
}

void *accept_request_test(void*from_client)
{
    int client=*(int*)from_client;
    char buf[10240];
    int len=read(client,buf,sizeof(buf)-1);
    printf("len:%d\n",len);
    printf("%s",buf); 
    close(client);
}

//打印错误信息，并退出
int error_die(const char*sc)
{
    perror(sc);
    exit(1);
}

//启动服务端
int startup(int*port)
{
    
    int httpd=0;
    struct sockaddr_in name;
    httpd=socket(PF_INET,SOCK_STREAM,0);
    if(httpd==-1)
	error_die("socket error");
    memset(&name,0,sizeof(name));
    name.sin_family=AF_INET;
    name.sin_port=htons(*port);
    name.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(httpd,(struct sockaddr*)&name,sizeof(name))<0)
	error_die("bind");

    //返回动态分配的端口号
    if(*port==0)
    {
	socklen_t namelen=sizeof(name);
	if(getsockname(httpd,(struct sockaddr*)&name,&namelen)==-1)
	    error_die("getsockname");
	*port=ntohs(name.sin_port);
    }

    if(listen(httpd,5)<0)
	error_die("listen");
    return httpd;
}

int main()
{
    int server_sock=-1;
    int port=4000;
    int client_sock=-1;
    struct sockaddr_in client_name;
    socklen_t client_name_len=sizeof(client_name);
    pthread_t newthread;
    server_sock=startup(&port);
    printf("httpd running on port %d\n",port);
    while(1)
    {
	client_sock=accept(server_sock,(struct sockaddr*)&client_name,&client_name_len);
	if(client_sock==-1)
	    error_die("accpet");
	if(pthread_create(&newthread,NULL,accept_request_test,(void*)&client_sock)!=0)
	    perror("pthread_create");	
    }
    return 0;
}
