
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>
#define port 4444
#define IP_address "127.0.0.1"
int main(){

    struct sockaddr_in serveraddr;

    char buffer[1024];
    int clientsocket = socket(AF_INET,SOCK_STREAM,0);
    serveraddr.sin_family =  AF_INET;
    serveraddr.sin_port =  port;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    connect(clientsocket,(struct sockaddr*)&serveraddr, sizeof(serveraddr));
    while(1){
        printf("Enter data: ");
        scanf("%s",buffer);
        send(clientsocket,buffer,sizeof(buffer),0);
        if(strcmp(buffer,"exit") == 0){
            close(clientsocket);
            printf("Disconnected from Server\n");
            exit(1);
        }
        memset(buffer,sizeof(buffer), 0);
        int recvBytes = recv(clientsocket,buffer,sizeof(buffer),0);
        buffer[recvBytes] = '\0';
        printf("Server: %s\n",buffer);
        memset(buffer,sizeof(buffer), 0);
        
    }
    return(0);
}