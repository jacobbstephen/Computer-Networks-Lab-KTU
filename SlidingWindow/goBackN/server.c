

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define PORT 8888
#define IP_ADDRESS "127.0.0.1"
#define WINDOW_SIZE 4

typedef struct
{
    char data[1024];
} Packet;

typedef struct
{
    int frameKind; // 0 => data, 1 => ack
    int seqNo;
    int ack; // 1 => ack
    Packet packet;
    int last; // last acknowldgement frame no
} Frame;

void simulateGoBackN(int clientSocket)
{
    Frame f_send;
    Frame f_recv;
    int expectedSeqNum = 0;
    int f_size;
    int temp = 1;
    while (1)
    {
        // receive the message
        f_size = recv(clientSocket, &f_recv, sizeof(Frame), 0);
        if (temp % 5 == 3)
            f_size = -1;
        if (f_size > 0 && !f_recv.frameKind)
        {
            if (f_recv.seqNo == expectedSeqNum)
            {
                // send acknowldgement
                printf("[+] FRAME RECEIVED [%d]\n", f_recv.seqNo);
                printf("Sender: %s\n", f_recv.packet.data);

                f_send.frameKind = 1;
                f_send.ack = 1;
                f_send.seqNo = f_recv.seqNo;
                f_send.last = expectedSeqNum;
                send(clientSocket, &f_send, sizeof(f_send), 0);
                printf("[+] ACKNOWLEDGMENT SENT [%d]\n", expectedSeqNum);
                expectedSeqNum++;
            }else{
                printf("[+] NEGATIVE ACKNOWLEDGMENT SENT=> Received: [%d] => Expected: [%d]\n",f_recv.seqNo, expectedSeqNum);
                // f_send.frameKind = 1;
                // f_send.ack = 0;
                // f_send.seqNo = f_recv.seqNo;
                // f_send.last = expectedSeqNum - 1;
                // send(clientSocket, &f_send, sizeof(f_send), 0);
            }
        }
        else
        {
            printf("[+] FRAME NOT RECEIVED [+]\n");
            f_send.frameKind = 1;
            f_send.ack = 0;
            f_send.seqNo = f_recv.seqNo;
            f_send.last = expectedSeqNum - 1;
            send(clientSocket, &f_send, sizeof(f_send), 0);
            printf("[+] -ve ACKNOWLEDGMENT SENT [%d]\n", expectedSeqNum);
        }
        temp++;
    }
}

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr, clientaddr;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(serverSocket, 1);

    socklen_t len = sizeof(clientaddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientaddr, &len);

    // Simulate Go-Back-N
    simulateGoBackN(clientSocket);

    close(clientSocket);
    return 0;
}

