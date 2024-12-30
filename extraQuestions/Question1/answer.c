#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains the server address structure
#include <arpa/inet.h>
#include <unistd.h> // for close()
#include<time.h>

#define PORT_NUMBER 3875
#define IP_ADDRESS "127.0.0.1"
#define SIZE 1024

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0 || clientSocket < 0) {
        printf("Socket creation failed");
        exit(1);
    }
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT_NUMBER); // Convert port number to network byte order
    serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    int f = fork();
    if(f == 0){
        // Child process -> request for time and print the pid
        printf("Client pid: %d\n", getpid());
        sleep(5);
        // connect-> send-> recv data-> print
        if (connect(clientSocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
            printf("Connection failed");
            close(clientSocket);
            exit(1);
        }
        printf("Connected to server");

        char buffer[SIZE] = "Hey server whats the time??";
        if (send(clientSocket, buffer, strlen(buffer), 0) < 0) {
            printf("Send failed");
            close(clientSocket);
            exit(1);
        }

        // receieve the time

        time_t currentTime;

        if (recv(clientSocket, &currentTime, sizeof(currentTime), 0) < 0) {
            printf("Receive failed");
            close(clientSocket);
            exit(1);
        }
        printf("Time: : %s\n", ctime(&currentTime));


        
    }else if(f > 0){
        // parent process -> send the time and print the pid
        printf("Server pid: %d\n", getpid());
        sleep(2);
        // bind->listen->accept-> recv -> send
        int bindSuccessful = bind(serverSocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        printf("Binding successful\n");

        printf("Listening...\n");
        listen(serverSocket,6);
        // accept
        struct sockaddr_in clientaddr;
        socklen_t lenOfClient = sizeof(clientaddr);
        int newSocket = accept(serverSocket, (struct sockaddr *)&clientaddr, &lenOfClient);
        // recv message
        char buffer[SIZE];
        memset(buffer, 0, SIZE); // Clear the buffer before receiving data
        int bytesReceived = recv(newSocket, buffer, SIZE, 0);
        printf("Client: %s\n", buffer);
        // send time
        time_t  current_time = time(NULL);
        send(newSocket, &current_time, sizeof(current_time), 0);

        close(newSocket);


    } else{
        printf("Error In Forking");
    }

    // Close the sockets
    close(serverSocket);
    close(clientSocket);
    return 0;
}

