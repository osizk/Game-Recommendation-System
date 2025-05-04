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
    CLEAR_SCREEN();
    printf("\n+-----------------------------+\n");
    printf("|      Game System Menu       |\n");
    printf("+-----------------------------+\n");
    printf("| [1] Login                   |\n");
    printf("| [2] Register                |\n");
    printf("| [3] Exit                    |\n");
    printf("+-----------------------------+\n");
    printf("Enter your choice: ");
}

int main() {
    loadGame("games.csv");
    loadRelations("relations.csv");
    int c;
    int choice;
    char username[100];

    while (1) {
        displayMainMenu();
        int input;
        scanf("%d",&input);
        while ((c = getchar()) != '\n' && c != EOF);
        switch (input) {
            case 1:
                switch (login(username)) {
                    case 1:
                        customerMenu(username);
                        break;
                    case 2: 
                        adminMenu();
                        break;
                    case 3:
                        printf("\nInvalid username or password.\n");
                        printf("Press Enter to continue...");
                        while ((c = getchar()) != '\n' && c != EOF);
                        break;
                    default:
                        printf("\nInvalid choice.\n");
                        printf("Press Enter to continue...");
                        while ((c = getchar()) != '\n' && c != EOF);
                }
                break;
            case 2: 
                user_register();
                printf("Press Enter to continue...");
                while ((c = getchar()) != '\n' && c != EOF);
                break;
            case 3:
                printf("\nExiting program...\n");
                return 0;
            default:
                printf("\nInvalid choice.\n");
                printf("Press Enter to continue...");
                while ((c = getchar()) != '\n' && c != EOF);
        }
                
    }
    return 0;
}