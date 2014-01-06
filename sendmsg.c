#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


#define PJ_INET6_ADDRSTRLEN	46
static int  UDPCreate();
static void UDPCreateThread();
static void* UDPRecvThread(void* arg);
static void* UDPSendThread(void* arg);


static struct app_t
{
    int rport;
    char ripaddr[PJ_INET6_ADDRSTRLEN];
    int lport;
    char lipaddr[PJ_INET6_ADDRSTRLEN]; 
    int localfd;
} icedemo;


static int  UDPCreate(){
   int sfd;
   struct sockaddr_in addr_server,addr_local;
   int len=sizeof(struct sockaddr);
   int sent,on=1;
  

   //icedemo_stop_session(); 
   sfd=socket(AF_INET,SOCK_DGRAM,0);
   if(sfd==-1){
       perror("SOCKET");
       return ;
   }
   setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR | SO_BROADCAST,&on,sizeof(on)); 
   memset(&addr_server,0,len);
   addr_server.sin_family=AF_INET;
   addr_server.sin_addr.s_addr=inet_addr(icedemo.ripaddr); 
   addr_server.sin_port=htons(icedemo.rport);

   memset(&addr_local,0,len);
   addr_local.sin_family=AF_INET;
   addr_local.sin_addr.s_addr=inet_addr(icedemo.lipaddr); 
   addr_local.sin_port=htons(icedemo.lport);
  
   if(bind(sfd,(struct sockaddr*)&addr_local,len)==-1){
    perror("BIND");
    return ;
    }  
    if(connect(sfd,(struct sockaddr*)&addr_server,len)==-1){
     perror("CONNECT");
     return ;
   }
   printf("OUR OWN UDP SOCKET CREATED:%d\n",sfd);
    return sfd;
}
static void UDPCreateThread(){
  int rt;
  pthread_t idsend,idrecv;
  rt=pthread_create(&idsend,NULL,UDPSendThread,NULL);
  if(rt!=0){
    perror("PTHREAD_CREATE UDPSendThread");
 }
  rt=pthread_create(&idrecv,NULL,UDPRecvThread,NULL);
  if(rt!=0){
    perror("PTHREAD_CREATE UDPRecvThread");
 } 

  pthread_join(idsend,NULL);
  pthread_join(idrecv,NULL);
}

static void* UDPRecvThread(void* arg){
  printf("UDPRecvThread CREATED\n");
  char buffer[1024];
  int n;
  for(;;){
    n=read(icedemo.localfd,buffer,sizeof(buffer));
    printf("read %d bytes,data:%s\n",n,buffer); 
  
  }
}

static void* UDPSendThread(void* arg){
   int n;
   char buffer[1024];
   printf("UDPSendThread CREATED\n");
   printf("sleep 10\n");
   sleep(10);
   for(;;){
   gets(&buffer); 
   n=strlen(buffer);
   n=write(icedemo.localfd,buffer,n);
   printf("%d byte to send,%d bytes sent\n",strlen(buffer),n);
  }
  

}


int main(int argc,char* argv[]){
    sprintf(icedemo.lipaddr,"%s",argv[1]);
    icedemo.lport=atoi(argv[2]);
    sprintf(icedemo.ripaddr,"%s",argv[3]);
    icedemo.rport=atoi(argv[4]);
 
    icedemo.localfd=UDPCreate();
    /* a thread to read and a thread to write*/
     UDPCreateThread();
   return 0;
}
