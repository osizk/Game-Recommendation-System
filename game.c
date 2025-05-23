#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <time.h>
#include "game.h"
#include "logging_system.h"

#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

game *hashIndex[tablesize] = {NULL};
UserPurchase* currentUserPurchase = NULL;
Cart cart;

//change string to index for hash table
unsigned int hash(char name[]) {
    unsigned int hash_value = 0;
    removeSpaces(name);//remove space before calculate index
    while (*name) {
        hash_value = (hash_value * 21) + tolower(*name);
        name++;
    }
    return hash_value % tablesize;
}
// function to save the current game list from the hash table
void saveGameToCSV(char filename[]) {
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

// function add game to hash table
void addGame(char name[], char genre[], float price) {
    unsigned int index = hash(name);
    game *newNode = malloc(sizeof(game));

    if (newNode == NULL) {
        printf("Failed create new game node.\n");
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
        // traverse to the end of the linked list
        game *current = hashIndex[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode; // add the new node at the end of the list
    }
    printf("Game '%s' added successfully.\n", name);

    saveGameToCSV("games.csv"); //call saveGameTOCSV to write game to games.csv file
}

//load game to hash table
void loadGame(char filename[]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file '%s'!\n", filename);
        return;
    }
    char line[256];
    fgets(line, sizeof(line), file);//skip first line

    //Read each line
    while (fgets(line, sizeof(line), file)) {
        char name[99];
        char genre[99];
        float price;
        //hash
        if (sscanf(line, "%[^,],%[^,],%f", name, genre, &price) == 3) {
            unsigned int index = hash(name);
            game *newNode = malloc(sizeof(game));

            if (newNode == NULL) {
                printf("Failed loading.\n");
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
        }
    }

    fclose(file);
    printf("Game data loaded from '%s'.\n", filename);
}

//load relation to graph
void loadRelations(char filename[]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf(" Warning: Could not load game relations from '%s'.\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char game1_name[100], game2_name[100];
        if (sscanf(line, "%[^,],%[^\n]", game1_name, game2_name) == 2) {
            addRelation(game1_name, game2_name,0);
            addRelation(game2_name, game1_name,0);
        }
    }
    fclose(file);
}

// function to print the list games in the hash table.
void printgamelist() {
    printf("\n+--------------------------------------------------------------+\n");
    printf("|                         Game List                            |\n");
    printf("+------------------------------+--------------------+----------+\n");

    printf("| %-28s | %-18s | %-8s |\n", "Name", "Genre", "Price");
    printf("+------------------------------+--------------------+----------+\n");

    int count = 0;
    for (int i = 0; i < tablesize; ++i) {
        game *temp = hashIndex[i];
        while (temp != NULL) {
            printf("| %-28s | %-18s | %-8.2f |\n", temp->name, temp->genre, temp->price);
            temp = temp->next;
            count++;
        }
    }

    if (count == 0) {
        printf("| %-64s |\n", "No games found in the system.");
         printf("+--------------------------------------------------+\n\n");
    } else {
         printf("+------------------------------+--------------------+----------+\n\n");
    }
}

//remove space from string
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

//compare 2 string without space and without sensitive case
int compareWithoutspaces(char name1[], char name2[]) {
    char temp1[100];
    char temp2[100];
    strcpy(temp1, name1);
    strcpy(temp2, name2);
    removeSpaces(temp1);
    removeSpaces(temp2);
    return strcasecmp(temp1, temp2);
}

//return game that same name as string that sent to this function
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

// fuction to edit game as genre and price
void editGame(char name[], char newGenre[], float newPrice) {
    game *gameToEdit = findGame(name); // find the game by name

    if (gameToEdit != NULL) {
        strcpy(gameToEdit->genre, newGenre);
        gameToEdit->price = newPrice;
        printf("Game '%s' updated successfully.\n", name);
        char editGame[100];
        snprintf(editGame, sizeof(editGame), "Edit %s", name);
        logging_event(editGame,"Admin");

        saveGameToCSV("games.csv"); //call fuction saveGameTOCSV to save 

    } else {
        printf("Game '%s' not found.\n", name);
    }
}

// fuction to delete game in hash table
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

            saveGameToCSV("games.csv"); //call fuction saveGameTOCSV to save 

            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Game '%s' not found.\n", name);
}

// Add a relation between two games if not already related
void addRelation(char name1[], char name2[], int writeToFile) {
    name1[strcspn(name1, "\n")] = 0;
    name2[strcspn(name2, "\n")] = 0;

    game *game1 = findGame(name1);
    game *game2 = findGame(name2);

    //when one game don't have in games.csv
    if (!game1 || !game2) {
        printf("game1 = %s game2 = %d\n",game1,game2);
        printf(" One or both games not found.\n");
        return;
    }
    //when add same game name
    if (game1 == game2) {
        printf(" Cannot relate a game to itself.\n");
        return;
    }

    // Check for existing relation
    for (int i = 0; i < game1->relationcount; ++i) {
        if (game1->related[i] == game2) {
            if (writeToFile) {
                printf("Relation already exists: %s <-> %s\n", name1, name2);
            }
            return;
        }
    }


    // Add relation if under limit
    if (game1->relationcount < max_relation) {
        game1->related[game1->relationcount++] = game2;

        // Save to CSV and log
        if (writeToFile) {
            FILE *fp = fopen("relations.csv", "a");
            if (fp != NULL) {
                fprintf(fp, "%s,%s\n", name1, name2);
                fclose(fp);
            }

            char logMsg[200];
            snprintf(logMsg, sizeof(logMsg), "Relation added: %s <-> %s", name1, name2);
            logging_event(logMsg, "Admin");

            printf("Relation added: %s <-> %s\n", name1, name2);
        }
    } else {
        printf("Max relation reached for '%s'\n", name1);
    }
}



// Add a game to the end of the queue
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

// Remove and return a game from the front of the queue
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

// Reset the visited flag of all games in the hash table
void setVisited(){
    for (int i = 0; i < tablesize; i++) {
        game *current = hashIndex[i];
        while (current != NULL) {
            current->visited = 0;
            current = current->next;
        }
    }
}

//BFS relation of game name(input game name) Just for test
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

// Clear all items from the cart and reset its state
void setCart(){
    CartItem* current = cart.front;
    CartItem* next;
    //clear memory
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    //Reset Cart Stage
    cart.front = NULL;
    cart.rear = NULL;
    cart.count = 0;
    cart.total = 0;
}

// Add a game to the shopping cart if it's not already in and cart is not full
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

     // Check for duplicates
    CartItem* current = cart.front;
    while (current != NULL) {
        if (compareWithoutspaces(current->game->name, name) == 0) {
            printf("Game '%s' is already in the cart.\n", name);
            return;
        }
        current = current->next;
    }

    // Allocate and append new cart item
    CartItem* newItem = malloc(sizeof(CartItem));
    if (newItem == NULL) {
        printf("fail create new item");
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

// Remove a game from the cart by name
void deletefromCart(char name[]){
    if(cart.count == 0) {
        printf("Cart is empty!\n");
        return;
    }
    CartItem *current=cart.front;
    CartItem *prev=NULL;
    int found = 0;

    // Traverse cart to find the game
    while(current!=NULL){
        if(!compareWithoutspaces(current->game->name,name)){
            // Remove node from linked list
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

// Display all games currently in the shopping cart
void viewCart(){
    // Clear the screen for clean output
    CLEAR_SCREEN();

    // Head
    printf("\n+--------------------------------------------------------------+\n");
    printf("|                     Your Shopping Cart                       |\n");
    printf("+------------------------------+--------------------+----------+\n");

    //if cart empty
    if(cart.count == 0) {
        printf("|Your cart is empty                                            |\n");
        printf("+--------------------------------------------------------------+\n");
        return;
    }
    
    //table head 
    printf("| %-28s | %-18s | %-8s |\n", "Game", "Genre", "Price");
    printf("+------------------------------+--------------------+----------+\n");

    // Display each item in cart
    CartItem* current = cart.front;
    while(current != NULL) {
        printf("| %-28s | %-18s | %-8.2f |\n", current->game->name,current->game->genre,current->game->price);
        current = current->next;
    }

    //Bottom with total Price
    printf("+------------------------------+--------------------+----------+\n");
    printf("|Total: $%-54.2f|\n", cart.total);
    printf("+--------------------------------------------------------------+\n\n");
}

//checkout function
void checkout(const char *username){
    //check, Is cart empty
    if(cart.count == 0) {
        printf("Your cart is empty!\n");
        return;
    }
    printf("\n---- Cart Summary ----\n");
    viewCart();//Display cart
    printf("\nConfirm purchase (y/n): ");
    char confirm_str[10];
    fgets(confirm_str, sizeof(confirm_str), stdin);
    char confirm = tolower(confirm_str[0]);
    //check what user type
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
            logging_event(activity, username);
            current = current->next;
        }

        setCart();
    } else {
        printf("Purchase cancelled\n");
    }
}

// Load user's purchase history from games.csv
void loadUserPurchaseHistory(const char* username) {
    char filename[150];
    snprintf(filename, sizeof(filename), "UserHistory/%s.csv", username);

    // Free purchase history
    if (currentUserPurchase != NULL) {
        free(currentUserPurchase);
        currentUserPurchase = NULL;
    }

    FILE* file = fopen(filename, "r");
    //check if the file doesn't exist
    if (!file) {
        currentUserPurchase = malloc(sizeof(UserPurchase));
        if (currentUserPurchase == NULL) {
             printf("currentUserPurchase malloc failed.\n");
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
    fgets(line, sizeof(line), file);//skip first line

    // Read each line
    while (fgets(line, sizeof(line), file) && currentUserPurchase->purchaseCount < max_cart) {
        char name[100], genre[100], date[20];
        float price;

        if (sscanf(line, "\"%[^\"]\",\"%[^\"]\",%f,%19s",name, genre, &price, date) == 4) {

            game* purchasedGame = findGame(name);
            if (purchasedGame) {
                // If the game is found, add it to the user's purchasedGames graph[array]
                currentUserPurchase->purchasedGames[currentUserPurchase->purchaseCount] = purchasedGame;
                currentUserPurchase->purchaseCount++;
            } else {
                 printf("Purchased game '%s' not found in current game list.\n", name);
            }
        }
    }

    fclose(file);
}

// Save current user's purchase history to a CSV file.
void saveUserPurchaseHistory(const char* username) {
    // If there's no history or history is empty, remove history file if it exist
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

    // Get the current date
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

// record games from the current cart into user's purchase history
void recordPurchase(const char* username, Cart* cart) {
    // If currentUserPurchase is NULL or doesn't match the current user, load the history
    if (!currentUserPurchase || strcmp(currentUserPurchase->username, username) != 0) {
        loadUserPurchaseHistory(username);
    }

    CartItem* item = cart->front;
    //loop all item in cart
    while (item != NULL && currentUserPurchase->purchaseCount < max_cart) {
        int alreadyPurchased = 0;
        // If game is already in the user's purchase history
        for (int i = 0; i < currentUserPurchase->purchaseCount; i++) {
            if (currentUserPurchase->purchasedGames[i] != NULL &&
                compareWithoutspaces(currentUserPurchase->purchasedGames[i]->name, item->game->name) == 0) {
                alreadyPurchased = 1;
                break;
            }
        }
        // If the game is not already in the history, add it
        if (!alreadyPurchased) {
            currentUserPurchase->purchasedGames[currentUserPurchase->purchaseCount] = item->game;
            currentUserPurchase->purchaseCount++;
        }

        item = item->next;
    }
    //check if history is full
    if (cart->count > 0 && currentUserPurchase->purchaseCount >= max_cart) {
        printf("Purchase history is full.\n");
    }
}
// Recommend games based on the user's purchase history and game relations
void recommendBasedOnHistory(const char* username) {
    CLEAR_SCREEN();
    // Load user purchase history if it not already loaded for this user
    if (currentUserPurchase == NULL || strcmp(currentUserPurchase->username, username) != 0) {
        loadUserPurchaseHistory(username);
    }
    // If user has no purchase history, show a message and return
    if (currentUserPurchase->purchaseCount == 0) {
        printf("No recommendation now. Try to buy some games.\n");
        return;
    }
    setVisited();// Reset visited flags for BFS
    
    // Struct for genre frequency
    typedef struct {
        char genre[99];
        int count;
    } GenreFrequency;
    
    GenreFrequency genreFreq[20] = {0};
    int genreCount = 0;

    // find genre frequency
    for (int i = 0; i < currentUserPurchase->purchaseCount; i++) {
        int found = 0;
        // Find if the genre is already in the frequency list
        for (int j = 0; j < genreCount; j++) {
            if (strcmp(genreFreq[j].genre, currentUserPurchase->purchasedGames[i]->genre) == 0) {
                genreFreq[j].count++;
                found = 1;
                break;
            }
        }
        // If the genre is not found add it
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
    
    // Struct for recommendations with distance and score
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
    printf("+--------------------------------------------------------------+\n");
    printf("|                     Game Recommendations                     |\n");
    printf("+------------------------------+-------------------+-----------+\n");
    printf("|%-30s|%-20s|%-10s|\n", "Name", "Genre", "Price");
    printf("+------------------------------+-------------------+-----------+\n");
    
    int maxToShow = 5;
    for (int i = 0; i < recCount && i < maxToShow; i++) {
        printf("|%-30s|%-20s|$%-9.2f|\n", recommendations[i].game->name, recommendations[i].game->genre, recommendations[i].game->price);
    }
    
    if (recCount == 0) {
        printf("No recommendations found.\n");
    }
    
    printf("+--------------------------------------------------------------+\n");
    
    char logMessage[200];
    snprintf(logMessage, sizeof(logMessage), "Viewed personalized recommendations");
    logging_event(logMessage, username);
}

//Display user's purchase history
void printPurchaseHistory(const char* username) {
    CLEAR_SCREEN();
    char filename[150];
    snprintf(filename, sizeof(filename), "UserHistory/%s.csv", username);
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("No purchase history found for %s\n", username);
        return;
    }
    printf("+---------------------------------------------------------------------------+\n");
    printf("|                              Purchase History                             |\n");
    printf("+------------------------------+--------------------+----------+------------+\n");
    printf("|%-30s|%-20s|%-10s|%-12s|\n", "Game", "Genre", "Price", "Date");
    printf("+------------------------------+--------------------+----------+------------+\n");

    char line[256];
    fgets(line, sizeof(line), file);//skip first line
    
    float totalSpent = 0.0f;
    int purchaseCount = 0;

    // Read each line
    while (fgets(line, sizeof(line), file)) {
        char name[100], genre[100], date[20];
        float price;
        
        if (sscanf(line, "\"%[^\"]\",\"%[^\"]\",%f,%19s", name, genre, &price, date) == 4) {
            printf("|%-30s|%-20s|$%-9.2f|%-12s|\n", name, genre, price, date);
            totalSpent += price;
            purchaseCount++;
        }
    }

    printf("+---------------------------------------------------------------------------+\n");
    printf("|Total purchases: %-58d|\n", purchaseCount);
    printf("|Total spent: $%-61.2f|\n", totalSpent);
    printf("+---------------------------------------------------------------------------+\n");
    fclose(file);
}