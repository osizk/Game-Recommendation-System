#include<stdio.h>
#include<string.h>
#include"game.h"

void customerMenu(){
    int input;
    int x=1;
    char name[100];
    while(x){
        printf("\n--- Main Menu ---\n");
        printf("[1] Search\n");
        printf("[2] Purchase\n");
        printf("[3] Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&input);
        while(getchar() != '\n');
        switch (input)
        {
        case 1:
            printf("Enter game name to search: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            game *found = findGame(name);
            if (found != NULL){
                printf("%s (%s, $%.2f)\n", found->name, found->genre, found->price);   
            }
            break;
        case 2:
            //Purchase system
            break;
        case 3:
            x=0;
            break;
        default:
        printf("Invalid input. Please enter 1, 2, or 3.");
            break;
        }
    }
}