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

int login(char *username){
    CLEAR_SCREEN();
    printf("\n+-----------------------------+\n");
    printf("|             Login           |\n");
    printf("+-----------------------------+\n");

    char input_user[50];
    char input_pass[50];
    char line[256];

    printf("Enter username: ");//input username
    if (fgets(input_user, sizeof(input_user), stdin) == NULL) {
        printf("Error username.\n");
        return 0;
    }
    input_user[strcspn(input_user, "\n")] = 0;

    printf("Enter password: ");//input password
    if (fgets(input_pass, sizeof(input_pass), stdin) == NULL) {
        printf("Error password.\n");
        return 0;
    }
    input_pass[strcspn(input_pass, "\n")] = 0;

    if(user_login(username,input_user,input_pass)){//check user
        return 1;
    }else if(admin_login(input_user,input_pass)){//check admin
        return 2;
    }else{
        return 3;//error
    }
}

int user_login(char *username,char *input_user,char *input_pass) {
    FILE *file = fopen("users.csv", "r");
    if (file == NULL) {
        printf("file not found");
        return 0;
    }

    int check = 0;
    char line[256];

    fgets(line, sizeof(line), file);//skip first line

    while (fgets(line, sizeof(line), file)) {
        char file_user[50];
        char file_pass[50];
        if (sscanf(line, "%[^,],%s", file_user, file_pass) == 2) {
            //checking is username and password are match with user.csv
            if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
                check = 1;
                strcpy(username, input_user);
                //logging
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

int admin_login(char *input_user,char *input_pass) {
    FILE *file = fopen("admin.csv", "r");
    if (file == NULL) {
        printf("file not found");
        return 0;
    }

    char line[256];
    int check = 0;

    fgets(line, sizeof(line), file);//skip first line

    while (fgets(line, sizeof(line), file)) {
        char file_user[50];
        char file_pass[50];
        if (sscanf(line, "%[^,],%s", file_user, file_pass) == 2) {
            //checking is username and password are match with admin.csv
            if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
                check = 1;
                //logging
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

int user_register() {
    CLEAR_SCREEN();
    printf("\n+-----------------------------+\n");
    printf("|       User Registration     |\n");
    printf("+-----------------------------+\n");

    FILE *file = fopen("users.csv", "a+");
    if (file == NULL) {
        printf("Error opening user database.\n");
        return 0;
    }

    char new_user[50];
    char new_pass[50];
    char confirm_pass[50];
    char line[256];

    // Get username
    printf("Enter new username: ");
    if (fgets(new_user, sizeof(new_user), stdin) == NULL) {
        printf("Error username.\n");
        fclose(file);
        return 0;
    }
    new_user[strcspn(new_user, "\n")] = 0;

    // Check if username already exists
    while (fgets(line, sizeof(line), file)) {
        char exist_user[50];
        if (sscanf(line, "%[^,],", exist_user) == 1) {
            if (strcmp(new_user, exist_user) == 0) {
                printf("Username already exists. Registration failed.\n");
                fclose(file);
                return 0;
            }
        }
    }

    // Get password
    printf("Enter new password: ");
    if (fgets(new_pass, sizeof(new_pass), stdin) == NULL) {
        printf("Error password.\n");
        fclose(file);
        return 0;
    }
    new_pass[strcspn(new_pass, "\n")] = 0;

    // Confirm password
    printf("Confirm password: ");
    if (fgets(confirm_pass, sizeof(confirm_pass), stdin) == NULL) {
        printf("Error confirm password.\n");
        fclose(file);
        return 0;
    }
    confirm_pass[strcspn(confirm_pass, "\n")] = 0;

    // Verify passwords
    if (strcmp(new_pass, confirm_pass) != 0) {
        printf("Passwords do not match. Registration failed.\n");
        fclose(file);
        return 0;
    }

    // Add newuser to file
    fprintf(file, "%s,%s\n", new_user, new_pass);
    fclose(file);

    // Logging
    char event[100];
    snprintf(event, sizeof(event), "New user registered: %s", new_user);
    logging_event(event, new_user);

    printf("Registration successful!\n");
    return 1;
}