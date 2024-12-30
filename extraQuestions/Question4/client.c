#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains the server address structure
#include <arpa/inet.h>
#include <unistd.h> // for close()

#define PORT_NUMBER 8884
#define IP_ADDRESS "127.0.0.1"
#define SIZE 1024

int main() {
    int alphabet[26] = {0};
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
    char buffer[SIZE];

    recv(clientSocket, buffer,SIZE,0);
    printf("Server: %s",buffer);
    memset(buffer,0,SIZE);
    printf("\nEnter the string to reverse: ");
    scanf("%s",buffer);
    if(send(clientSocket,buffer,sizeof(buffer), 0) < 0){
        printf("Error in sending the sentence");
        exit(1);
    }
    // memset(buffer,0,SIZE);
    recv(clientSocket, alphabet,sizeof(alphabet),0);

    for(int i = 0; i < strlen(buffer); i++){
        printf("%c -> %d\n", buffer[i], alphabet[buffer[i] - 'a']);
    }
    // Close the socket
    close(clientSocket);

    return 0;
}
