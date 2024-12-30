#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains the server address structure
#include <arpa/inet.h>
#include <unistd.h> // for close()
#include<math.h>
#define PORT_NUMBER 4569
#define IP_ADDRESS "127.0.0.1"
#define SIZE 1024
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
    if (listen(serverSocket, 2) < 0) {
        perror("Listen failed");
        close(serverSocket);
        exit(1);
    }
    printf("Listening...\n");
    // 1st client -> recieve float no
    struct sockaddr_in clientaddr1;
    socklen_t lenOfClient1 = sizeof(clientaddr1);
    int clientSocket1 = accept(serverSocket, (struct sockaddr *)&clientaddr1, &lenOfClient1);
    if (clientSocket1 < 0) {
        perror("Accept failed");
        close(serverSocket);
        exit(1);
    }
    float receviedNumber;
    if(recv(clientSocket1, &receviedNumber, sizeof(receviedNumber), 0) < 0){
        printf("Error in receving data\n");
        exit(1);
    }
    receviedNumber = powf(receviedNumber, 1.5);
    // client2 -> send the result
    struct sockaddr_in clientaddr2;
    socklen_t lenOfClient2 = sizeof(clientaddr2);
    int clientSocket2 = accept(serverSocket, (struct sockaddr *)&clientaddr2, &lenOfClient2);
    if (clientSocket2 < 0) {
        perror("Accept failed");
        close(serverSocket);
        exit(1);
    }
    printf("%f",receviedNumber);
     // send the result
    if(send(clientSocket2, &receviedNumber, sizeof(receviedNumber), 0) < 0){
        printf("Error in sending data\n");
        exit(1);
    }
    // Close the sockets
    close(clientSocket1);
    close(clientSocket2);
    close(serverSocket);
    return 0;
}