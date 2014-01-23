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

   struct sockaddr_in addr_server,addr_local;
   int len;
} icedemo;


static int  UDPCreate(){
   int sfd;
   int sent,on=1;
  
   icedemo.len=sizeof(struct sockaddr);
   //icedemo_stop_session(); 
   sfd=socket(AF_INET,SOCK_DGRAM,0);
   if(sfd==-1){
       perror("SOCKET");
       return ;
   }
   setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR | SO_BROADCAST,&on,sizeof(on)); 
   memset(&icedemo.addr_server,0,icedemo.len);
   icedemo.addr_server.sin_family=AF_INET;
   icedemo.addr_server.sin_addr.s_addr=inet_addr(icedemo.ripaddr); 
   icedemo.addr_server.sin_port=htons(icedemo.rport);

   memset(&icedemo.addr_local,0,icedemo.len);
   icedemo.addr_local.sin_family=AF_INET;
   icedemo.addr_local.sin_addr.s_addr=inet_addr(icedemo.lipaddr); 
   icedemo.addr_local.sin_port=htons(icedemo.lport);
  
   if(bind(sfd,(struct sockaddr*)&icedemo.addr_local,icedemo.len)==-1){
    perror("BIND");
    return ;
    } 
   /* 
    if(connect(sfd,(struct sockaddr*)&addr_server,len)==-1){
     perror("CONNECT");
     return ;
   }
   */
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
  struct sockaddr_in addr;
  int addr_len=sizeof(struct sockaddr_in);
  for(;;){
    //n=read(icedemo.localfd,buffer,sizeof(buffer));
    n=recvfrom(icedemo.localfd,buffer,sizeof(buffer), 0 , (struct sockaddr *)&icedemo.addr_server ,&addr_len);   
    printf("receive from %s:%d\n" , inet_ntoa( icedemo.addr_server.sin_addr),ntohs(icedemo.addr_server.sin_port));
    buffer[n]='\0';
    printf("read %d bytes,data:%s\n",n,buffer); 
  
  }
}

static void* UDPSendThread(void* arg){
   int n;
   char buffer[1024]="client-----2";
   printf("UDPSendThread CREATED\n");
   printf("sleep 10\n");
   for(;;){
   //gets(&buffer); 
   sleep(3);
   n=strlen(buffer);
   //n=write(icedemo.localfd,buffer,n);
   n=sendto(icedemo.localfd,buffer,n,0,(struct sockaddr*)&icedemo.addr_server,icedemo.len);
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
