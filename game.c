#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include "game.h"

game *Game_hash[tablesize] = {NULL};

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

    if (Game_hash[index] == NULL) {
        Game_hash[index] = newNode;
    } else {
        game *current = Game_hash[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    printf("Game '%s' added successfully.\n", name);
}


void loadgame_hash(char filename[]) {
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


void printgamelist() {
    printf("\n--- Game List ---\n");
    printf("%-30s %-20s %-10s\n", "Name", "Genre", "Price");
    printf("----------------------------------------------------------\n");

    int game_count = 0;
    for (int i = 0; i < tablesize; ++i) {
        game *temp = Game_hash[i];
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

game *findGame(char name[]) {
    unsigned int index = hash(name);
    game *current = Game_hash[index];

    while (current != NULL) {
        if (strcasecmp(current->name, name) == 0) {
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
    } else {
        printf("Game '%s' not found.\n", name);
    }
}
void deleteGame(char name[]) {
    unsigned int index = hash(name);
    game *current = Game_hash[index];
    game *prev = NULL;

    while (current != NULL) {

        if (strcasecmp(current->name, name) == 0) {

            if (prev == NULL) {

                Game_hash[index] = current->next;
            } else {

                prev->next = current->next;
            }
            free(current);
            printf("Game '%s' deleted successfully.\n", name);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Game '%s' not found.\n", name);
}