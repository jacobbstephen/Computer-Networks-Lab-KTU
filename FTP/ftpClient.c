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
void readFile(int clientSocket){
    FILE* fp = fopen("input.txt","r");
    if(fp == NULL){
        printf("Error in opening the file\n");
        exit(1);
    }
    int bytesReceived = 0;
    char buffer[SIZE];
    while((bytesReceived = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0){
        if(send(clientSocket,buffer,bytesReceived,0) < 0){
            printf("error in sending");
            close(clientSocket);
            return;
        }
    }
    fclose(fp);
    close(clientSocket);
}
int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set the server address structure
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT_NUMBER);
    serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Connection failed");
        close(clientSocket);
        exit(1);
    }
    readFile(clientSocket);
    // Close the socket
    close(clientSocket);

    return 0;
}
