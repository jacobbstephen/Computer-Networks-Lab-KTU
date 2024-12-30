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

void countNoOfCharacters(char buffer[], int alphabet[]){
   int len = strlen(buffer);
    for(int i = 0; i < len; i++){
        alphabet[buffer[i] - 'a']++;
    }
}
int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int alphabet[26] = {0};
    // char temp[]  = "jacobb";
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
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientaddr, &lenOfClient);
    if (clientSocket < 0) {
        perror("Accept failed");
        close(serverSocket);
        exit(1);
    }
    char buffer[SIZE] = "HEY client send me the sentence to count the no of Characters";
    if(send(clientSocket,buffer,sizeof(buffer),0) < 0){
        printf("error in sending the request");
    }
    memset(buffer,0,SIZE);
    recv(clientSocket, buffer,SIZE,0);
    printf("\n Client send: %s\n",buffer);
    countNoOfCharacters(buffer, alphabet);
    printf("Finished counting\n");
    for(int i = 0; i < strlen(buffer); i++){
        printf("%c -> %d\n", buffer[i], alphabet[buffer[i] - 'a']);
    }
    if(send(clientSocket,alphabet,sizeof(alphabet),0) < 0){
        printf("error in sending the reversed string");
    }
    // Close the sockets
    close(clientSocket);
    close(serverSocket);
    return 0;
}