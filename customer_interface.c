#include<stdio.h>
#include<string.h>
#include"game.h"

void customerMenu(){
    setCart();
    int input;
    int x=1;
    char name[100];
    while(x){
        printf("\n--- Main Menu ---\n");
        printf("[1] Search\n");
        printf("[2] Add to cart\n");
        printf("[3] Delete from cart\n");
        printf("[4] View cart\n");
        printf("[5] Checkout\n");
        printf("[6] Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&input);
        while(getchar() != '\n');
        switch (input)
        {
        case 1:
            printf("Enter game to search: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            game *found = findGame(name);
            if (found != NULL){
                printf("%s (%s, $%.2f)\n", found->name, found->genre, found->price);   
            }
            break;
        case 2:
            printf("Enter game for add to cart: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            addtoCart(name);
            break;
        case 3:
            printf("Enter game for delete from cart: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            deletefromCart(name);
            break;
        case 4:
            viewCart();
            break;
        case 5:
            checkout();
            break;
        case 6:
            x=0;
            break;
        default:
        printf("Invalid input.");
            break;
        }
    }
}