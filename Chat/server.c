#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define port 4444
#define IP_address "127.0.0.1"
void handleClient(int clientSocket){
    char buffer[1024];
    while(1){
        int recv_bytes = recv(clientSocket,buffer, sizeof(buffer),0);
        if (recv_bytes <= 0) {
            printf("Disconnected from Client %d\n", clientSocket);
            break;
        }
        buffer[recv_bytes] = '\0';
        if (strcmp(buffer, "exit") == 0) {
            printf("Disconnected from Client %d\n", clientSocket);
            break;
        }else {
            printf("Client: %s\n", buffer);
            printf("Enter Message: ");
            scanf("%s",buffer);
            send(clientSocket, buffer, strlen(buffer), 0);
            memset(buffer,sizeof(buffer),0);
        }
    }
    close(clientSocket);
    exit(0);
}
int main()
{
    char buffer[1024];
    struct sockaddr_in serveraddr, clientaddr;
    pid_t childpid;
    int serversocket = socket(AF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = port;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    bind(serversocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(serversocket, 6);
    printf("Listening\n");

    while (1){
        socklen_t len = sizeof(clientaddr);
        int clientSocket = accept(serversocket,(struct sockaddr * ) &clientaddr, &len);
        printf("CONNECTED TO CLIENT\n");

        int childpid = fork();
        if(childpid < 0) {
            printf("Error\n");
            close(clientSocket);
            continue;
        }
        else if(childpid == 0){
            close(serversocket);
            handleClient(clientSocket);

        }else{
            close(clientSocket);
        }
    }
    close(serversocket);
    return (0);
}
