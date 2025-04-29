#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin_interface.h"
#include "game.h"
#include "logging_system.h"

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
        printf("4. Watch Logging\n");
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
                
                char addGame[100];
                snprintf(addGame, sizeof(addGame), "Add %s  ", name);
                logging_event(addGame,"Admin");

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

                    char editGame[100];
                    snprintf(editGame, sizeof(editGame), "Edit %s  ", name);
                    logging_event(editGame,"Admin");
                } else {
                    printf("Game '%s' not found. Cannot edit.\n", name);
                }
                break;
            case 3:
                printf("Enter the name of the game to delete: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                deleteGame(name);

                char deleteGame[100];
                snprintf(deleteGame, sizeof(deleteGame), "Delete %s  ", name);
                logging_event(deleteGame,"Admin");
                break;
            case 4:

                char date[11];
                printf("Select Date (format: YYYY-MM-DD): ");
                scanf("%10s", date);
                display_logging(date);
                break;
            case 5:
                printf("Game list is shown above.\n");
                break;
            case 6:
                printf("Returning to main menu...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
