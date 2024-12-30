// #include<stdio.h>
// #include<limits.h>
// #include<stdlib.h>

// #define MAX 10
// #define INFINITY INT_MAX

// typedef struct Edge{
//     int src, dest, weight;
// }edge;

// struct Graph{
//     int v, e, src;  // store the count of each 
//     edge edges[MAX];
// }graph;

// // The next hop for a node v tells you the first node you should go to if you want to travel from the source to v.
// void printRoutingTable(int distance[], int nexthop[], int sourceNode) {
//     printf("Routing table for node %d:\n", sourceNode);
//     printf("VERTEX\tDISTANCE\tNEXTHOP\n");
//     for (int i = 0; i < graph.v; i++) {
//         printf("%d\t%d\t\t%d\n", i, distance[i], nexthop[i]);
//     }
//     printf("\n");
// }
// void initialiseSingleSource(int sourceNode, int distance[], int nextHop[]){
//     graph.src = sourceNode;
//     for(int i = 0; i < graph.v; i++){
//         distance[i] = INFINITY;
//         nextHop[i] = -1;
//     }
//     distance[sourceNode] = 0;
// }
// void Relax(int u,int v, int wt, int sourceNode, int distance[], int nextHop[]){
//      if(distance[u] != INFINITY  && distance[u] + wt < distance[v]){
//             distance[v] = distance[u] + wt;
//             nextHop[v] = (u == sourceNode)? v : nextHop[u];
//         }
// }
// void dvt(int sourceNode){
//     int distance[MAX];
//     int nextHop[MAX];
//    initialiseSingleSource(sourceNode,distance,nextHop);
//    for(int i = 0; i < graph.v - 1; i++){
//         for(int j = 0; j < graph.e;j++){
//             int u = graph.edges[j].src;
//             int v = graph.edges[j].dest;
//             int wt = graph.edges[j].weight;
//             Relax(u,v,wt, sourceNode, distance,nextHop);
//         }
//    }
//    for(int i = 0; i < graph.e; i++){
//         int u = graph.edges[i].src;
//         int v =  graph.edges[i].dest;
//         int wt = graph.edges[i].weight;
//         if(distance[u] != INFINITY  && distance[u] + wt < distance[v]){
//             printf("Negative cycles Exist\n");
//             return;
//         }
//     }

//     printRoutingTable(distance, nextHop, sourceNode);

// }
// void getRouterDetials(){
//     printf("Enter no of Router: ");
//     scanf("%d",&graph.v);
//     printf("Enter no of Links: ");
//     scanf("%d",&graph.e);
//     printf("Enter source destination weight for each link:\n");
//     for(int i = 0; i < graph.e; i++)
//         scanf("%d %d %d",&graph.edges[i].src, &graph.edges[i].dest,&graph.edges[i].weight);
// }
// int main(){
//     getRouterDetials();
//     for(int i = 0; i < graph.v;i++){
//         dvt(i);
//     }
//     return(0);
// }



#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define MAX 10
#define INFINITY INT_MAX

typedef struct Edge {
    int src, dest, weight;
} edge;

typedef struct Graph {
    int v, e;  // Number of vertices and edges
    edge edges[MAX];
} Graph;

// Print the routing table for a given source node
void printRoutingTable(int distance[], int nextHop[], int sourceNode, int v) {
    printf("Routing table for node %d:\n", sourceNode);
    printf("VERTEX\tDISTANCE\tNEXTHOP\n");
    for (int i = 0; i < v; i++) {
        printf("%d\t%d\t\t%d\n", i, distance[i], nextHop[i]);
    }
    printf("\n");
}

// Initialize single source for Bellman-Ford
void initialiseSingleSource(int sourceNode, int distance[], int nextHop[], int v) {
    for (int i = 0; i < v; i++) {
        distance[i] = INFINITY;
        nextHop[i] = -1;
    }
    distance[sourceNode] = 0;
}

// Relax an edge (u, v) with weight wt
void Relax(int u, int v, int wt, int distance[], int nextHop[]) {
    if (distance[u] != INFINITY && distance[u] + wt < distance[v]) {
        distance[v] = distance[u] + wt;
        nextHop[v] = nextHop[u] == -1 ? u : nextHop[u];
    }
}

// Run Bellman-Ford algorithm from a single source node
void dvt(Graph *graph, int sourceNode) {
    int distance[MAX];
    int nextHop[MAX];
    initialiseSingleSource(sourceNode, distance, nextHop, graph->v);

    // Relax edges (v-1) times
    for (int i = 0; i < graph->v - 1; i++) {
        for (int j = 0; j < graph->e; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int wt = graph->edges[j].weight;
            Relax(u, v, wt, distance, nextHop);
        }
    }

    // Check for negative-weight cycles
    for (int i = 0; i < graph->e; i++) {
        int u = graph->edges[i].src;
        int v = graph->edges[i].dest;
        int wt = graph->edges[i].weight;
        if (distance[u] != INFINITY && distance[u] + wt < distance[v]) {
            printf("Negative cycles exist\n");
            return;
        }
    }

    printRoutingTable(distance, nextHop, sourceNode, graph->v);
}

// Get graph details from user
void getRouterDetails(Graph *graph) {
    printf("Enter number of Routers: ");
    scanf("%d", &graph->v);
    if (graph->v > MAX) {
        printf("Number of routers exceeds the maximum limit of %d\n", MAX);
        exit(1);
    }

    printf("Enter number of Links: ");
    scanf("%d", &graph->e);
    if (graph->e > MAX) {
        printf("Number of links exceeds the maximum limit of %d\n", MAX);
        exit(1);
    }

    printf("Enter source, destination, weight for each link:\n");
    for (int i = 0; i < graph->e; i++) {
        scanf("%d %d %d", &graph->edges[i].src, &graph->edges[i].dest, &graph->edges[i].weight);
    }
}

int main() {
    Graph graph;
    getRouterDetails(&graph);

    for (int i = 0; i < graph.v; i++) {
        dvt(&graph, i);
    }

    return 0;
}
