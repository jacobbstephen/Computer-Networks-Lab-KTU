#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains the server address structure
#include <arpa/inet.h>
#include <unistd.h> // for close()

#define PORT_NUMBER 9855
#define IP_ADDRESS "127.0.0.1"
#define SIZE 1024

void convertToUpperOrLowerCase(char buffer[], int choice) {
    for (int i = 0; i < strlen(buffer); i++) {
        if (choice == 1 && buffer[i] >= 'a' && buffer[i] <= 'z') {
            buffer[i] = buffer[i] - 32;
        } else if (choice == 0 && buffer[i] >= 'A' && buffer[i] <= 'Z') {
            buffer[i] = buffer[i] + 32;
        }
    }
}
void readFile(int clientSocket){
    int choice;
    char buffer[SIZE] = "HEY client send me the file name";
    if(send(clientSocket,buffer,sizeof(buffer),0) < 0){
        printf("error in requesting name of the file");
    }
    recv(clientSocket, buffer,SIZE,0);
    FILE* fp = fopen(buffer,"r");
    if(fp == NULL){
        printf("Error in opening the file\n");
        strcpy(buffer,"The requested file is not found");
        send(clientSocket,buffer,sizeof(buffer),0);
        return;
    }
    memset(buffer,0,SIZE);
    strcpy(buffer,"Send your choice (0 - > Lowercase) (1 -> UpperCase)");
    if(send(clientSocket,buffer,sizeof(buffer),0) < 0){
        printf("error in requesting choice");
    }
    recv(clientSocket, &choice,sizeof(choice),0);
    int bytesRead = 0;
    while((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0){
        convertToUpperOrLowerCase(buffer, choice);
        buffer[bytesRead] = '\0';
        if(send(clientSocket,buffer,bytesRead,0) < 0){
            printf("error in sending");
            close(clientSocket);
            return;
        }
        memset(buffer,0,SIZE);
    }
    fclose(fp);
    close(clientSocket);
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
    readFile(newSocket);
    // Close the sockets
    close(newSocket);
    close(serverSocket);
    return 0;
}