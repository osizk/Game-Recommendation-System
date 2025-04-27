#include "logging_system.h"
#include <stdio.h>
#include <time.h>
#include <string.h> // Include for strcspn if needed for string manipulation

// Function to log an event with associated user information to a file
void logging_event(const char *event, const char *user)
{
    // Open the logging file in append mode
    FILE *fp = fopen("logging.txt","a");
    if(fp == NULL)
    {
        // Print an error if the file cannot be opened
        printf("Can't open logging.txt\n");
        return;
    }

    // Get the current time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // Format and print the log entry to the file
    fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d | %s | %s\n",
        t->tm_year + 1900, // Year since 1900
        t->tm_mon + 1,     // Month (0-11, so add 1)
        t->tm_mday,        // Day of the month
        t->tm_hour,        // Hour
        t->tm_min,         // Minute
        t->tm_sec,         // Second
        event,             // The event string
        user               // The user string
    );

    // Close the file
    fclose(fp);
}

// Function to display the contents of the logging file
void display_logging()
{
    // Open the logging file in read mode (changed from "a" to "r" to read)
    FILE *fp = fopen("logging.txt","r");
    if(fp == NULL)
    {
        // Print an error if the file cannot be opened
        // Note: If the file doesn't exist yet, this is expected.
        printf("Can't open logging.txt\n");
        return;
    }

    char line[512];
    printf("\n----System Logging Head----\n"); // Added newlines for better formatting
    // Read and print each line from the logging file
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }
    printf("----System Logging Bottom----\n\n"); // Added newlines for better formatting

    // Close the file
    fclose(fp);
}
