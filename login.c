#include "login.h"
#include <stdio.h>
#include <string.h>
#include "logging_system.h" // Include the logging system header

int user_login()
{
    FILE *file = fopen("users.csv", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        // In a real application, you might handle this error differently
        // For now, returning 1 might indicate failure to open,
        // but the function is expected to return 0 or 1 based on login success.
        // Consider returning 0 or adding more robust error handling.
        return 0; // Returning 0 as login cannot succeed without the file
    }

    char input_user[50];
    char input_pass[50];
    char line[256];

    printf("Enter username: ");
    scanf("%s", input_user);
    printf("Enter password: ");
    scanf("%s", input_pass);

    int check = 0; // 0 for failure, 1 for success

    while (fgets(line, sizeof(line), file)) {
        char file_user[50];
        char file_pass[50];

        // Use sscanf to parse username and password from the line
        if (sscanf(line, "%[^,],%s", file_user, file_pass) == 2) {
            // Compare input credentials with credentials from the file
            if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
                check = 1; // Login successful

                // Logging User login - Corrected function name
                char event[100];
                snprintf(event, sizeof(event), "User %s login ", input_user);
                logging_event(event, input_user); // Changed from log_event to logging_event
                break; // Exit the loop after successful login
            }
        }
    }

    fclose(file); // Close the file

    return check; // Return login status
}

int admin_login()
{
    FILE *file = fopen("admin.csv", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        // Similar to user_login, handle file opening error appropriately
        return 0; // Returning 0 as login cannot succeed without the file
    }

    char input_user[50];
    char input_pass[50];
    char line[256];

    printf("Enter admin username: "); // Added "admin" for clarity
    scanf("%s", input_user);
    printf("Enter admin password: "); // Added "admin" for clarity
    scanf("%s", input_pass);

    int check = 0; // 0 for failure, 1 for success

    while (fgets(line, sizeof(line), file)) {
        char file_user[50];
        char file_pass[50];

        // Use sscanf to parse username and password from the line
        if (sscanf(line, "%[^,],%s", file_user, file_pass) == 2) {
            // Compare input credentials with credentials from the file
            if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
                check = 1; // Login successful

                // Logging Admin login - Corrected function name
                char event[100];
                snprintf(event, sizeof(event), "Admin %s login ", input_user);
                logging_event(event, input_user); // Changed from log_event to logging_event
                break; // Exit the loop after successful login
            }
        }
    }

    fclose(file); // Close the file

    return check; // Return login status
}
