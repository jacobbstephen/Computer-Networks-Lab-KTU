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
void writeFile(int clientSocket){
    char buffer[SIZE];
    FILE* fp = fopen("output.txt","w");
    if(fp == NULL){
        printf("FILE OPENING ERROR\n");
        exit(1);
    }
    int bytesReceived;
    while((bytesReceived = recv(clientSocket, buffer,sizeof(buffer), 0)) > 0){
        fwrite(buffer, sizeof(char), bytesReceived,fp);
    }
    printf("File saved\n");
    fclose(fp);
}
int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printf("Socket creation failed");
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
    if (listen(serverSocket, 6) < 0) {
        perror("Listen failed");
        close(serverSocket);
        exit(1);
    }
    printf("Listening...\n");
    struct sockaddr_in clientaddr;
    socklen_t lenOfClient = sizeof(clientaddr);
    int newSocket = accept(serverSocket, (struct sockaddr *)&clientaddr, &lenOfClient);
    if (newSocket < 0) {
        perror("Accept failed");
        close(serverSocket);
        exit(1);
    }
    writeFile(newSocket);
    // Close the sockets
    close(newSocket);
    close(serverSocket);
    return 0;
}