#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include<netinet/in.h>
#include<time.h>
#define port 4444
#define IP_ADDRESS "127.0.0.1"
#define SIZE 1024
int main(){
    struct sockaddr_in serveraddr, clientaddr;
    int n;
    time_t current_time;
    int serversocket = socket(AF_INET,SOCK_STREAM,0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    bind(serversocket,(struct sockaddr*) &serveraddr,sizeof(serveraddr));
    
    listen(serversocket,6);
    int len = sizeof(clientaddr);
    int clientsocket = accept(serversocket,(struct sockaddr *) &clientaddr,&len);
        recv(clientsocket,&n,sizeof(n),0);
        current_time = time(NULL);
        printf("Client asked for time at  %s\n",ctime(&current_time));
        send(clientsocket,&current_time,sizeof(current_time),0);
    return(0);
}