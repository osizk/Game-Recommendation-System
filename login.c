#include "login.h"
#include <stdio.h>
#include <string.h>

int user_login()
{
    FILE *file = fopen("users.csv", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    char input_user[50];
    char input_pass[50];
    char line[256];

    printf("Enter username: ");
    scanf("%s", input_user);
    printf("Enter password: ");
    scanf("%s", input_pass);

    int check = 0;

    while (fgets(line, sizeof(line), file)) {
        char file_user[50];
        char file_pass[50];

        if (sscanf(line, "%[^,],%s", file_user, file_pass) == 2) {
            if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
                check = 1;
                break;
            }
        }
    }

    fclose(file);

    return check;
}

int admin_login()
{
    FILE *file = fopen("admins.csv", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    char input_user[50];
    char input_pass[50];
    char line[256];

    printf("Enter username: ");
    scanf("%s", input_user);
    printf("Enter password: ");
    scanf("%s", input_pass);

    int check = 0;

    while (fgets(line, sizeof(line), file)) {
        char file_user[50];
        char file_pass[50];

        if (sscanf(line, "%[^,],%s", file_user, file_pass) == 2) {
            if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
                check = 1;
                break;
            }
        }
    }

    fclose(file);

    return check;
}