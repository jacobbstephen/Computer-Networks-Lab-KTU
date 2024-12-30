// server receives the frame send by client
//  create a frame -> bakki steps-> recv the frame and send acknowldgement

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
    int temp = 0;// just to show that it is not acknowldged
    while(1){
        f_size = recv(clientSocket, &f_recv, sizeof(f_recv), 0);
        if( temp % 2) f_size = -1;// just randomly miss the frame
        if(f_size > 0 && !f_recv.frameKind && f_recv.seqNo == seqNo){
            printf("[+] Frame Received [+]\n");
            printf("Sender: %s\n",f_recv.packet.data);
            f_send.ack = 1;
            f_send.frameKind = 1;
            f_send.seqNo = f_recv.seqNo;
            send(clientSocket,&f_send,sizeof(f_send),0);
            printf("[+] ACKNOWLDGMENT SEND[+]\n");
            seqNo++;
        }
        else if(f_recv.seqNo != seqNo)
            printf("Wrong Sequence Number\n");
        else{
            f_send.ack = 0;
            f_send.frameKind = 1;
            f_send.seqNo = f_recv.seqNo;
            send(clientSocket,&f_send,sizeof(f_send),0);
        }
        temp++;
    }
}

int main(){
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serveraddr, clientaddr;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (struct sockaddr* )&serveraddr, sizeof(serveraddr));
    listen (serverSocket, 1);
    socklen_t len = sizeof(clientaddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientaddr, &len);

    // simulate StopAndWait
    simulateStopAndWait(clientSocket);
    close(clientSocket);
    return(0);
}