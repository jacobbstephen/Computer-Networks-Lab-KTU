// program is to calculate the time of server

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include<netinet/in.h>
#define SIZE 1024
#define port 4444
#define IP_ADDRESS "127.0.0.1"
#include <arpa/inet.h>
#include<time.h>


int main() {
    int n = 2;
    struct sockaddr_in serveraddr;
    time_t current_time,start_time,rtt ;
    int serversocket = socket(AF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    connect(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    start_time = time(NULL);

    send(serversocket,&n,sizeof(n),0);
    recv(serversocket,&current_time,sizeof(current_time),0);
    rtt = time(NULL) - start_time;
    current_time += rtt/2;
    printf("Server time = %s",ctime(&current_time));
    return 0;
}
