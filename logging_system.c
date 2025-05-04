#include "logging_system.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#define LOG_FOLDER "Logging"


// Constructs the log filename based on the given date and stores it in 'filename'.
// The result will be something like "Logging/logging_2025-04-28.csv"
void CreateLogbyDate(char *filename, size_t size, const char *date)
{
    // Format: Logging/logging_<date>.csv  
    snprintf(filename, size, LOG_FOLDER"/logging_%s.csv", date);
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
    FILE *fp = fopen(filename,"a+");
    if(fp == NULL)
    {
        // Print an error if the file cannot be opened
        printf("Can't open file: %s\n",filename);
        return;
    }

    //Check is Empty file or not if Empty Add Head of file
    fseek(fp, 0, SEEK_END);   
    long size = ftell(fp);    

    if (size == 0) {
        fprintf(fp, "Date,Time,Activity,User\n"); //Set Header of CSV
    }

    fseek(fp, 0, SEEK_END);


    // Format and print the log entry to the file
    fprintf(fp, "%04d-%02d-%02d,%02d:%02d:%02d,%s,%s\n",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        event,
        user);

    // Close the file
    fclose(fp);
}

//function Display Logging
void display_logging(const char *date)
{

    // Construct the log file path based on the provided date
    char filename[256];
    CreateLogbyDate(filename, sizeof(filename), date);  

    // Open the log file for reading
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Can't open file: %s\n", filename);
        return;
    }
    // Print head of logging
    char line[512];
    printf("+-----------------------------------------------------------------------------------+\n");
    printf("|                             System Logging: %s                            |\n", date);
    printf("|----------+-------------------------------------------------------------+----------|\n");
    printf("|%-10s|%-61s|%-10s|\n", "Time", "Activity", "User");
    printf("|----------+-------------------------------------------------------------+----------|\n");

    // Skip the CSV head line
    fgets(line, sizeof(line), fp);

    // Read and parse each log line
    while (fgets(line, sizeof(line), fp))
    {
        char tempdate[10], time[10],activity[100], user[20];
        
        // Parse the CSV line into individual fields
        if (sscanf(line, "%[^,],%[^,],%[^,],%s",tempdate, time, activity, user) == 4) {

            // Display the parsed log entry in table format
            printf("|%-10s|%-61s|%-10s|\n", time, activity, user);
        }
    }

    // Print closing table border
    printf("+-----------------------------------------------------------------------------------+\n");

    // Close the file
    fclose(fp);
}