#ifndef GAME_H
#define GAME_H

#define tablesize 20
#define max_relation 100
#define max_cart 100

typedef struct game{
    char name[99];
    char genre[99];
    float price;
    struct game *next;
    struct game *related[max_relation];
    int relationcount;
    int visited;
}game;

typedef struct queue{
    struct game *game;
    struct queue *next;
}queue;

typedef struct CartItem {
    game* game;
    struct CartItem* next;
} CartItem;

typedef struct {
    CartItem* front;
    CartItem* rear;
    int count;
    float total;
} Cart;

typedef struct UserPurchase {
    char username[100];
    game* purchasedGames[max_cart];
    int purchaseCount;
} UserPurchase;

void saveGameToCSV(char filename[]);
unsigned int hash(char name[]);
void addGame(char name[], char genre[], float price);
void loadGame(char filename[]);
void loadRelations(char filename[]);
void printgamelist();
void removeSpaces(char str[]);
int compareWithoutspaces(char name1[], char name2[]);
game *findGame(char name[]);
void editGame(char name[], char newGenre[], float newPrice);
void deleteGame(char name[]);
void addRelation(char name1[], char name2[], int writeToFile);
void enqueue(queue **front,queue **rear, game *game);
game* dequeue(queue **front,queue **rear);
void setVisited();
void BFS(char name[]); //not use just for test
void setCart();
void addtoCart(char name[]);
void deletefromCart(char name[]);
void viewCart();
void checkout(const char *username);
void loadUserPurchaseHistory(const char* username);
void saveUserPurchaseHistory(const char* username);
void recordPurchase(const char* username, Cart* cart);
void recommendBasedOnHistory(const char* username);
void printPurchaseHistory(const char* username);
#endif