#ifndef GAME_H
#define GAME_H

#define tablesize 20
#define max_relation 100

typedef struct game
{
    char name[99];
    char genre[99];
    float price;
    struct game *next;
    struct game *related[max_relation];
    int relationcount;
    int visited;
}game;

typedef struct queue {
    struct game *game;
    struct queue *next;
}queue;

unsigned int hash(char name[]);
void addGame(char name[], char genre[], float price);
void loadgame_hash(char filename[]);
void printgamelist();
game *findGame(char name[]);
void editGame(char name[], char newGenre[], float newPrice);
void deleteGame(char name[]);
void addRelation(char name1[],char name2[]);
void enqueue(queue **front,queue **rear, game *game);
game* dequeue(queue **front,queue **rear);
void setVisited();
void BFS(char name[]);
#endif
