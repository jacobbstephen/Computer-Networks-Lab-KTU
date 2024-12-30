#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

#define OUTGOINGRATE 1
#define BUCKETCAPACITY 10
#define ADD_PACKET 2
struct LeakyBucket{
    int size;
}bucket;

void addPacket(int packetSize){
    if(bucket.size + packetSize >= BUCKETCAPACITY){
        printf("Bucket OverFlow!!\n");
        return;
    }
    bucket.size += packetSize;
}

void simulateLeakyBucket(int duration){
    int packetSize = 0;
    // add packet at odd intervals and increase the size of the packet by some random amt;
    for(int i = 0; i < duration; i++){
        printf("Time: %d => Bucket Size: %d\n",i,bucket.size);
        if(bucket.size > 0){
            bucket.size -= OUTGOINGRATE;
            if(bucket.size < 0) bucket.size = 0;
        }
        // if(i % 2 == 1){
        //     srand(time(NULL)); // Seed the random number generator
        //     int randomNo = rand() % 4;
        //     packetSize += randomNo;
        //     addPacket(packetSize);
        // }

        if(i%2 == 1){
            packetSize  = packetSize +  ADD_PACKET;
            addPacket(packetSize);
        }
        sleep(1);
    }
}
int main(){
    int duration = 10;
    bucket.size = 0;
    addPacket(5);
    simulateLeakyBucket(duration);
    return(0);
}