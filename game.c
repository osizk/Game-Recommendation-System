#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include "game.h"
#include "logging_system.h"

game *hashIndex[tablesize] = {NULL};
Cart cart;
unsigned int hash(char name[]) {
    unsigned int hash_value = 0;
    while (*name) {
        hash_value = (hash_value * 21) + tolower(*name);
        name++;
    }
    return hash_value % tablesize;
}

void addGame(char name[], char genre[], float price) {
    unsigned int index = hash(name);
    game *newNode = malloc(sizeof(game));

    if (newNode == NULL) {
        printf("Memory allocation failed for new game node.\n");
        return;
    }
    strcpy(newNode->name, name);
    strcpy(newNode->genre, genre);
    newNode->price = price;
    newNode->next = NULL;

    if (hashIndex[index] == NULL) {
        hashIndex[index] = newNode;
    } else {
        game *current = hashIndex[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    printf("Game '%s' added successfully.\n", name);
}


void loadGame(char filename[]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file '%s'!\n", filename);
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char name[99];
        char genre[99];
        float price;

        if (sscanf(line, "%[^,],%[^,],%f", name, genre, &price) == 3) {
            addGame(name, genre, price);
        } else {
            printf("Skipping malformed line in %s: %s", filename, line);
        }
    }

    fclose(file);
}

void loadRelations(char filename[]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening relations file");
        return;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char game1[100], game2[100];
        if (sscanf(line, "%[^,],%[^\n]", game1, game2) == 2) {
            addRelation(game1, game2);
            addRelation(game2, game1);
        }
    }
    fclose(file);
}

void printgamelist() {
    printf("\n--- Game List ---\n");
    printf("%-30s %-20s %-10s\n", "Name", "Genre", "Price");
    printf("----------------------------------------------------------\n");

    int game_count = 0;
    for (int i = 0; i < tablesize; ++i) {
        game *temp = hashIndex[i];
        while (temp != NULL) {

            printf("%-30s %-20s %-10.2f\n", temp->name, temp->genre, temp->price);
            temp = temp->next;
            game_count++;
        }
    }

    if (game_count == 0) {
        printf("No games found in the system.\n");
    }
    printf("----------------------------------------------------------\n\n");
}

void removeSpaces(char str[])
{
    int index = 0;
    for (int i = 0; str[i]; i++){
        if (str[i] != ' '){
            str[index] = str[i];
            index++;
        }
    }
    str[index] = '\0';
}

int compareWithoutspaces(char name1[], char name2[]) {
    char temp1[100];
    char temp2[100];
    strcpy(temp1, name1);
    strcpy(temp2, name2);
    removeSpaces(temp1);
    removeSpaces(temp2);
    return strcasecmp(temp1, temp2);
}

