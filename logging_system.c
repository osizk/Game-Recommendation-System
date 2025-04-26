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
    
}