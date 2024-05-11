#include <stdio.h>
#include <limits.h>
#define V 5
void bellmanFord(int graph[V][V], int startNode);
void initializeDist(int dist[V], int startNode);
void relaxation(int dist[V], int cost[V][V]);
void displayLenght(int dist[V], int startNode);
int main(){
    int graph[V][V] = {
        {0, 1, 0, 5, 0},
        {0, 0, -2, 0, 7},
        {0, 0, 0, 0, 1},
        {0, 3, -3, 0, 0},
        {0, 0, 0, 0, 0}
    };
    int startNode = 1;
    bellmanFord(graph, startNode);
    return 0;
}
void bellmanFord(int graph[V][V], int startNode){
    int dist[V];
    initializeDist(dist, startNode);  
    relaxation(dist, graph);
    displayLenght(dist, startNode); 
}
void initializeDist(int dist[V], int startNode){
    for(int i = 0; i < V; i++)
        dist[V] = INT_MAX;
    dist[startNode - 1] = 0;
}
void relaxation(int dist[V], int cost[V][V]){
    for(int k = 0; k < V - 1; k++){
        for(int i = 0; i < V; i++){
            for(int j = 0; j < V; j++){
                if(cost[i][j] != 0 && dist[i] != INT_MAX && dist[i] + cost[i][j] < dist[j]){
                    dist[j] = dist[i] + cost[i][j];
                }
            }
        }
    }
}
void displayLenght(int dist[V], int startNode){
    printf("Cel mai scurt drum de la nodul %d la toate celelalte noduri:\n", startNode);
    for(int i = 0; i < V; i++){
        printf("Nodul %d -> %d\n", i + 1, dist[i]);
    }
}
