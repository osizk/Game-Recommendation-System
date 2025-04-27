#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin_interface.h"
#include "game.h"

void adminMenu() {
    int choice;
    char name[99];
    char genre[99];
    float price;
    char newGenre[99];
    float newPrice;

    
    while (1) {
        printgamelist();

        printf("\n--- Admin Menu ---\n");
        printf("1. Add Game\n");
        printf("2. Edit Game\n");
        printf("3. Delete Game\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        while (getchar() != '\n');

        switch (choice) {
            case 1:
                printf("Enter game name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                printf("Enter game genre: ");
                fgets(genre, sizeof(genre), stdin);
                genre[strcspn(genre, "\n")] = 0;

                printf("Enter game price: ");
                scanf("%f", &price);
                while (getchar() != '\n');

                addGame(name, genre, price);
                break;
            case 2:
                printf("Enter the name of the game to edit: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                if (findGame(name) != NULL) {
                    printf("Enter new genre: ");
                    fgets(newGenre, sizeof(newGenre), stdin);
                    newGenre[strcspn(newGenre, "\n")] = 0;

                    printf("Enter new price: ");
                    scanf("%f", &newPrice);
                    while (getchar() != '\n');

                    editGame(name, newGenre, newPrice);
                } else {
                    printf("Game '%s' not found. Cannot edit.\n", name);
                }
                break;
            case 3:
                printf("Enter the name of the game to delete: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                deleteGame(name);
                break;
            case 4:
                printf("Game list is shown above.\n");
                break;
            case 5:
                printf("Returning to main menu...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
