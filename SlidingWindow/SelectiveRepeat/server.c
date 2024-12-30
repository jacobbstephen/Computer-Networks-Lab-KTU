#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define PORT 7777
#define IP_ADDRESS "127.0.0.1"
#define WINDOW_SIZE 4

typedef struct {
    char data[1024];
} Packet;

typedef struct {
    int frameKind; // 0 => data, 1 => ack
    int seqNo;
    int ack; // 1 => ack
    Packet packet;
    int last; // last acknowledgement frame no
} Frame;

void simulateSelectiveRepeat(int clientSocket) {
    Frame f_send;
    Frame f_recv;
    Frame buffer[WINDOW_SIZE];
    int expectedSeqNum = 0;
    int f_size;
    int temp = 1;
    int i = 0;

    // Initialize the buffer sequence numbers to -1
    for (int j = 0; j < WINDOW_SIZE; j++) {
        buffer[j].seqNo = -1;
    }

    while (1) {
        // Receive the message
        f_size = recv(clientSocket, &f_recv, sizeof(Frame), 0);

        // Simulate frame loss
        if (temp % 6 == 3)
            f_size = -1;

        if (f_size > 0 && !f_recv.frameKind) {
            if (f_recv.seqNo == expectedSeqNum) {
                // Send acknowledgement
                printf("[+] FRAME RECEIVED [%d]\n", f_recv.seqNo);
                printf("Sender: %s\n", f_recv.packet.data);

                f_send.frameKind = 1;
                f_send.ack = 1;
                f_send.seqNo = f_recv.seqNo;
                f_send.last = expectedSeqNum;
                send(clientSocket, &f_send, sizeof(f_send), 0);
                printf("[+] ACKNOWLEDGMENT SENT [%d]\n", expectedSeqNum);
                expectedSeqNum++;
                i = (i + 1) % WINDOW_SIZE;

                // Process buffered frames
                while (buffer[expectedSeqNum % WINDOW_SIZE].seqNo == expectedSeqNum) {
                    printf("[+] BUFFERED FRAME RECEIVED [%d]\n", buffer[expectedSeqNum % WINDOW_SIZE].seqNo);
                    printf("Sender: %s\n", buffer[expectedSeqNum % WINDOW_SIZE].packet.data);

                    f_send.frameKind = 1;
                    f_send.ack = 1;
                    f_send.seqNo = buffer[expectedSeqNum % WINDOW_SIZE].seqNo;
                    f_send.last = expectedSeqNum;
                    send(clientSocket, &f_send, sizeof(f_send), 0);
                    printf("[+] ACKNOWLEDGMENT SENT [%d]\n", expectedSeqNum);
                    buffer[expectedSeqNum % WINDOW_SIZE].seqNo = -1; // Mark buffer slot as empty
                    expectedSeqNum++;
                }
            } else {
                printf("[+] STORED IN BUFFER=> Received: [%d] => Expected: [%d]\n", f_recv.seqNo, expectedSeqNum);
                buffer[f_recv.seqNo % WINDOW_SIZE] = f_recv;
            }
        } else {
            printf("[+] FRAME NOT RECEIVED [+]\n");
            f_send.frameKind = 1;
            f_send.ack = 0;
            f_send.seqNo = expectedSeqNum - 1;
            f_send.last = expectedSeqNum - 1;
            send(clientSocket, &f_send, sizeof(f_send), 0);
            printf("[+] -ve ACKNOWLEDGMENT SENT [%d]\n", expectedSeqNum);
        }
        temp++;
    }
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr, clientaddr;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(serverSocket, 1);

    socklen_t len = sizeof(clientaddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientaddr, &len);

    // simulateSelectiveRepeat
    simulateSelectiveRepeat(clientSocket);

    close(clientSocket);
    return 0;
}