game *findGame(char name[]) {
    unsigned int index = hash(name);
    game *current = hashIndex[index];

    while (current != NULL) {
        if (compareWithoutspaces(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void editGame(char name[], char newGenre[], float newPrice) {
    game *gameToEdit = findGame(name);

    if (gameToEdit != NULL) {
        strcpy(gameToEdit->genre, newGenre);
        gameToEdit->price = newPrice;
        printf("Game '%s' updated successfully.\n", name);
        char editGame[100];
        snprintf(editGame, sizeof(editGame), "Edit %s  ", name);
        logging_event(editGame,"Admin");
    } else {
        printf("Game '%s' not found.\n", name);
    }
}
void deleteGame(char name[]) {
    unsigned int index = hash(name);
    game *current = hashIndex[index];
    game *prev = NULL;

    while (current != NULL) {

        if (!strcasecmp(current->name, name)) {

            if (prev == NULL) {

                hashIndex[index] = current->next;
            } else {

                prev->next = current->next;
            }
            free(current);
            printf("Game '%s' deleted successfully.\n", name);
            char deleteGame[100];
            snprintf(deleteGame, sizeof(deleteGame), "Delete %s  ", name);
            logging_event(deleteGame,"Admin");

            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Game '%s' not found.\n", name);
}

void addRelation(char name1[],char name2[]) {
    game *game1 = findGame(name1);
    game *game2 = findGame(name2);
    if (game1 == NULL) {
        printf("Game '%s' not found!\n", name1);
        return;
    }
    if (game2 == NULL) {
        printf("Game '%s' not found!\n", name2);
        return;
    }
    if (game1 == game2){
        return;
    }
    for (int i=0;i<game1->relationcount;++i) {
        if (game1->related[i] == game2) {
            printf("Relation already exists between '%s' and '%s'\n", name1, name2);
            return;
        }
    }
    if (game1->relationcount < max_relation) {
        game1->related[game1->relationcount] = game2;
        game1->relationcount++;
        printf("Added relation: '%s' -> '%s'\n", name1, name2);
    }else{
        printf("Cannot add more relations to '%s' (maximum reached)\n", name1);
    }
}

void enqueue(queue **front,queue **rear, game *game){
    queue *newnode = malloc(sizeof(queue));
    newnode->game = game;
    newnode->next = NULL;
    
    if (*front == NULL) {
        *front = newnode;
        *rear = newnode;
    } else {
        (*rear)->next = newnode;
        *rear = newnode;
    }
}

game* dequeue(queue **front, queue **rear) {
    if (*front == NULL) {
        return NULL;
    }
    queue *temp = *front;
    game *result = temp->game;
    
    *front = (*front)->next;
    if (*front == NULL) {
        *rear = NULL;
    }
    free(temp);
    return result;
}

void setVisited(){
    for (int i = 0; i < tablesize; i++) {
        game *current = hashIndex[i];
        while (current != NULL) {
            current->visited = 0;
            current = current->next;
        }
    }
}

void BFS(char name[]){
    game *Game = findGame(name);
    if (Game == NULL) {
        printf("Game '%s' not found!\n", name);
        return;
    }
    setVisited();
    queue *front = NULL;
    queue *rear = NULL;

    Game->visited = 1;
    enqueue(&front,&rear,Game);
    printf("Games related to '%s':\n", name);

    while (front!=NULL) {
        game *current = dequeue(&front,&rear);
        
        if (current != Game) {
            printf("- %s (%s, $%.2f)\n", current->name, current->genre, current->price);
        }

        for (int i = 0; i < current->relationcount && i < max_relation; i++) {
            if (current->related[i] != NULL && !current->related[i]->visited) {
                current->related[i]->visited = 1;
                enqueue(&front, &rear, current->related[i]);
            }
        }
    }
}

void setCart(){
    cart.front = NULL;
    cart.rear = NULL;
    cart.count = 0;
    cart.total = 0;
}

void addtoCart(char name[]){
    if(cart.count >= max_cart) {
        printf("Cart is full! Maximum %d items allowed.\n", max_cart);
        return;
    }
    game *found = findGame(name);
    if(found == NULL) {
        printf("Game '%s' not found!\n", name);
        return;
    }
    CartItem* newItem = malloc(sizeof(CartItem));
    newItem->game = found;
    newItem->next = NULL;

    if(cart.front == NULL) {
        cart.front = newItem;
        cart.rear = newItem;
    } else {
        cart.rear->next = newItem;
        cart.rear = newItem;
    }
    cart.count++;
    cart.total+=found->price;

    printf("Added to cart: %s ($%.2f)\n", found->name, found->price);
    printf("Cart now has %d items (Total: $%.2f)\n", cart.count, cart.total);
}

void deletefromCart(char name[]){
    if(cart.count == 0) {
        printf("Cart is empty!\n");
        return;
    }
    CartItem *current=cart.front;
    CartItem *prev=NULL;
    while(current!=NULL){
        if(!compareWithoutspaces(current->game->name,name)){
            if(prev==NULL){
                cart.front=current->next;
            }else{
                prev->next=current->next;
            }
            if(current == cart.rear) {
                cart.rear = prev;
            }
            cart.count--;
            cart.total -= current->game->price;
            printf("Removed from cart: %s\n", current->game->name);
            printf("Cart now has %d items (Total: $%.2f)\n", cart.count, cart.total);
        }
        prev = current;
        current = current->next;
    }
}

void viewCart(){
    printf("\n--- Your Shopping Cart ---\n");
    if(cart.count == 0) {
        printf("Your cart is empty\n");
        return;
    }
    
    printf("%-30s %-20s %-10s\n","Game","Genre","Price");
    printf("-----------------------------------------------------------\n");
    
    CartItem* current = cart.front;
    while(current != NULL) {
        printf("%-30s %-20s $%-9.2f\n", current->game->name,current->game->genre,current->game->price);
        current = current->next;
    }
    
    printf("\nTotal: $%.2f\n", cart.total);
    printf("-----------------------------------------------------------\n");
}

void checkout(){
    if(cart.count == 0) {
        printf("Your cart is empty!\n");
        return;
    }
    printf("\n---- Cart Summary ----\n");
    viewCart();
    printf("\nConfirm purchase (y/n): ");
    char confirm;
    scanf("%c", &confirm);    
    if(tolower(confirm) == 'y') {
        printf("\nPurchase completed! Thank you!\n");
        printf("Total charged: $%.2f\n", cart.total);      
        setCart();
    } else {
        printf("Purchase cancelled\n");
    }
}