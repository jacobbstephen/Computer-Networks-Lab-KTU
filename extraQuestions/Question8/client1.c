#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains the server address structure
#include <arpa/inet.h>
#include <unistd.h> // for close()

#define PORT_NUMBER 4569
#define IP_ADDRESS "127.0.0.1"
#define SIZE 1024

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
   // send the float no
   float sendValue;
   printf("Enter the floating Number: ");
   scanf("%f",&sendValue);
   if(send(clientSocket, &sendValue, sizeof(sendValue), 0) < 0){
        printf("Error in receving data\n");
        exit(1);
    }
    // Close the socket
    close(clientSocket);

    return 0;
}
