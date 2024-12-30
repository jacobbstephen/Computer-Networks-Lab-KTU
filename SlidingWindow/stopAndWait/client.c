#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define PORT 8888
#define IP_ADDRESS "127.0.0.1"
// declare the frame{framekind, seqno, ack,packet}
typedef struct {
    char data[1024];
}Packet;

typedef struct{
    int frameKind;// 0=> data, 1=> ack
    int seqNo;
    int ack;
    Packet packet;
}Frame;

void simulateStopAndWait(int clientSocket){
    Frame f_send, f_recv;
    int seqNo = 0; int f_size;
    int ack = 1;// just to show that it is acknowldged
    while(1){
        // if ack then only send data
        if(ack){
            f_send.frameKind = 0;
            f_send.seqNo = seqNo;
            printf("Enter the data to be send to the server: ");
            scanf("%s",f_send.packet.data);
            send(clientSocket,&f_send,sizeof(f_send),0);
            printf("[+] FRAME SEND[+]\n");
        }

        //  receive ack for the frame send
        f_size = recv(clientSocket, &f_recv, sizeof(f_recv), 0);
        if(f_size > 0 && f_recv.frameKind && f_recv.ack && f_recv.seqNo == seqNo){
            printf("[+]ACKNOWLDGMENT RECEIVED[+]\n");
            ack = 1;
            seqNo ++;
        }
        else{
            printf("[+]ACKNOWLDGMENT NOT RECEIVED[+]\n");
            send(clientSocket,&f_send, sizeof(f_send),0);
            printf("[+]Frame Send AGAIN [+]\n");
            ack = 0;
        }
    }
}

int main(){
    int clientSocket = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serveraddr;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    connect(clientSocket, (struct sockaddr* )&serveraddr, sizeof(serveraddr));
    // simulate StopAndWait
    simulateStopAndWait(clientSocket);
    close(clientSocket);
    return(0);
}