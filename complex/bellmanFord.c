#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
typedef struct
{
    int id;
    char *name;
    float balance;
} wallet;
typedef struct queueNode{
    int data;
    struct queueNode *next;
}queueNode;
typedef struct {
    queueNode *front, *rear;
}queue;
void memory(int ***graph, wallet **arr, int V);
void inputGraph(int **graph, int E, FILE *ft);
void bellmanFordMin(int **graph, int V, int src, int dest, wallet *wallets);
void bellmanFordMax(int **graph, int V, int src, int dest, wallet *wallets);
int timeCalculation(int time_confirmation, int option);
void transaction(wallet *wallets, int **graph, int sender_id, int receiver_id, float amount, int option);
queue *createQueue();
int isEmpty(queue *q);
void enqueue(queue *q, int data);
int dequeue(queue *q);
void printPath(int parent[], int dest, wallet *wallets, int V);
void printWalletsBalance(wallet *wallets, int V);
int main()
{
    int V = 5, E = 7, **graph = NULL, sender_id, receiver_id, option;
    FILE *ft = NULL;
    float amount;
    wallet *arr = NULL;
    memory(&graph, &arr, V);
    ft = fopen("inputGraph.txt", "r");
    if (!ft)
    {
        printf("Eroare la deschiderea fisierului!");
        exit(1);
    }
    inputGraph(graph, E, ft);
    wallet wallets[] = {
        {1, "0x7a", 400.0},
        {2, "0x7b", 200.0},
        {3, "0x7c", 300.0},
        {4, "0x7d", 400.0},
        {5, "0x7e", 500.0}};

    do
    {
        sender_id = 1;
        printf("1. Efectuarea unei tranzactii\n");
        printf("2. Vizualiarea balantei conturilor\n");
        printf("3. Vizualizarea caii\n");
        printf("0. Iesire\n");
        printf("Introduceti optiunea: ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("Introduceti ID-ul wallet-ului destinatar:");
            scanf("%d", &receiver_id);
            if(receiver_id > V){
                printf("Nu exista acest wallet id!\n");
                break;
            }
            printf("Introduceti suma tranzactiei:");
            scanf("%f", &amount);
            printf("1. Timp de confirmare mai lung (comision: $0.20)\n");
            printf("2. Timp de confirmare mai scurt (comision: 10%% din suma)\n");
            printf("Introduceti numarul optiunii dorite: ");
            int timeConf;
            scanf("%d", &timeConf);
            transaction(wallets, graph, sender_id, receiver_id, amount, timeConf);
            break;
        case 2:
            printWalletsBalance(wallets, V);
            break;
        case 3:
            if(timeConf == 1){
                bellmanFordMax(graph, V, sender_id - 1, receiver_id - 1, wallets);
            }else{
                bellmanFordMin(graph, V, sender_id - 1, receiver_id - 1, wallets);
            }
            break;
        case 0:
            printf("Iesire din program.\n");
            return 0;
        default:
            printf("Optiune invalida! Va rugam sa alegeti din nou.\n");
            break;
        }
    } while (1);
    return 0;
}
void memory(int ***graph, wallet **arr, int V)
{
    *graph = (int **)malloc(V * sizeof(int *));
    *arr = (wallet *)malloc(V * sizeof(wallet));
    for (int i = 0; i < V; i++)
    {
        (*graph)[i] = calloc(V, sizeof(int));
        if ((*graph)[i] == NULL)
        {
            printf("Eroare la alocare!");
            exit(1);
        }
    }
    for (int i = 0; i < V; i++)
    {
        (*arr + i)->name = (char *)malloc(256 * sizeof(char));
    }
}
void inputGraph(int **graph, int E, FILE *ft)
{
    for (int i = 0; i < E; i++)
    {
        int A = 0, B = 0, weight = 0;
        fscanf(ft, "%d %d %d", &A, &B, &weight);
        graph[A - 1][B - 1] = weight;
    }
}
void bellmanFordMin(int **graph, int V, int src, int dest, wallet *wallets)
{
    int *dist = malloc(V * sizeof(int));
    int *parent = malloc(V * sizeof(int));
    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;
        parent[i] = -1;
    }
    dist[src] = 0;
    for (int k = 0; k < V - 1; k++)
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (graph[i][j] != 0 && dist[i] != INT_MAX && dist[i] + graph[i][j] < dist[j])
                {
                    dist[j] = dist[i] + graph[i][j];
                    parent[j] = i;
                }
            }
        }
    }
    printf("Drumul minim de la %s la %s este: ", wallets[src].name, wallets[dest].name);
    printPath(parent, dest, wallets, V);
    printf("\n");
    free(dist);
    free(parent);
}
void bellmanFordMax(int **graph, int V, int src, int dest, wallet *wallets)
{
    int *dist = malloc(V * sizeof(int));
    int *parent = malloc(V * sizeof(int));
    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MIN;
        parent[i] = -1;
    }
    dist[src] = 0;
    for (int k = 0; k < V - 1; k++)
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (graph[i][j] != 0 && dist[i] != INT_MIN && dist[i] + graph[i][j] > dist[j])
                {
                    dist[j] = dist[i] + graph[i][j];
                    parent[j] = i;
                }
            }
        }
    }
    printf("Drumul maxim de la %s la %s este: ", wallets[src].name, wallets[dest].name);
    printPath(parent, dest, wallets, V);
    printf("\n");
    free(dist);
    free(parent);
}
queue *createQueue(){
    queue *q = (queue *)malloc(sizeof(queue));
    q->front = q->rear = NULL;
    return q;
    
}
int isEmpty(queue *q)
{
    return q->front == NULL;
}
void enqueue(queue *q, int data){
    queueNode *newNode = (queueNode *)malloc(sizeof(queueNode));
    newNode->data = data;
    newNode->next = NULL;
    if(isEmpty(q)){
       q->front = q->rear = newNode;
    }else{
        q->rear->next = newNode;
        q->rear = newNode;
    }
}
int dequeue(queue *q){
    if(isEmpty(q)){
        printf("Coada este goala!\n");
        return INT_MIN;
    }
    queueNode *temp =   q->front;
    int data = temp->data;
    q->front = (queueNode *)q->front->next;
    if(q->front == NULL){
        q->rear = NULL;
    }
    free(temp);
    return data;
}
void printPath(int parent[], int dest, wallet *wallets, int V)
{
    queue *q = createQueue();
    int path[V], idx = 0;
    while (parent[dest] != -1) {
        path[idx++] = dest;
        dest = parent[dest];
    }
    path[idx++] = dest; 
    for (int i = idx - 1; i >= 0; i--) {
        enqueue(q, path[i]); 
    }
    while (!isEmpty(q)) {
        int node = dequeue(q); 
        printf("%s ", wallets[node].name); 
    }
    free(q); 
}
int timeCalculation(int time_confirmation, int option)
{
    if (option == 1)
    {
        return time_confirmation;
    }
    else
    {
        return time_confirmation;
    }
}
void transaction(wallet *wallets, int **graph, int sender_id, int receiver_id, float amount, int option)
{
    srand(time(0));
    float tax;
    char *time_confirmation;
    int timeStp;
    time_t now = time(0);
    struct tm *currentTime = localtime(&now);
    printf("Tranzactia a fost initializata la ora: %02d:%02d:%02d\n", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
    if (option == 1)
    {
        timeStp = rand() % 86400 + 3600;
        tax = 0.2;
    }
    else
    {
        timeStp = rand() % 5;
        tax = amount * 0.1;
    }

    if (wallets[sender_id - 1].balance > amount)
    {
        wallets[sender_id - 1].balance -= amount + tax;
    }
    else
    {
        printf("Nu ai suficenti bani!\n");
        printf("Tranzactie esuata!\n");
        return;
    }
    printf("Balanta contului (%s) dupa tranzactie: %.2f\n", wallets[sender_id - 1].name, wallets[sender_id - 1].balance);
    printf("Asteptam timpul de confirmare care este de: %d secunde\n", timeStp);
    sleep(timeStp);
    printf("Timpul de confirmare sa incheiat.\n");
    wallets[receiver_id - 1].balance += amount;
    printf("Balanta contului (%s) dupa tranzactie: %.2f USDT\n", wallets[receiver_id - 1].name, wallets[receiver_id - 1].balance);
    printf("Tranzactia a fost efectuata cu succes!\n");
    now = time(0);
    currentTime = localtime(&now);
    printf("Tranzactia a fost finalizata la ora: %02d:%02d:%02d\n", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
    printf("Comisionul platit: $%.2f\n", tax);
    printf("Ponderea tranzactiei este de %d secunde\n", timeCalculation(timeStp, option));
    printf("Suma transferata de la wallet-ul %s la wallet-ul %s: %.2f USDT\n", wallets[sender_id - 1].name, wallets[receiver_id - 1].name, amount);
}
void printWalletsBalance(wallet *wallets, int V)
{
    for (int i = 0; i < V; i++)
    {
        printf("Wallet nr. %d , adresa: %s, balanta contului: %.2f USDT\n", wallets[i].id, wallets[i].name, wallets[i].balance);
    }
}

