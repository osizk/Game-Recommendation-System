#include "logging_system.h"
#include <stdio.h>
#include <time.h>
#include <string.h> // Include for strcspn if needed for string manipulation
#define LOG_FOLDER "Logging"
#define USER_FOLDER "UserLogging"

void CreateLogbyDate(char *filename, size_t size, const char *date)
{  
    snprintf(filename, size, LOG_FOLDER"/logging_%s.csv", date);
}

void CreateLogUser(char *filename, size_t size,const char *user)
{  
    snprintf(filename, size, USER_FOLDER"/%s.csv", user);
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

void logging_user(const char *event, const char *user)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char filename[256];
    CreateLogUser(filename, sizeof(filename), user);

    FILE *fp = fopen(filename, "a+");
    if (fp == NULL)
    {
        printf("Can't open user log file: %s\n", filename);
        return;
    }


    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    if (size == 0) {
        fprintf(fp, "Date,Time,Activity,User\n");
    }

    fseek(fp, 0, SEEK_END); 

    fprintf(fp, "%04d-%02d-%02d,%02d:%02d:%02d,%s,%s\n",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday,
            t->tm_hour,
            t->tm_min,
            t->tm_sec,
            event,
            user);

    fclose(fp);
}

void display_user_logging(const char *user)
{
    char filename[256];
    CreateLogUser(filename, sizeof(filename), user);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("No log found for user: %s\n", user);
        return;
    }

    char line[512];
    printf("\n---- User Logging: %s ----\n", user);
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }
    printf("---- End of User Logging ----\n\n");

    fclose(fp);
}