#include "../include/Game.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Game.hpp"

/**
 * @brief 玩家初始化：选择职业、设置血量、初始化基础Buff向量和战斗上下文
 */
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

/**
 * @brief 为铁甲战士填充初始牌组：5张打击(1)、4张防御(2)、1张痛击(3)，并设置起始金币90
 */
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

/**
 * @brief 按ID从卡牌库查询卡牌（ID从1开始），越界时打印错误并返回默认值
 */
CardState Card_Searcher(int id,CardLibrary *lib){
    if (id < 1 || id > (int)lib->CardDataArray.size()) {
        printf("Error: Card id %d out of range (size: %zu)\n", id, lib->CardDataArray.size());
        return CardState();
    }
    return lib->CardDataArray[id-1];
}

/**
 * @brief 按ID从招式库查询招式（ID从501开始），越界时打印错误并返回默认值
 */
MoveFact Move_Searcher(int id,MoveLibrary *lib){
    if (id < 501 || id > 501 + (int)lib->movedata.size()) {
        printf("Error: Move id %d out of range\n", id);
        return MoveFact();
    }
    return lib->movedata[id-501];
}

/**
 * @brief 按ID从敌人库查询敌人蓝图（ID从1开始），越界时打印错误并返回默认值
 */
EnemyData Enemy_Searcher(int id,EnemyLibrary *lib){
    if (id < 1 || id > (int)lib->enemystate.size()) {
        printf("Error: Enemy id %d out of range (size: %zu)\n", id, lib->enemystate.size());
        return EnemyData();
    }
    return lib->enemystate[id-1];
}

/**
 * @brief 生成 [a, b] 闭区间随机整数，自动处理 a>b 的情况
 */
int RandNum_between(int a,int b){
    int max=b;
    int min=a;
    if(a>b){ max=a; min=b; }
    return rand()%(max-min+1)+min;
}

/**
 * @brief Fisher-Yates 洗牌，从数组末尾向前遍历，每次与随机位置的元素交换
 */
void Card_Shuffier(int card[],int len){
    printf("\n");
    for(int i=len-1;i>-1;i--){
        int j=rand()%(i+1);
        swap_value(&card[i],&card[j]);
    }
    printf("Shuffier finished\n");
}

/**
 * @brief 将 Buff 枚举值映射为 UI 显示的简写名称
 * @note STR=力量, DEX=敏捷, THN=荆棘, BAR=壁垒, VUL=易伤, WEK=虚弱, FRL=脆弱, RIT=仪式
 */
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

/**
 * @brief 打印玩家状态面板：HP图形条、格挡值、能量、Buff列表
 */
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

/**
 * @brief 打印敌人状态面板：名字、意图、HP图形条、格挡值、Buff列表
 */
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