#include <stdio.h>
#include "login.h"
#include "game.h"

int main() {
    loadgame_hash("games.csv");
    int input;
    while(1){
    printf("[1]Admin\n");
    printf("[2]Customer\n");
    printf("[3]Exit\n");
    scanf("%d",&input);
    if(input == 1){
        while(1){
            if(admin_login()){
                printf("success\n");
                break;
            }else{
                printf("fail\n");
            }
        }
    }else if(input == 2){
        while(1){
            if(user_login()){
                printf("success\n");
                printHashTable();
                break;
            }else{
                printf("fail\n");
            }
        }
    }else if(input == 3){
        break;
    }else{
        printf("input error\n");
    }
    }
    
    return 0;
}