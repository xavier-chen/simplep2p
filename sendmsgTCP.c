#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


#define PJ_INET6_ADDRSTRLEN	46
static void  TCPClient();
static void TCPServer();

static struct app_t
{
    int rport;
    char ripaddr[PJ_INET6_ADDRSTRLEN];
    int lport;
    char lipaddr[PJ_INET6_ADDRSTRLEN]; 
    int localfd;
} icedemo;


static void  TCPClient(){
   int sfd;
   struct sockaddr_in addr_server,addr_local;
   int len=sizeof(struct sockaddr);
   char buffer[1024];
   int n,sent,on=1;
  

   sfd=socket(AF_INET,SOCK_STREAM,0);
   if(sfd==-1){
       perror("SOCKET");
       return ;
   }
   setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR ,&on,sizeof(on)); 
   memset(&addr_server,0,len);
   addr_server.sin_family=AF_INET;
   addr_server.sin_addr.s_addr=inet_addr(icedemo.ripaddr); 
   addr_server.sin_port=htons(icedemo.rport);

   memset(&addr_local,0,len);
   addr_local.sin_family=AF_INET;
   addr_local.sin_addr.s_addr=inet_addr(icedemo.lipaddr); 
   addr_local.sin_port=htons(icedemo.lport);
   //use this addr to connect to server 
   if(bind(sfd,(struct sockaddr*)&addr_local,len)==-1){
    perror("BIND");
    return ;
    }  
    if(connect(sfd,(struct sockaddr*)&addr_server,len)==-1){
     perror("CONNECT");
     return ;
   }
   printf("OUR OWN UDP SOCKET CREATED:%d\n",sfd);

   for(;;){
   gets(&buffer); 
   n=strlen(buffer);
   n=write(sfd,buffer,n);
   printf("%d byte to send,%d bytes sent\n",strlen(buffer),n);
  }


}


static void  TCPServer(){
   int sfd,connfd;
   struct sockaddr_in addr_local,addr_peer;  // local addr to bind listen
   int len=sizeof(struct sockaddr);
   char peeraddr[1024];
   char data[1024];
   int n,sent,on=1;
  

   sfd=socket(AF_INET,SOCK_STREAM,0);
   if(sfd==-1){
       perror("SOCKET");
       return ;
   }
   setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR ,&on,sizeof(on)); 

   memset(&addr_local,0,len);
   addr_local.sin_family=AF_INET;
   addr_local.sin_addr.s_addr=inet_addr(icedemo.lipaddr); 
   addr_local.sin_port=htons(icedemo.lport);

   if(bind(sfd,(struct sockaddr*)&addr_local,len)==-1){
    perror("BIND");
    return ;
    }  
    if(listen(sfd,10)==-1){
     perror("listen");
     return ;
   }
   for(;;){ 
    printf("Listening for connect:\n");
    len=sizeof(struct sockaddr_in);
    connfd=accept(sfd,(struct sockaddr*)&addr_peer,&len);
    if(connfd==-1){
        perror("ACCEPT");
        continue;
    }
    printf("remote peer:%s\n",inet_ntoa(addr_peer.sin_addr));
    while(1){
     n=read(connfd,data,sizeof(data));
     data[n]='\0';
     printf("read %d bytes,content:%s\n",n,data); 
   }
   
   }
}



int main(int argc,char* argv[]){
    int servering=0;
    if( argc<6){
      printf("USERAGE: local_addr local_port remote_addr remote_port servering(0|1)\n");
      return 1;
    }
    sprintf(icedemo.lipaddr,"%s",argv[1]);
    icedemo.lport=atoi(argv[2]);
    sprintf(icedemo.ripaddr,"%s",argv[3]);
    icedemo.rport=atoi(argv[4]);
    servering=atoi(argv[5]); 
    if (servering){
       TCPServer();
    }
    else{
     TCPClient();
    }
   return 0;
}
