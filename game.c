#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <time.h>
#include "game.h"
#include "logging_system.h"

game *hashIndex[tablesize] = {NULL};
UserPurchase* currentUserPurchase = NULL;
Cart cart;

unsigned int hash(char name[]) {
    unsigned int hash_value = 0;
    while (*name) {
        hash_value = (hash_value * 21) + tolower(*name);
        name++;
    }
    return hash_value % tablesize;
}

void saveGameListToCSV(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file '%s' for writing!\n", filename);
        return;
    }

    fprintf(file, "Name,Genre,Price\n");

    for (int i = 0; i < tablesize; ++i) {
        game *temp = hashIndex[i];
        while (temp != NULL) {
            fprintf(file, "%s,%s,%.2f\n", temp->name, temp->genre, temp->price);
            temp = temp->next;
        }
    }

    fclose(file);
    printf("Game list saved to '%s'.\n", filename);
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
    newNode->relationcount = 0;

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

    saveGameListToCSV("games.csv");
}


void loadGame(char filename[]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file '%s'!\n", filename);
        return;
    }
    char line[256];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char name[99];
        char genre[99];
        float price;

        if (sscanf(line, "%[^,],%[^,],%f", name, genre, &price) == 3) {
            unsigned int index = hash(name);
            game *newNode = malloc(sizeof(game));

            if (newNode == NULL) {
                printf("Memory allocation failed during loading.\n");
                continue;
            }
            strcpy(newNode->name, name);
            strcpy(newNode->genre, genre);
            newNode->price = price;
            newNode->next = NULL;
            newNode->relationcount = 0;

            if (hashIndex[index] == NULL) {
                hashIndex[index] = newNode;
            } else {
                game *current = hashIndex[index];
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = newNode;
            }
        } else {
            printf("Skipping malformed line in %s: %s", filename, line);
        }
    }

    fclose(file);
    printf("Game data loaded from '%s'.\n", filename);
}

void loadRelations(char filename[]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening relations file");
        printf("Warning: Could not load game relations from '%s'.\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char game1_name[100], game2_name[100];
        if (sscanf(line, "%[^,],%[^\n]", game1_name, game2_name) == 2) {
            addRelation(game1_name, game2_name);
        } else {
             printf("Skipping malformed relation line in %s: %s", filename, line);
        }
    }
    fclose(file);
     printf("Game relations loaded from '%s'.\n", filename);
}

void printgamelist() {
    printf("\n+--------------------------------------------------+\n");
    printf("|                   Game List                      |\n");
    printf("+--------------------------------------------------+\n");

    printf("| %-28s | %-18s | %-8s |\n", "Name", "Genre", "Price");
    printf("+------------------------------+--------------------+----------+\n");

    int game_count = 0;
    for (int i = 0; i < tablesize; ++i) {
        game *temp = hashIndex[i];
        while (temp != NULL) {
            printf("| %-28s | %-18s | %-8.2f |\n", temp->name, temp->genre, temp->price);
            temp = temp->next;
            game_count++;
        }
    }

    if (game_count == 0) {
        printf("| %-64s |\n", "No games found in the system.");
         printf("+--------------------------------------------------+\n\n");
    } else {
         printf("+------------------------------+--------------------+----------+\n\n");
    }
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
        snprintf(editGame, sizeof(editGame), "Edit %s", name);
        logging_event(editGame,"Admin");

        saveGameListToCSV("games.csv");

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
            snprintf(deleteGame, sizeof(deleteGame), "Delete %s", name);
            logging_event(deleteGame,"Admin");

            saveGameListToCSV("games.csv");

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
        printf("Warning: Game '%s' not found when adding relation!\n", name1);
        return;
    }
    if (game2 == NULL) {
        printf("Warning: Game '%s' not found when adding relation!\n", name2);
        return;
    }
    if (game1 == game2){
        return;
    }
    for (int i=0;i<game1->relationcount;++i) {
        if (game1->related[i] == game2) {
            return;
        }
    }
    if (game1->relationcount < max_relation) {
        game1->related[game1->relationcount] = game2;
        game1->relationcount++;
    }else{
        printf("Warning: Cannot add more relations to '%s' (maximum reached)\n", name1);
    }
}

