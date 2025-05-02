#include "login.h"
#include <stdio.h>
#include <string.h>
#include "logging_system.h"
#include "game.h"
#include <stdlib.h>
#include <ctype.h>


#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif


int user_login(char *username) {
     CLEAR_SCREEN();
    printf("\n+-----------------------------+\n");
    printf("|        User Login           |\n");
    printf("+-----------------------------+\n");

    FILE *file = fopen("users.csv", "r");
    if (file == NULL) {
        perror("Error opening users file");
        printf("User login is currently unavailable.\n");
        printf("\nPress Enter to continue...");
        getchar();
        return 0;
    }

    char input_user[50];
    char input_pass[50];
    char line[256];

    printf("Enter username: ");
    if (fgets(input_user, sizeof(input_user), stdin) == NULL) {
        printf("Error reading username.\n");
        fclose(file);
        return 0;
    }
    input_user[strcspn(input_user, "\n")] = 0;

    printf("Enter password: ");
    if (fgets(input_pass, sizeof(input_pass), stdin) == NULL) {
        printf("Error reading password.\n");
        fclose(file);
        return 0;
    }
    input_pass[strcspn(input_pass, "\n")] = 0;

    int check = 0;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char file_user[50];
        char file_pass[50];
        if (sscanf(line, "%[^,],%s", file_user, file_pass) == 2) {
            if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
                check = 1;
                strcpy(username, input_user);
                char event[100];
                snprintf(event, sizeof(event), "User %s login", input_user);
                logging_event(event, input_user);
                loadUserPurchaseHistory(username);
                break;
            }
        }
    }

    fclose(file);
    return check;
}

int admin_login() {
    CLEAR_SCREEN();
    printf("\n+-----------------------------+\n");
    printf("|         Admin Login         |\n");
    printf("+-----------------------------+\n");

    FILE *file = fopen("admin.csv", "r");
    if (file == NULL) {
        perror("Error opening admin file");
         printf("Admin login is currently unavailable.\n");
          printf("\nPress Enter to continue...");
          getchar();
        return 0;
    }

    char input_user[50];
    char input_pass[50];
    char line[256];

    printf("Enter admin username: ");
    if (fgets(input_user, sizeof(input_user), stdin) == NULL) {
        printf("Error reading admin username.\n");
        fclose(file);
        return 0;
    }
    input_user[strcspn(input_user, "\n")] = 0;

    printf("Enter admin password: ");
    if (fgets(input_pass, sizeof(input_pass), stdin) == NULL) {
         printf("Error reading admin password.\n");
        fclose(file);
        return 0;
    }
    input_pass[strcspn(input_pass, "\n")] = 0;


    int check = 0;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char file_user[50];
        char file_pass[50];
        if (sscanf(line, "%[^,],%s", file_user, file_pass) == 2) {
            if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
                check = 1;
                char event[100];
                snprintf(event, sizeof(event), "Admin %s login", input_user);
                logging_event(event, input_user);
                break;
            }
        }
    }

    fclose(file);
    return check;
}