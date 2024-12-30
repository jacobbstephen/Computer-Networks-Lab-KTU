#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 7777
#define IP_ADDRESS "127.0.0.1"
#define WINDOW_SIZE 4

typedef struct {
    char data[1024];
} Packet;

typedef struct {
    int frameKind; // 0 => data, 1 => ack
    int seqNo;
    int ack; // The acknowledgment number
    Packet packet;
    int last;
} Frame;

void simulateSelectiveRepeat(int clientSocket) {
    Frame f_send[WINDOW_SIZE], f_recv;
    int base = 0;
    int nextSeqNum = 0;
    int f_size;
    int windowCount = 0;

    while (1) {
        // Send frames within the window
        while (nextSeqNum < base + WINDOW_SIZE && windowCount < WINDOW_SIZE) {
            f_send[nextSeqNum % WINDOW_SIZE].frameKind = 0;
            f_send[nextSeqNum % WINDOW_SIZE].seqNo = nextSeqNum;
            printf("Enter data to be sent (seqNo %d): ", nextSeqNum);
            scanf("%s", f_send[nextSeqNum % WINDOW_SIZE].packet.data);
            send(clientSocket, &f_send[nextSeqNum % WINDOW_SIZE], sizeof(f_send[nextSeqNum % WINDOW_SIZE]), 0);
            printf("[+] FRAME SENT [%d]\n", nextSeqNum);
            nextSeqNum++;
            windowCount++;
        }

        // Receive acknowledgment
        f_size = recv(clientSocket, &f_recv, sizeof(Frame), 0);
        if (f_size > 0 && f_recv.frameKind && f_recv.ack) {
            printf("[+] ACKNOWLEDGMENT RECEIVED [%d]\n", f_recv.seqNo);
            base = f_recv.last + 1;

            // Adjust windowCount based on new base
            if(base == nextSeqNum) windowCount = nextSeqNum - base;
        } else {
            printf("[+] ACKNOWLEDGMENT NOT RECEIVED\n");
            send(clientSocket, &f_send[base % WINDOW_SIZE], sizeof(f_send[base % WINDOW_SIZE]), 0);
        }
    }
}

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    connect(clientSocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    // Simulate Selective Repeat
    simulateSelectiveRepeat(clientSocket);

    close(clientSocket);
    return 0;
}
