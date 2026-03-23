#include "../include/game.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"

//查阅函数
CardState  Card_Searcher(int id,CardLibrary *lib);
MoveFact  Move_Searcher(int id,MoveLibrary *lib);
EnemyData Enemy_Searcher(int id,EnemyLibrary *lib);
void Card_Shuffier(int card[],int len);
int RandNum_between(int a,int b);
void swap_value(int *a,int *b);



void game_init_player(PlayerState *player){


    //卡牌
    memset(&player->battle_context,0,sizeof(BattleContext));
    memset(&player->master_card,0,sizeof(Master_Card));

    //角色选择
    printf("choose your hero:\n");
    printf("1.The Ironclad 2.The Slient 3.The defect 4.The Watcher\n");
    while(1){
        if(scanf("%d",&player->hero)){
            break;
        }else {
            printf("重新输入：\n");
            
        }
    }

    switch (player->hero){
    case Ironclad:
        player->max_health=Ironclad_MAX_Health;
            Init_Card_Ironclad(player);
        break;
    case Slient:
        player->max_health=Slient_MAX_Health;
        break;
    case Defect:
        player->max_health=Defect_MAX_Health;
        break;
    case Watcher:
        player->max_health=Watcher_MAX_Health;
        break;
    default:
        printf("error\n");
        return;
        break;
    }
    //基础状态
    player->health=player->max_health;
    player->defend=0;
    player->MaxCoat=3;
    player->CurrCost=3;

    //战士职业
    //打击初始

}
 void Init_Card_Ironclad(PlayerState *player){
    int i = 0;
    //打击
    for (i = 0; i < 5; i++)
    {
        player->master_card.master[i] = 1;
        player->master_card.master_count++;
    }
    // 防御初始
    for (i = 5; i < 9; i++)
    {
        player->master_card.master[i] = 2;
        player->master_card.master_count++;
    }
    // 痛击
    player->master_card.master[9] = 3;
    player->master_card.master_count++;
    player->gold = 90;
}
void PlayerState_print(PlayerState *mc){
    printf("Herotype:%d\n 当前血量：%d\n 最大血量：%d\n 当前格挡值%d\n费用:%d/%d\n",mc->hero,mc->health,mc->max_health,mc->defend,mc->CurrCost,mc->MaxCoat);

}
CardState  Card_Searcher(int id,CardLibrary *lib){
    CardState  card=lib->CardDataArray[id-1];
    return card;
}
MoveFact Move_Searcher(int id,MoveLibrary *lib){
    MoveFact move=lib->movedata[id-501];
    return move;
}
EnemyData Enemy_Searcher(int id,EnemyLibrary *lib){
    EnemyData enemy=lib->enemystate[id-1];
    return enemy;
}
int RandNum_between(int a,int b){
    int max=b;
    int min=a;
    if(a>b){
        max=a;
        min=b;
    }
    int result=rand()%(max-min+1)+min;
    return result;
}

void Card_Shuffier(int card[],int len){

    int i=0;
    printf("\n");
    for(i=len-1;i>-1;i--){
        int j=rand()%(i+1);
        swap_value(&card[i],&card[j]);
    }
    printf("Shuffier finished");
    printf("\n");
}


const char* UI_Get_Buff_Name(int index) {
    switch(index) {
        case Buff_STRENGTH:   return "STR"; // 力量
        case Buff_DEXTERITY:  return "DEX"; // 敏捷
        case Buff_THORNS:     return "THN"; // 荆棘
        case Buff_BARRIER:    return "BAR"; // 壁垒
        case DeBuff_VALNERABALE: return "VUL"; // 易伤
        case DeBuff_WEAK:     return "WEK"; // 虚弱
        case Debuff_FRAIL:    return "FRL"; // 脆弱
        case EnemyBuff_RITUAL:return "RIT"; // 仪式
        default: return "???";
    }
}
void Print_Player_Status(PlayerState *p) {
    printf("\n======================= PLAYER =======================\n");
    
    // 1. 血量条展示 [██████░░░░]
    int bar_width = 20;
    int filled = (p->health * bar_width) / p->max_health;
    printf("HP: [");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) printf("█");
        else printf("░");
    }
    printf("] %d/%d  ", p->health, p->max_health);

    // 2. 格挡与能量
    printf("| Block: %-2d | Energy: %d/%d\n", p->defend, p->CurrCost, p->MaxCoat);

    // 3. Buff 展示 (只打印层数 > 0 的)
    printf("Buffs: ");
    int has_buff = 0;
    for (int i = 0; i < STAUS_MAX_COUNT; i++) {
        if (p->PlayerBuff[i] > 0) {
            printf("(%s:%d) ", UI_Get_Buff_Name(i), p->PlayerBuff[i]);
            has_buff = 1;
        }
    }
    if (!has_buff) printf("None");
    printf("\n------------------------------------------------------\n");
}

void Print_Enemy_Status(EnemyState *e) {
    printf("\n----------------------- ENEMY -----------------------\n");
    
    // 1. 名字与意图
    printf("意图: %-12s\n",e->curr_move.name );

    // 2. 血量条
    int bar_width = 20;
    int filled = (e->health * bar_width) / e->enemydata.Max_health;
    printf("HP: [");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) printf("█");
        else printf("░");
    }
    printf("] %d/%d  ", e->health, e->enemydata.Max_health);

    // 3. 格挡
    printf("| Block: %-2d\n", e->defend);

    // 4. Buff 展示
    printf("Buffs: ");
    int has_buff = 0;
    for (int i = 0; i < STAUS_MAX_COUNT; i++) {
        if (e->EnemyBuff[i] > 0) {
            printf("(%s:%d) ", UI_Get_Buff_Name(i), e->EnemyBuff[i]);
            has_buff = 1;
        }
    }
    if (!has_buff) printf("None");
    printf("\n=====================================================\n");
}
