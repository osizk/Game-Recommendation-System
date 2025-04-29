#include <stdio.h>
#include "login.h"
#include "game.h"
#include "admin_interface.h"
#include "customer_interface.h"
int main() {
    loadGame("games.csv");
    loadRelations("relations.csv");
    int input;

    while (1) {
        printf("\n--- Main Menu ---\n");
        printf("[1] Admin\n");
        printf("[2] Customer\n");
        printf("[3] Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &input);

        while (getchar() != '\n');

        if (input == 1) {
            if (admin_login()) {
                printf("Admin login successful.\n");
                adminMenu();
            } else {
                printf("Admin login failed.\n");
            }
        } else if (input == 2) {
            if (user_login()) {
                printf("Customer login successful.\n");
                printgamelist();
                customerMenu();
            } else {
                printf("Customer login failed.\n");
            }
        } else if (input == 3) {
            printf("Exiting program.\n");
            break;
        } else {
            printf("Invalid input. Please enter 1, 2, or 3.\n");
        }
    }

    return 0;
}
