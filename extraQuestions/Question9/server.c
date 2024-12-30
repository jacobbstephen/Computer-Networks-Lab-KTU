#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains the server address structure
#include <arpa/inet.h>
#include <unistd.h> // for close()
#include <time.h>

#define PORT_NUMBER 11446
#define IP_ADDRESS "127.0.0.1"
#define SIZE 1024

int main() {
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
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

    struct sockaddr_in clientaddr;
    socklen_t clientaddrLen = sizeof(clientaddr);
    
    // RANDOM NUMBER GENERATION
    srand(time(NULL)); // Seed the random number generator
    int randomNo = (rand() % 6) + 1;
    printf("Generated Random No: %d\n", randomNo);

    // Receive a datagram from the client to ensure client is ready to receive
    char buffer[SIZE];
    recvfrom(serverSocket, buffer, SIZE, 0, (struct sockaddr *)&clientaddr, &clientaddrLen);

    // Send the random number to the client
    if (sendto(serverSocket, &randomNo, sizeof(randomNo), 0, (struct sockaddr *)&clientaddr, clientaddrLen) < 0) {
        perror("Error sending data");
        close(serverSocket);
        exit(1);
    }

    printf("Random number sent to client\n");

    close(serverSocket);
    return 0;
}
