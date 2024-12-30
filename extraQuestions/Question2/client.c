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
void printFileContents(int clientSocket){
    char buffer[SIZE];
    recv(clientSocket, buffer,SIZE,0);
    printf("Server: %s\n",buffer);
    memset(buffer,0,SIZE);
    strcpy(buffer,"input.txt");
    if(send(clientSocket,buffer,sizeof(buffer), 0) < 0){
        printf("Error in sending file name");
        return;
    }
    int bytesReceived;
    while((bytesReceived = recv(clientSocket, buffer,sizeof(buffer), 0)) > 0){
        if(bytesReceived < 0){
            printf("Error inn receving data\n");
            return;
        }
        buffer[bytesReceived] = '\0';
        printf("%s\n", buffer);
    }
    printf("All detials printed\n");
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
    printFileContents(clientSocket);
    // Close the socket
    close(clientSocket);

    return 0;
}
