// // program for TCP server
// /*
//     steps
//     1. create a socket()
//     2. set the server address structure
//     3. bind socket to server address ie =>  associate the socket with specified address and  portq
//     4. listen for  incoming connections
//     5. accept the connection
//     5.recv data
//     6. send data
// */

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
    // Receive data
    char buffer[SIZE];
    memset(buffer, 0, SIZE); // Clear the buffer before receiving data
    int bytesReceived = recv(newSocket, buffer, SIZE, 0);
    if (bytesReceived < 0) {
        perror("Receive failed");
        close(newSocket);
        close(serverSocket);
        exit(1);
    }
    printf("Data received: %s\n", buffer);
    // Send data
    strcpy(buffer, "Hello from Server");
    int bytesSent = send(newSocket, buffer, strlen(buffer), 0);
    if (bytesSent < 0) {
        perror("Send failed");
        close(newSocket);
        close(serverSocket);
        exit(1);
    }
    // Close the sockets
    close(newSocket);
    close(serverSocket);
    return 0;
}
//The server address structure is used to specify the address and port number of the 
//server to which your client program will connect, or the server program will bind
//The accept() function creates a new socket for the connection with the client.
// Each client connection is handled by a separate socket. This allows the server to manage multiple client connections concurrently.
// The original listening socket (serversocket) remains in listening state, while the newly created socket (newsocket) is used for communication with the accepted client.