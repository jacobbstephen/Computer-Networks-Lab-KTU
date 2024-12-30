 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains the server address structure
#include <arpa/inet.h>
#include <unistd.h> // for close()

#define PORT_NUMBER 3875
#define IP_ADDRESS "127.0.0.1"
#define SIZE 1024

int main() {
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT_NUMBER); // Convert port number to network byte order
    serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    int bindSuccessful = bind(serverSocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (bindSuccessful < 0) {
        perror("Binding failed");
        close(serverSocket);
        exit(1);
    }
    printf("Binding successful\n");
    char buffer[SIZE];
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    
    int recBytes = recvfrom(serverSocket, buffer, SIZE, 0,(struct sockaddr *) &clientaddr,&len);
    if(recBytes < 0){
        printf("Error in reading the data");
        exit(1);
    }
    buffer[recBytes] = '\0';
    printf("%s\n", buffer);

    strcpy(buffer,"Hello From Server");
    // printf("%s\n", buffer);
    sendto(serverSocket,buffer,sizeof(buffer),0,(struct sockaddr*) &clientaddr, sizeof(clientaddr));
    close(serverSocket);
    return 0;
}

// a protocol is a set of rules and conventions that define how data is transmitted and received across a network