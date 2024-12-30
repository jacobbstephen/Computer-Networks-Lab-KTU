#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains the server address structure
#include <arpa/inet.h>
#include <unistd.h> // for close()

#define PORT_NUMBER 4444
#define IP_ADDRESS "127.0.0.1"
#define SIZE 1024
void printRequiredDetials(int clientSocket){
    char buffer[SIZE];
    recv(clientSocket, buffer,SIZE,0);
    printf("Server: %s\n",buffer);
    memset(buffer,0,SIZE);
    strcpy(buffer,"input.txt");
    if(send(clientSocket,buffer,sizeof(buffer), 0) < 0){
        printf("Error in sending file name");
        return;
    }
    
    memset(buffer,0,SIZE);
    int bytesReceived;
    int count[3];
    if(recv(clientSocket, count,sizeof(count), 0) < 0){
        printf("Error in receiving the detials");
        exit(1);
    }
    printf("Lines ->%d\n",count[0]);
    printf("Words ->%d\n",count[1]);
    printf("Characters ->%d\n",count[2]);
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
        printf("Connection failed");
        close(clientSocket);
        exit(1);
    }
    printRequiredDetials(clientSocket);
    // Close the socket
    close(clientSocket);

    return 0;
}
