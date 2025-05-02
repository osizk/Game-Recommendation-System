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
        printf("| [5] View Game List          |\n");
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
                             getchar();
                            break;
                        }

                        addGame(name, genre, price);

                        char addGameLog[100];
                        snprintf(addGameLog, sizeof(addGameLog), "Add %s", name);
                        logging_event(addGameLog,"Admin");

                        break;
                    case 2:
                        printf("\n--- Edit Existing Game ---\n");
                         printf("Enter the name of the game to edit: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = 0;

                        game *gameToEdit = findGame(name);
                        if (gameToEdit != NULL) {
                            printf("Enter new genre: (current: %s): ", gameToEdit->genre);
                            fgets(newGenre, sizeof(newGenre), stdin);
                            newGenre[strcspn(newGenre, "\n")] = 0;

                            printf("Enter new price: (current: %.2f): ", gameToEdit->price);
                            char new_price_str[20];
                            fgets(new_price_str, sizeof(new_price_str), stdin);
                            if (sscanf(new_price_str, "%f", &newPrice) != 1) {
                                printf("Invalid new price entered. Please enter a number.\n");
                                 printf("\nPress Enter to continue...");
                                 getchar();
                                break;
                            }

                            editGame(name, newGenre, newPrice);
                             char editGameLog[100];
                             snprintf(editGameLog, sizeof(editGameLog), "Edit %s", name);
                             logging_event(editGameLog,"Admin");


                        } else {
                            printf("Game '%s' not found. Cannot edit.\n", name);
                        }
                        break;
                    case 3:
                        printf("\n--- Delete Game ---\n");
                        printf("Enter the name of the game to delete: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = 0;

                        deleteGame(name);

                        break;
                    case 4:
                        {
                            char date[11];
                            printf("\n--- Watch System Logging ---\n");
                            printf("Select Date (format:YYYY-MM-DD): ");
                            if (fgets(date, sizeof(date), stdin) != NULL) {
                                date[strcspn(date, "\n")] = 0;
                                display_logging(date);
                            } else {
                                printf("Error reading date input.\n");
                            }
                        }
                        break;
                    case 5:
                        printf("\nPress Enter to continue and view the game list...");
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);

                         CLEAR_SCREEN();
                        printgamelist();
                         printf("\nPress Enter to return to the Admin Menu...");
                         getchar();
                         int c2;
                         while ((c2 = getchar()) != '\n' && c2 != EOF);
                        break;
                    case 6:
                        printf("\nReturning to main menu...\n");
                         int c3;
                         while ((c3 = getchar()) != '\n' && c3 != EOF);
                        return;
                    default:
                        printf("\nInvalid choice. Please try again.\n");
                         printf("\nPress Enter to continue...");
                         getchar();
                         int c4;
                         while ((c4 = getchar()) != '\n' && c4 != EOF);
                }
            } else {
                printf("\nInvalid input. Please enter a number.\n");
                 printf("\nPress Enter to continue...");
                 int c;
                 while ((c = getchar()) != '\n' && c != EOF);
            }
        } else {
            printf("\nError reading input. Returning to main menu.\n");
            return;
        }
    }
}