#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef struct Stack{
    int *elements;
    int head;
    int capacity;
}Stack;
void memory(int ***arr, int n);
void inputGraphFile(int **graph, int e, FILE *ft);
void BellmanFord(int **cost, int n, int startNode, int destNode);
Stack *createStack(int capacity);
int isEmpty(Stack *stack);
void push(Stack *stack, int item);
int pop(Stack *stack);
void freeStack(Stack *stack);
void displayPath(int *distances, int *predecessors, int destNode);
void freeMemory(int **graph, int n);
int main() {
    int n, e, startNode, destNode, **graph = NULL;
    FILE *ft = NULL;
    printf("Introduceti nr. de noduri:");
    scanf("%d", &n); 
    printf("Introduceti nr. de muchii:");
    scanf("%d", &e);
    memory(&graph, n);
    ft = fopen("input.txt", "r");
    if(!ft){
        printf("Eroare la deschiderea fisierului!");
        exit(1);
    }
    inputGraphFile(graph, e, ft);
    fclose(ft);
    printf("Introduceti nodul de start:");
    scanf("%d", &startNode);
    printf("Introduceti nodul de destinatie:");
    scanf("%d", &destNode);
    BellmanFord(graph, n, startNode - 1, destNode - 1);
    freeMemory(graph, n);
    return 0;
}
void memory(int ***graph, int n) {
    *graph = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        (*graph)[i] = (int *)calloc(n, sizeof(int));
        if ((*graph)[i] == NULL) {
            printf("Eroare la alocare!");
            exit(1);
        }
    }
}
void inputGraphFile(int **graph, int e, FILE *ft) {
    for (int i = 0; i < e; i++) {
        int A = 0, B = 0, weight = 0;
        fscanf(ft, "%d %d %d", &A, &B, &weight);
        graph[A - 1][B - 1] = weight;
    }
}

void BellmanFord(int **cost, int n, int startNode, int destNode) {
    int *dist = (int *)malloc(n * sizeof(int));
    int *pred = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        dist[i] = INT_MAX;
        pred[i] = -1; 
    }
    dist[startNode] = 0;
    for (int k = 0; k < n - 1; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (cost[i][j] != 0 && dist[i] != INT_MAX && dist[i] + cost[i][j] < dist[j]) {
                    dist[j] = dist[i] + cost[i][j];
                    pred[j] = i; 
                }
            }
        }
    }
    displayPath(dist, pred, destNode);
    free(dist);
    free(pred);
}
Stack *createStack(int capacity){
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->elements = (int*)malloc(capacity * sizeof(int));
    stack->head = -1;
    stack->capacity = capacity;
    return stack;
}
int isEmpty(Stack *stack){
    return stack->head == -1;
}
void push(Stack *stack, int item){
    stack->elements[++stack->head] = item;
}
int pop(Stack *stack){
    return stack->elements[stack->head--];
}
void freeSatck(Stack *stack){
    free(stack->elements);
    free(stack);
}
void displayPath(int *distances, int *predecessors, int destNode) {
    printf("Distanta minima: %d\n", distances[destNode]);
    printf("Drumul: ");
    int currentNode = destNode;
    Stack *stack = createStack(50);
    while (currentNode != -1) {
        push(stack, currentNode);
        currentNode = predecessors[currentNode];
    }
    while(!isEmpty(stack)){
        printf("%d ", pop(stack) + 1);
    }
    printf("\n");
    free(stack);
}
void freeMemory(int **arr, int n){
    for(int i = 0; i < n; i++){
        free(arr[i]);
    }
    free(arr);
}