#include <stdio.h>
#include <windows.h>
#include "./include/game.h"
#include "./include/DataLoader.h"



// //玩家状态结构体
// typedef struct {
//     HeroType hero;
//     int health;   //血量与最大血量
//     int max_health;
//     int defend;    //格挡值
//     int CurrCost;
//     int MaxCoat;
// }PlayerState;

int main (){
    // #ifdef _WIN32
    SetConsoleOutputCP(65001); // 设置输出为 UTF-8
    // #endif
    PlayerState *MC=(PlayerState*)malloc(sizeof(PlayerState));
    if(MC==NULL){
        Memory_error_print();
    }
    CardState *aaa=Data_load_init_card();
    game_init_player(MC);
    printf("Herotype:%d\n 当前血量：%d\n 最大血量：%d\n 当前格挡值%d\n费用:%d/%d\n",MC->hero,MC->health,MC->max_health,MC->defend,MC->CurrCost,MC->MaxCoat);
    return 0;
}