void enqueue(queue **front,queue **rear, game *game){
    queue *newnode = malloc(sizeof(queue));
    if (newnode == NULL) {
        printf("Memory allocation failed for queue node.\n");
        return;
    }
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
    printf("\nGames related to '%s':\n", name);

    int found_related = 0;
    while (front!=NULL) {
        game *current = dequeue(&front,&rear);

        if (current != Game) {
            printf("- %s (%s, $%.2f)\n", current->name, current->genre, current->price);
            found_related = 1;
        }

        for (int i = 0; i < current->relationcount && i < max_relation; i++) {
            if (current->related[i] != NULL && !current->related[i]->visited) {
                current->related[i]->visited = 1;
                enqueue(&front, &rear, current->related[i]);
            }
        }
    }
    if (!found_related) {
        printf("No direct or indirect related games found.\n");
    }
}

void setCart(){
    CartItem* current = cart.front;
    CartItem* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
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
    CartItem* current = cart.front;
    while (current != NULL) {
        if (compareWithoutspaces(current->game->name, name) == 0) {
            printf("Game '%s' is already in the cart.\n", name);
            return;
        }
        current = current->next;
    }

    CartItem* newItem = malloc(sizeof(CartItem));
    if (newItem == NULL) {
        printf("Memory allocation failed for cart item.\n");
        return;
    }
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
    int found = 0;
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
            free(current);
            found = 1;
            break;
        }
        prev = current;
        current = current->next;
    }
    if (!found) {
         printf("Game '%s' not found in the cart.\n", name);
    }
}

void viewCart(){
    system("cls");
    printf("\n+--------------------------------------------------------------+\n");
    printf("|                     Your Shopping Cart                       |\n");
    printf("+------------------------------+--------------------+----------+\n");

    if(cart.count == 0) {
        printf("|Your cart is empty                                            |\n");
        printf("+--------------------------------------------------------------+\n");
        return;
    }

    printf("| %-28s | %-18s | %-8s |\n", "Game", "Genre", "Price");
    printf("+------------------------------+--------------------+----------+\n");

    CartItem* current = cart.front;
    while(current != NULL) {
        printf("| %-28s | %-18s | %-8.2f |\n", current->game->name,current->game->genre,current->game->price);
        current = current->next;
    }

    printf("+------------------------------+--------------------+----------+\n");
    printf("|Total: $%.2f                                                  |\n", cart.total);
    printf("+--------------------------------------------------------------+\n\n");
}

void checkout(const char *username){
    if(cart.count == 0) {
        printf("Your cart is empty!\n");
        return;
    }
    printf("\n---- Cart Summary ----\n");
    viewCart();
    printf("\nConfirm purchase (y/n): ");
    char confirm_str[10];
    fgets(confirm_str, sizeof(confirm_str), stdin);
    char confirm = tolower(confirm_str[0]);

    if(confirm == 'y') {
        printf("\nPurchase completed! Thank you!\n");
        printf("Total charged: $%.2f\n", cart.total);
        recordPurchase(username,&cart);
        saveUserPurchaseHistory(username);

        CartItem* current = cart.front;
        while (current != NULL)
        {
            char activity[200];
            snprintf(activity, sizeof(activity), "Purchased Game: %s ($%.2f)",current->game->name, current->game->price);
            logging_user(activity, username);
            current = current->next;
        }

        setCart();
    } else {
        printf("Purchase cancelled\n");
    }
}

