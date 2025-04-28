#include "logging_system.h"
#include <stdio.h>
#include <time.h>
#include <string.h> // Include for strcspn if needed for string manipulation
#define LOG_FOLDER "Logging"

void CreateLogbyDate(char *filename, size_t size, const char *date)
{  
    snprintf(filename, size, LOG_FOLDER"/logging_%s.txt", date);
}

// Function to log an event with associated user information to a file
void logging_event(const char *event, const char *user)
{

    // Get the current time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char today[11];
    snprintf(today, sizeof(today), "%04d-%02d-%02d",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday);

    char filename[256];
    CreateLogbyDate(filename, sizeof(filename),today);

    // Open the logging file in append mode
    FILE *fp = fopen(filename,"a");
    if(fp == NULL)
    {
        // Print an error if the file cannot be opened
        printf("Can't open file: %s\n",filename);
        return;
    }

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
void display_logging(const char *date)
{
    char filename[256];
    CreateLogFileNameByDate(filename, sizeof(filename), date);
    
    // Open the logging file in read mode (changed from "a" to "r" to read)
    FILE *fp = fopen(filename,"r");
    if(fp == NULL)
    {
        // Print an error if the file cannot be opened
        // Note: If the file doesn't exist yet, this is expected.
        printf("Can't open file: %s\n", filename);
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

//Prepare for main

/*
#include "logging_system.h"
#include <stdio.h>

int main()
{
    int choice;
    char date[11];

    printf("1. watch Logging\n");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Select Date (format: YYYY-MM-DD): ");
        scanf("%10s", date);
        display_logging_by_date(date);
    } else {
        printf("Wrong choice\n");
    }

    return 0;
}
*/