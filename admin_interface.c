#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "admin_interface.h"
#include "game.h"
#include "logging_system.h"

#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

void adminMenu() {
    int choice;
    char name[99];
    char genre[99];
    float price;
    char newGenre[99];
    float newPrice;
    char name1[99],name2[99];
    int c;

    while (1) {
        CLEAR_SCREEN();
        printgamelist();

        printf("+-----------------------------+\n");
        printf("|         Admin Menu          |\n");
        printf("+-----------------------------+\n");
        printf("| [1] Add Game                |\n");
        printf("| [2] Edit Game               |\n");
        printf("| [3] Delete Game             |\n");
        printf("| [4] Watch Logging           |\n");
        printf("| [5] Add relation            |\n");
        printf("| [6] Back to Main Menu       |\n");
        printf("+-----------------------------+\n");
        printf("Enter your choice: ");

        char input_str[10];
        if (fgets(input_str, sizeof(input_str), stdin) != NULL) {
            if (sscanf(input_str, "%d", &choice) == 1) {
                switch (choice) {
                    case 1:
                        printf("\n--- Add New Game ---\n");
                        printf("Enter game name: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = 0;

                        printf("Enter game genre: ");
                        fgets(genre, sizeof(genre), stdin);
                        genre[strcspn(genre, "\n")] = 0;

                        printf("Enter game price: ");
                        char price_str[20];
                        fgets(price_str, sizeof(price_str), stdin);
                        if (sscanf(price_str, "%f", &price) != 1) {
                            printf("Invalid price entered. Please enter a number.\n");
                            printf("\nPress Enter to continue...");
                            while ((c = getchar()) != '\n' && c != EOF);
                            break;
                        }

                        addGame(name, genre, price);
                        char addGameLog[100];
                        snprintf(addGameLog, sizeof(addGameLog), "Add %s", name);
                        logging_event(addGameLog, "Admin");
                        break;

                    case 2:
                        printf("\n--- Edit Existing Game ---\n");
                        printf("Enter the name of the game to edit: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = 0;

                        game *gameToEdit = findGame(name);
                        if (gameToEdit != NULL) {
                            printf("Enter new genre (current: %s): ", gameToEdit->genre);
                            fgets(newGenre, sizeof(newGenre), stdin);
                            newGenre[strcspn(newGenre, "\n")] = 0;

                            printf("Enter new price (current: %.2f): ", gameToEdit->price);
                            char new_price_str[20];
                            fgets(new_price_str, sizeof(new_price_str), stdin);
                            if (sscanf(new_price_str, "%f", &newPrice) != 1) {
                                printf("Invalid new price entered.\n");
                                printf("\nPress Enter to continue...");
                                while ((c = getchar()) != '\n' && c != EOF);
                                break;
                            }

                            editGame(name, newGenre, newPrice);
                        } else {
                            printf("Game '%s' not found.\n", name);
                        }

                        printf("\nPress Enter to continue...");
                        while ((c = getchar()) != '\n' && c != EOF);
                        break;

                    case 3:
                        printf("\n--- Delete Game ---\n");
                        printf("Enter the name of the game to delete: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = 0;

                        if (findGame(name)) {
                            deleteGame(name);
                        } else {
                            printf("Game '%s' not found.\n", name);
                        }

                        printf("\nPress Enter to continue...");
                        while ((c = getchar()) != '\n' && c != EOF);
                        break;

                    case 4:
                        printf("\n--- Watch System Logging ---\n");
                        char date[11];
                        printf("Select Date (format: YYYY-MM-DD): ");
                        fgets(date, sizeof(date), stdin);
                        date[strcspn(date, "\n")] = 0;
                        CLEAR_SCREEN();
                        display_logging(date);
                        printf("Press Enter to continue...");
                        getchar();                
                        while ((c = getchar()) != '\n' && c != EOF);
                        break;

                    case 5:
                        printf("\n--- Add Relation Between Games ---\n");
                    
                        printf("Name of game [1]: ");
                        fgets(name1, sizeof(name1), stdin);
                        name1[strcspn(name1, "\n")] = 0;
                    
                        printf("Name of game [2]: ");
                        fgets(name2, sizeof(name2), stdin);
                        name2[strcspn(name2, "\n")] = 0;
                    
                        if (findGame(name1) && findGame(name2)) {
                            addRelation(name1, name2, 1);  
                            addRelation(name2, name1, 0);  
                        } else {
                            printf(" One or both games not found. Cannot add relation.\n");
                        }
                    
                        printf("\nPress Enter to continue...");
                        while ((c = getchar()) != '\n' && c != EOF);
                        break;
                    
                    case 6:
                        printf("\nReturning to main menu...\n");
                        while ((c = getchar()) != '\n' && c != EOF);
                        return;

                    default:
                        printf("\nInvalid choice. Please try again.\n");
                        printf("\nPress Enter to continue...");
                        while ((c = getchar()) != '\n' && c != EOF);
                }
            } else {
                printf("\nInvalid input. Please enter a number.\n");
                printf("\nPress Enter to continue...");
                while ((c = getchar()) != '\n' && c != EOF);
            }
        } else {
            printf("\nError reading input. Returning to main menu.\n");
            return;
        }
    }
}
