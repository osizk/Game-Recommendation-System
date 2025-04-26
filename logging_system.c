#include "logging_system.h"
#include <stdio.h>
#include <time.h>

void logging_event(const char *event, const char *user)
{
    FILE *fp = fopen("logging.txt","a");
    if(fp == NULL)
    {
        printf("Can't open logging.txt\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d | %s | %s\n",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        event,
        user
    );

    fclose(fp);
}
// command use for admin logging_event(event,admin);
// command use for other user logging_event(event,other user);


void display_logging()
{
    FILE *fp = fopen("lpgging.txt","a");
    if(fp == NULL)
    {
        printf("Can't open logging.txt\n");
        return;
    }

    char line[512];
    printf("----SystemLoggingHead----");
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }
    printf("----SystemLoggingBottom----");

    fclose(fp);
}