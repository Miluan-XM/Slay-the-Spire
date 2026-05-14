#include "../include/Game.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Game.hpp"

CardState Card_Searcher(int id,CardLibrary *lib);
MoveFact Move_Searcher(int id,MoveLibrary *lib);
EnemyData Enemy_Searcher(int id,EnemyLibrary *lib);
void Card_Shuffier(int card[],int len);
int RandNum_between(int a,int b);
void swap_value(int *a,int *b);

void game_init_player(PlayerState *player){
    player->battle_context = BattleContext();
    player->master_card = Master_Card();
    player->PlayerBuff.assign(STAUS_MAX_COUNT, 0);

    printf("choose your hero:\n");
    printf("1.The Ironclad 2.The Slient 3.The defect 4.The Watcher\n");
    while(1){
        if(scanf("%d",&player->hero)){
            break;
        }else{
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
    }
    player->health=player->max_health;
    player->defend=0;
    player->MaxCoat=3;
    player->CurrCost=3;
}

void Init_Card_Ironclad(PlayerState *player){
    int i = 0;
    player->master_card.master.assign(100, 0);
    for (i = 0; i < 5; i++){
        player->master_card.master[i] = 1;
        player->master_card.master_count++;
    }
    for (i = 5; i < 9; i++){
        player->master_card.master[i] = 2;
        player->master_card.master_count++;
    }
    player->master_card.master[9] = 3;
    player->master_card.master_count++;
    player->gold = 90;
}

CardState Card_Searcher(int id,CardLibrary *lib){
    if (id < 1 || id > (int)lib->CardDataArray.size()) {
        printf("Error: Card id %d out of range (size: %zu)\n", id, lib->CardDataArray.size());
        return CardState();
    }
    return lib->CardDataArray[id-1];
}
MoveFact Move_Searcher(int id,MoveLibrary *lib){
    if (id < 501 || id > 501 + (int)lib->movedata.size()) {
        printf("Error: Move id %d out of range\n", id);
        return MoveFact();
    }
    return lib->movedata[id-501];
}
EnemyData Enemy_Searcher(int id,EnemyLibrary *lib){
    if (id < 1 || id > (int)lib->enemystate.size()) {
        printf("Error: Enemy id %d out of range (size: %zu)\n", id, lib->enemystate.size());
        return EnemyData();
    }
    return lib->enemystate[id-1];
}

int RandNum_between(int a,int b){
    int max=b;
    int min=a;
    if(a>b){ max=a; min=b; }
    return rand()%(max-min+1)+min;
}

void Card_Shuffier(int card[],int len){
    printf("\n");
    for(int i=len-1;i>-1;i--){
        int j=rand()%(i+1);
        swap_value(&card[i],&card[j]);
    }
    printf("Shuffier finished\n");
}

const char* UI_Get_Buff_Name(int index) {
    switch(index) {
        case Buff_STRENGTH:   return "STR";
        case Buff_DEXTERITY:  return "DEX";
        case Buff_THORNS:     return "THN";
        case Buff_BARRIER:    return "BAR";
        case DeBuff_VALNERABALE: return "VUL";
        case DeBuff_WEAK:     return "WEK";
        case Debuff_FRAIL:    return "FRL";
        case EnemyBuff_RITUAL:return "RIT";
        default: return "???";
    }
}

void Print_Player_Status(PlayerState *p) {
    printf("\n======================= PLAYER =======================\n");
    int bar_width = 20;
    int filled = (p->health * bar_width) / (p->max_health > 0 ? p->max_health : 1);
    printf("HP: [");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) printf("█");
        else printf("░");
    }
    printf("] %d/%d  ", p->health, p->max_health);
    printf("| Block: %-2d | Energy: %d/%d\n", p->defend, p->CurrCost, p->MaxCoat);
    printf("Buffs: ");
    int has_buff = 0;
    for (int i = 0; i < (int)p->PlayerBuff.size(); i++) {
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
    printf("[%s] 意图: %s\n",e->enemydata.name,e->curr_move.name);
    int bar_width = 20;
    int filled = (e->health * bar_width) / (e->enemydata.Max_health > 0 ? e->enemydata.Max_health : 1);
    printf("HP: [");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) printf("█");
        else printf("░");
    }
    printf("] %d/%d  ", e->health, e->enemydata.Max_health);
    printf("| Block: %-2d\n", e->defend);
    printf("Buffs: ");
    int has_buff = 0;
    for (int i = 0; i < (int)e->EnemyBuff.size(); i++) {
        if (e->EnemyBuff[i] > 0) {
            printf("(%s:%d) ", UI_Get_Buff_Name(i), e->EnemyBuff[i]);
            has_buff = 1;
        }
    }
    if (!has_buff) printf("None");
    printf("\n=====================================================\n");
}