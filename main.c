#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "./include/game.h"
#include "./include/DataLoader.h"
#include "./include/Debug.h"
#include "./include/battle.h"
#include "./include/Effect.h"

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
    srand((unsigned int)time(NULL));
    SetConsoleOutputCP(65001); // 设置输出为 UTF-8
    PlayerState mc;
    memset(&mc,0,sizeof(PlayerState));
    // if(mc==NULL){
    //     Memory_error_print();
    // }
    //所有需要的库
    CardLibrary *Card_Library=Data_load_init_card();
    EnemyLibrary *Enemy_Library=Data_load_init_enemy();
    MoveLibrary *Move_Library=Data_load_init_move();

    //玩家初始化
    game_init_player(&mc);


    //显示已加载数据
    print_CardLoaded_array(Card_Library->CardDataArray,Card_Library->len);
    debug_print_enemy_library(Enemy_Library);
    debug_print_move_library(Move_Library);
    //战斗部分  
//战斗初始化
    battle_init_player(&mc);
    int id=1;
    EnemyState enemy=battle_init_enemy(id,Enemy_Library);
    //回合循环
    perror("init finshed");      
    int choice=0;
    while (1){
        int *curr_move=Ai_Resolver(&enemy);
        for(int i=0;i<4;i++){
            printf("%d",curr_move[i]);
        }
        perror("AI_finished");
        enemy.curr_move=Move_Searcher(curr_move[0],Move_Library);

        Print_Player_Status(&mc);
        Print_Enemy_Status(&enemy);

        Turn_Player(&mc,Card_Library);

        /**玩家回合 */
        while(1){
            int judge=scanf("%d",&choice);
            if(judge!=0){
                if(choice==-1){
                    break;
                }else{
                    //出牌
                    CardState curr_card=Card_Searcher(mc.battle_context.hand[choice],Card_Library);
                    Card_Resolver(&mc,&enemy,Card_Library,curr_card.id,curr_card.CardCost);
                    Print_Player_Status(&mc);
                    Print_Enemy_Status(&enemy);
                    //出牌后重新调整手牌排序
                }

            }else{
                printf("非法输入，重新输入\n");
            }
        }
        printf("回合结束\n");
        Turn_Enemy(&enemy,&mc);
        //招式数值复制
        int value[3];
        for(int i=0;i<3;i++){
            value[i]=curr_move[i+1];
        }
        Move_Resolver(curr_move[0],value,Move_Library,&enemy,&mc);
        Print_Player_Status(&mc);
        Print_Enemy_Status(&enemy);
        }

//战斗结算
    //洗牌测试
    // int a[10]={0,1,2,3,4,5,6,7,8,9};
    // Card_Shuffier(a,10);
    free(Card_Library);
    free(Enemy_Library);
    free(Move_Library);
    return 0;
}