void loadUserPurchaseHistory(const char* username) {
    char filename[150];
    snprintf(filename, sizeof(filename), "UserHistory/%s.csv", username);

    if (currentUserPurchase != NULL) {
        free(currentUserPurchase);
        currentUserPurchase = NULL;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        currentUserPurchase = malloc(sizeof(UserPurchase));
        if (currentUserPurchase == NULL) {
             printf("Memory allocation failed for user purchase history.\n");
             return;
        }
        strcpy(currentUserPurchase->username, username);
        currentUserPurchase->purchaseCount = 0;
        return;
    }

    currentUserPurchase = malloc(sizeof(UserPurchase));
     if (currentUserPurchase == NULL) {
        printf("Memory allocation failed for user purchase history.\n");
        fclose(file);
        return;
    }
    strcpy(currentUserPurchase->username, username);
    currentUserPurchase->purchaseCount = 0;

    char line[256];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && currentUserPurchase->purchaseCount < max_cart) {
        char name[100], genre[100], date[20];
        float price;

        if (sscanf(line, "\"%[^\"]\",\"%[^\"]\",%f,%19s",
                  name, genre, &price, date) == 4) {

            game* purchasedGame = findGame(name);
            if (purchasedGame) {
                currentUserPurchase->purchasedGames[currentUserPurchase->purchaseCount] = purchasedGame;
                currentUserPurchase->purchaseCount++;
            } else {
                 printf("Warning: Purchased game '%s' not found in current game list.\n", name);
            }
        } else {
             printf("Warning: Skipping malformed purchase history line for user %s: %s", username, line);
        }
    }

    fclose(file);
}

void saveUserPurchaseHistory(const char* username) {
    if (!currentUserPurchase || currentUserPurchase->purchaseCount == 0) {
         char filename[100];
         snprintf(filename, sizeof(filename), "UserHistory/%s.csv", username);
         remove(filename);
         return;
    }

    char filename[100];
    snprintf(filename, sizeof(filename), "UserHistory/%s.csv", username);

    FILE* newFile = fopen("temp_history.csv", "w");

    if (!newFile) {
        printf("Error creating temporary history file\n");
        return;
    }

    fprintf(newFile, "\"Game\",\"Genre\",\"Price\",\"PurchaseDate\"\n");

    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char date_str[20];
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", tm_info);

    for (int i = 0; i < currentUserPurchase->purchaseCount; i++) {
        if (currentUserPurchase->purchasedGames[i] != NULL) {
            fprintf(newFile, "\"%s\",\"%s\",%.2f,%s\n",
                    currentUserPurchase->purchasedGames[i]->name,
                    currentUserPurchase->purchasedGames[i]->genre,
                    currentUserPurchase->purchasedGames[i]->price,
                    date_str);
        }
    }

    fclose(newFile);

    remove(filename);
    rename("temp_history.csv", filename);
}


void recordPurchase(const char* username, Cart* cart) {
    if (!currentUserPurchase) {
        currentUserPurchase = malloc(sizeof(UserPurchase));
        strcpy(currentUserPurchase->username, username);
        currentUserPurchase->purchaseCount = 0;
    }
    loadUserPurchaseHistory(username);
    CartItem* item = cart->front;
    while (item != NULL && currentUserPurchase->purchaseCount < max_cart) {
        int alreadyPurchased = 0;
        for (int i = 0; i < currentUserPurchase->purchaseCount; i++) {
            if (currentUserPurchase->purchasedGames[i] != NULL &&
                compareWithoutspaces(currentUserPurchase->purchasedGames[i]->name, item->game->name) == 0) {
                alreadyPurchased = 1;
                break;
            }
        }

        if (!alreadyPurchased) {
            currentUserPurchase->purchasedGames[currentUserPurchase->purchaseCount] = item->game;
            currentUserPurchase->purchaseCount++;
        }

        item = item->next;
    }
     if (cart->count > 0 && currentUserPurchase->purchaseCount >= max_cart) {
        printf("Warning: Purchase history is full. Could not record all purchased games.\n");
    }
}

