#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains the server address structure
#include <arpa/inet.h>
#include <unistd.h> // for close()
#include <ctype.h>
#define PORT_NUMBER 4444
#define IP_ADDRESS "127.0.0.1"
#define SIZE 1024
void countTheRequiredDetials(char buffer[], int count[]) {
    // count[0] => lines, count[1] => words, count[2] => characters
    int lines = 0;
    int words = 1;
    int characters = 2;
    int inWord = 0; // Flag to check if we are inside a word

    for (int i = 0; i < strlen(buffer); i++) {
        if (isalpha(buffer[i])) {
            count[characters]++;
            if (!inWord) {
                count[words]++;
                inWord = 1;
            }
        } else if (buffer[i] == '\n') {
            count[lines]++;
            inWord = 0;
        } else if (isspace(buffer[i])) {
            inWord = 0;
        }
    }
    if (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] != '\n') {
        count[lines]++;
    }
}
void readFile(int clientSocket){
    int choice;
    int count[3] = {0};
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
    int bytesRead = 0;
    while((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0){
        buffer[bytesRead] = '\0';
        countTheRequiredDetials(buffer,count);
        memset(buffer,0,SIZE);
    }
    printf("Lines ->%d\n",count[0]);
    printf("Words ->%d\n",count[1]);
    printf("Characters ->%d\n",count[2]);
    if(send(clientSocket, count, sizeof(count), 0) < 0){
        printf("Error in sending detials\n");
        exit(1);
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