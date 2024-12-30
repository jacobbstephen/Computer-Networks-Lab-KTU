#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
//1. intialise single source, for(v in G.V -1){for e in G.E{Relax}}
#define MAX 10
#define INFINITY INT_MAX

typedef struct {
    int src, dest, weight;
}Edge;

struct Graph{
    int v, e,src;
    Edge edges[MAX];
};
struct Graph graph;
void getRouterDetials(){
    printf("Enter no of Routers: ");
    scanf("%d",&graph.v);
    printf("Enter no of Links: ");
    scanf("%d",&graph.e);
    printf("Enter the source destination and weight for each link\n");
    for(int i = 0; i < graph.e; i++)
        scanf("%d %d %d",&graph.edges[i].src,&graph.edges[i].dest, &graph.edges[i].weight);

}
void intialiseSingleSource(int sourceNode, int distance[], int intermediateHop[]){
    graph.src = sourceNode;
    for( int i = 0; i < graph.v; i++){
        distance[i] = INFINITY;
        intermediateHop[i] = -1;
    }
    distance[sourceNode] = 0;
    intermediateHop[sourceNode] = sourceNode;
}
void Relax(int u,int v,int wt,int distance[], int intermediateHop[], int sourceNode){
    if(distance[u] != INFINITY && distance[v] > distance[u] + wt){
        distance[v] = distance[u] + wt;
        intermediateHop[v] = (u == sourceNode)? v: intermediateHop[u];
    }
}
void printRoutingTable(int distance[], int intermediateHop[], int sourceNode) {
    printf("Routing table for node %d:\n", sourceNode);
    printf("VERTEX\tDISTANCE\tintermediateHop\n");
    for (int i = 0; i < graph.v; i++) {
        printf("%d\t%d\t\t%d\n", i, distance[i], intermediateHop[i]);
    }
    printf("\n");
}
void dvr(int sourceNode){
    graph.src = sourceNode;
    int distance[MAX];
    int intermediateHop[MAX];
    intialiseSingleSource(sourceNode,distance, intermediateHop);
    for(int i = 0; i < graph.v - 1; i++){
        for(int j = 0; j < graph.e; j++){
            int u = graph.edges[j].src;
            int v = graph.edges[j].dest;
            int wt = graph.edges[j].weight;
            Relax(u,v,wt,distance, intermediateHop, sourceNode);
        }
    }
    for(int i = 0; i < graph.e; i++){
        int u = graph.edges[i].src;
        int v =  graph.edges[i].dest;
        int wt = graph.edges[i].weight;
        if(distance[u] != INFINITY  && distance[u] + wt < distance[v]){
            printf("Negative cycles Exist\n");
            return;
        }
    }
    printRoutingTable(distance, intermediateHop, sourceNode);
}
int main(){
    getRouterDetials();
    for(int i = 0; i < graph.v; i++)
        dvr(i);
    return (0);
}