void recommendBasedOnHistory(const char* username) {
    system("cls");
    if (currentUserPurchase == NULL || strcmp(currentUserPurchase->username, username) != 0) {
        loadUserPurchaseHistory(username);
    }
    if (currentUserPurchase->purchaseCount == 0) {
        printf("No purchase history found for %s. Try browsing our catalog!\n", username);
        return;
    }
    setVisited();
    
    typedef struct {
        char genre[99];
        int count;
    } GenreFrequency;
    
    GenreFrequency genreFreq[20] = {0};
    int genreCount = 0;
    
    for (int i = 0; i < currentUserPurchase->purchaseCount; i++) {
        // find genre frequency
        int found = 0;
        for (int j = 0; j < genreCount; j++) {
            if (strcmp(genreFreq[j].genre, currentUserPurchase->purchasedGames[i]->genre) == 0) {
                genreFreq[j].count++;
                found = 1;
                break;
            }
        }
        if (!found && genreCount < 20) {
            strcpy(genreFreq[genreCount].genre, currentUserPurchase->purchasedGames[i]->genre);
            genreFreq[genreCount].count = 1;
            genreCount++;
        }
    }
    
    // Sort genres by frequency (bubble sort)
    for (int i = 0; i < genreCount - 1; i++) {
        for (int j = 0; j < genreCount - i - 1; j++) {
            if (genreFreq[j].count < genreFreq[j+1].count) {
                GenreFrequency temp = genreFreq[j];
                genreFreq[j] = genreFreq[j+1];
                genreFreq[j+1] = temp;
            }
        }
    }
    
    // Find recommended games (BFS from all purchased games)
    queue* front = NULL;
    queue* rear = NULL;
    
    // Mark all purchased games as visited (distance 0)
    for (int i = 0; i < currentUserPurchase->purchaseCount; i++) {
        game* purchased = currentUserPurchase->purchasedGames[i];
        purchased->visited = 1;
        enqueue(&front, &rear, purchased);
    }
    
    // Track recommendations with their distance and genre match score
    typedef struct {
        game* game;
        int distance;
        int genreScore;
    } Recommendation;
    
    Recommendation recommendations[max_relation * 2];
    int recCount = 0;
    
    //BFS
    while (front != NULL) {
        game* current = dequeue(&front, &rear);
        int currentDistance = current->visited - 1;//game that owned distance will be 0
        
        if (currentDistance > 0) {
            int genreScore = 0;
            for (int i = 0; i < genreCount; i++) {
                if (strcmp(current->genre, genreFreq[i].genre) == 0) {
                    genreScore = genreCount - i; // Higher score for more frequent genres
                    break;
                }
            }
            
            recommendations[recCount].game = current;
            recommendations[recCount].distance = currentDistance;
            recommendations[recCount].genreScore = genreScore;
            recCount++;
        }
        for (int i = 0; i < current->relationcount && i < max_relation; i++) {
            if (current->related[i] != NULL && !current->related[i]->visited) {
                current->related[i]->visited = current->visited + 1;
                enqueue(&front, &rear, current->related[i]);
            }
        }
    }
    
    // Sort by genre score (primary) and distance (secondary)
    for (int i = 0; i < recCount - 1; i++) {
        for (int j = 0; j < recCount - i - 1; j++) {
            if (recommendations[j].genreScore < recommendations[j+1].genreScore || 
                (recommendations[j].genreScore == recommendations[j+1].genreScore && 
                 recommendations[j].distance > recommendations[j+1].distance)) {
                Recommendation temp = recommendations[j];
                recommendations[j] = recommendations[j+1];
                recommendations[j+1] = temp;
            }
        }
    }
    
    // Display
    printf("\n--- Game Recommendations for YOU ---\n");
    
    printf("\n%-5s %-30s %-20s %-10s %s\n", "Rank", "Name", "Genre", "Price", "Why Recommended");
    printf("----------------------------------------------------------------\n");
    
    int maxToShow = 5;
    for (int i = 0; i < recCount && i < maxToShow; i++) {
        char reason[100];
        if (recommendations[i].genreScore > 0) {
            snprintf(reason, sizeof(reason), "Matches your preferred genres");
        } else {
            snprintf(reason, sizeof(reason), "Related to your purchases (%d degree%s)", 
                    recommendations[i].distance,
                    recommendations[i].distance == 1 ? "" : "s");
        }
        
        printf("%-5d %-30s %-20s $%-9.2f %s\n", 
               i+1, 
               recommendations[i].game->name, 
               recommendations[i].game->genre, 
               recommendations[i].game->price,
               reason);
    }
    
    if (recCount == 0) {
        printf("No recommendations found. Try browsing our new releases!\n");
    }
    
    printf("----------------------------------------------------------------\n");
    
    char logMessage[200];
    snprintf(logMessage, sizeof(logMessage), "Viewed personalized recommendations");
    logging_user(logMessage, username);
}