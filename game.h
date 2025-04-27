#ifndef GAME_H
#define GAME_H

#define tablesize 20

typedef struct game
{
    char name[99];
    char genre[99];
    float price;
    struct game *next;
}game;

unsigned int hash(char name[]);
void addGame(char name[], char genre[], float price);
void loadgame_hash(char filename[]);
void printHashTable();
#endif