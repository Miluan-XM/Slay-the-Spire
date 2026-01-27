#include <stdio.h>
#include <windows.h>
#include "./include/game.h"
#include "./include/DataLoader.h"
#include "./include/Debug.h"



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
    PlayerState *mc=(PlayerState*)malloc(sizeof(PlayerState));
    if(mc==NULL){
        Memory_error_print();
    }
    CardLibrary *Card_Library=Data_load_init_card();
    EnemyLibrary *Enemy_Library=Data_load_init_enemy();
    MoveLibrary *Move_Library=Data_load_init_move();
    game_init_player(mc);


    printf("Herotype:%d\n 当前血量：%d\n 最大血量：%d\n 当前格挡值%d\n费用:%d/%d\n",mc->hero,mc->health,mc->max_health,mc->defend,mc->CurrCost,mc->MaxCoat);



    
    print_CardLoaded_array(Card_Library->CardDataArray,Card_Library->len);
    debug_print_enemy_library(Enemy_Library);
    debug_print_move_library(Move_Library);


    free(Card_Library);
    free(Enemy_Library);
    free(Move_Library);
    free(mc);
    return 0;
}