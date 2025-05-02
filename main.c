#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "login.h"
#include "admin_interface.h"
#include "customer_interface.h"
#include "game.h"
#include "logging_system.h"

#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

void displayMainMenu() {
    printf("\n+-----------------------------+\n");
    printf("|      Game System Menu       |\n");
    printf("+-----------------------------+\n");
    printf("\n+-----------------------------+\n");
    printf("|         Main Menu           |\n");
    printf("+-----------------------------+\n");
    printf("| [1] Admin Login             |\n");
    printf("| [2] Customer Login          |\n");
    printf("| [3] Exit                    |\n");
    printf("+-----------------------------+\n");
    printf("Enter your choice: ");
}

int main() {
    loadGame("games.csv");
    loadRelations("relations.csv");

    int choice;
    char username[100];

    while (1) {
        CLEAR_SCREEN();
        displayMainMenu();

        char input_str[10];
        if (fgets(input_str, sizeof(input_str), stdin) != NULL) {
            if (sscanf(input_str, "%d", &choice) == 1) {

                switch (choice) {
                    case 1:
                        if (admin_login()) {
                            printf("\nAdmin login successful!\n");
                            int c;
                            while ((c = getchar()) != '\n' && c != EOF);
                            adminMenu();
                        } else {
                            printf("\nAdmin login failed.\n");
                             printf("\nPress Enter to continue...");
                             getchar();
                             int c2;
                             while ((c2 = getchar()) != '\n' && c2 != EOF);
                        }
                        break;
                    case 2: {
                        int c3;
                        while ((c3 = getchar()) != '\n' && c3 != EOF);
                        if (user_login(username)) {
                            printf("\nWelcome, %s!\n", username);
                            int c4;
                            while ((c4 = getchar()) != '\n' && c4 != EOF);
                            customerMenu(username);
                        } else {
                            printf("\nUser login failed.\n");
                            printf("\nPress Enter to continue...");
                            getchar();
                            int c5;
                            while ((c5 = getchar()) != '\n' && c5 != EOF);
                        }
                        break;
                        }
                    case 3:
                        printf("\nExiting Game System. Goodbye!\n");
                        return 0;
                    default:
                        printf("\nInvalid choice. Please enter a number between 1 and 3.\n");
                         printf("\nPress Enter to continue...");
                         getchar();
                         int c6;
                         while ((c6 = getchar()) != '\n' && c6 != EOF);
                }
            } else {
                printf("\nInvalid input. Please enter a number.\n");
                 printf("\nPress Enter to continue...");
                 int c;
                 while ((c = getchar()) != '\n' && c != EOF);
            }
        } else {
            printf("\nError reading input. Exiting.\n");
            return 1;
        }
    }

    return 0;
}