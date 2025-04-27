#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
}

void loadgame_hash(char filename[])
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        char name[99];
        char genre[99];
        float price;
        
        if (sscanf(line, "%[^,],%[^,],%f",name, genre, &price) == 3) {
            addGame(name, genre, price);
        }
    }

    fclose(file);
}

void printHashTable() {
    printf("Game Hash Table\n");
    printf("-------------------------\n");
    
    printf("Hash Table:\n");
    for(int i=0;i<tablesize;++i){
        game *temp = Game_hash[i];
        printf("[%d]",i);
        while (temp!=NULL)
        {
            printf(" -> %s, %s, %.2f",temp->name,temp->genre,temp->price);
            temp=temp->next;
        }
        printf("\n");
    }
}
