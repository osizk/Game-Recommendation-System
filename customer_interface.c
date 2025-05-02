#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h> // Add this line to include stdlib.h
#include"game.h"
#include "logging_system.h"

#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif
#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

void customerMenu(const char *username){
    setCart();
    int input;
    char name[100];

    while(1){
        CLEAR_SCREEN();
        printf("\n+-----------------------------+\n");
        printf("|       Customer Menu         |\n");
        printf("+-----------------------------+\n");
        printf("| [1] Search Games            |\n");
        printf("| [2] Add to Cart             |\n");
        printf("| [3] Delete from Cart        |\n");
        printf("| [4] View Cart               |\n");
        printf("| [5] Checkout                |\n");
        printf("| [6] View Purchase History   |\n");
        printf("| [7] Back to Main Menu       |\n");
        printf("+-----------------------------+\n");
        printf("Enter your choice: ");

        char input_str[10];
        if (fgets(input_str, sizeof(input_str), stdin) != NULL) {
            if (sscanf(input_str, "%d", &input) == 1) {

                switch (input)
                {
                case 1:
                    printf("\n--- Search Game ---\n");
                    printf("Enter game to search: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';
                    game *found = findGame(name);
                    if (found != NULL){
                        printf("\nFound: %s (%s, $%.2f)\n", found->name, found->genre, found->price);
                    } else {
                         printf("\nGame '%s' not found.\n", name);
                    }
                    break;
                case 2:
                    printf("\n--- Add to Cart ---\n");
                    printf("Enter game to add to cart: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';
                    addtoCart(name);
                    break;
                case 3:
                     printf("\n--- Delete from Cart ---\n");
                     printf("Enter game to delete from cart: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';
                    deletefromCart(name);
                    break;
                case 4:
                    viewCart();
                    break;
                case 5:
                    checkout(username);
                    break;
                case 6:
                     printf("\n--- Your Purchase History ---\n");
                    display_user_logging(username);
                    break;
                case 7:
                    printf("\nReturning to main menu...\n");
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    return;
                default:
                    printf("\nInvalid input. Please try again.\n");
                     printf("\nPress Enter to continue...");
                     getchar();
                      int c2;
                     while ((c2 = getchar()) != '\n' && c2 != EOF);
                    break;
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

         if (input != 7) {
             printf("\nPress Enter to continue...");
             getchar();
             int c;
             while ((c = getchar()) != '\n' && c != EOF);
         }
    }
}