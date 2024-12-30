#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains the server address structure
#include <arpa/inet.h>
#include <unistd.h> // for close()

#define PORT_NUMBER 11446
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

    // just sending a random message stating that the client is ready
    char initMessage[SIZE] = "Ready to receive";
    if (sendto(clientSocket, initMessage, sizeof(initMessage), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Error sending initial message");
        close(clientSocket);
        exit(1);
    }

    int randomNo;
    socklen_t serveraddrLen = sizeof(serveraddr);

    // Receive the random number from the server
    int recBytes = recvfrom(clientSocket, &randomNo, sizeof(randomNo), 0, (struct sockaddr *)&serveraddr, &serveraddrLen);
    if (recBytes < 0) {
        perror("Error receiving data");
        close(clientSocket);
        exit(1);
    }

    printf("Received Random No: %d\n", randomNo);

    close(clientSocket);
    return 0;
}
