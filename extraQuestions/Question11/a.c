#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#define INFINTY INT_MAX
#define MAX 10
typedef struct Edge{
    int src,dest,weight;
}edge;
struct Graph{
    int v,e,s;// v, e, s => no of vertices and edges and source
    edge edges[MAX];
}graph;
void getRouterDetials(){
    printf("Enter no of Router: ");
    scanf("%d",&graph.v);
    printf("Enter no of Links: ");
    scanf("%d",&graph.e);
    printf("Enter source node: ");
    scanf("%d",&graph.s);
    printf("Enter source, destination, weight\n");
    for(int i = 0; i < graph.e; i++)
        scanf("%d %d %d\n",&graph.edges[i].src,&graph.edges[i].dest,&graph.edges[i].weight);
}
void dvt(){
    int distance[MAX];
    int nexthop[MAX];
    for(int i = 0; i < graph.v; i++){
        distance[i] = INFINTY;
        nexthop[i] = -1;
    }
    distance[graph.s] = 0;
    for(int i = 0; i < graph.e; i++){
        int u = graph.edges[i].src;
        int v =  graph.edges[i].dest;
        int wt = graph.edges[i].weight;
        if(distance[u] != INFINTY  && distance[u] + wt < distance[v]){
            distance[v] = distance[u] + wt;
            nexthop[v] = u;
        }
    }
    for(int i = 0; i < graph.e; i++){
        int u = graph.edges[i].src;
        int v =  graph.edges[i].dest;
        int wt = graph.edges[i].weight;
        if(distance[u] != INFINTY  && distance[u] + wt < distance[v]){
            printf("Negative cycles Exist\n");
            return;
        }
    }
    printf("VERTEX\tISTANCE\tNEXTHOP\n");;
    for(int i  = 0; i < graph.v;i++){
        printf("%d\t%d\t%d\n",i,distance[i],nexthop[i]);
    }

}
int main(){
    getRouterDetials();
    dvt();
    return(0);
}