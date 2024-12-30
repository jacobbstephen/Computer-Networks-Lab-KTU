//UDP Client
/*
    1.  Socket creation
    2. set the server address
    3. send data using sendto
*/

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
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT_NUMBER); // Convert port number to network byte order
    serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    char buffer[SIZE] = "Hello From CLient";
    sendto(clientSocket,buffer,sizeof(buffer),0,(struct sockaddr*) &serveraddr, sizeof(serveraddr));
    memset(buffer, 0, SIZE);
    socklen_t len = sizeof(serveraddr);
    int recBytes = recvfrom(clientSocket, buffer, SIZE - 1, 0,(struct sockaddr *) &serveraddr,&len);
    if(recBytes < 0){
        printf("Error in reading the data");
        exit(1);
    }
    buffer[recBytes] = '\0';
    printf("%s\n", buffer);
    close(clientSocket);
    return 0;
}