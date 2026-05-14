#include <stdio.h>
#include "../include/Debug.hpp"

/**
 * @brief 以表格形式打印所有已加载卡牌的详细信息（用于启动时调试输出）
 */
void print_CardLoaded_array(CardState *card,int len){
    if (card == nullptr || len <= 0) {
        printf("\n[DEBUG] Card library is empty or NULL.\n");
        return;
    }
        printf("Have loaded data follows:\n");
    int i=0,j=0;
    printf("%-4s | %-12s | %-6s | %-4s | %-20s\n", "ID", "Name", "Type", "Cost", "Effects");
    printf("------------------------------------------------------------------\n");

    for(i=0;i<len;i++){
        printf("%-4d | %-12s | %-10s | %-4d | { ", 
                card[i].id, 
                card[i].name, 
                print_enum_to_string_CardTYpe(card[i].cardtype), 
                card[i].CardCost);
        for(j=0;j<EFFECT_MAX;j++){
            printf("%-20s",print_enum_to_string_CardEffect(card[i].effect_array[j].effect));
        }
        for(j=0;j<EFFECT_MAX;j++){
            printf("%-3d",card[i].effect_array[j].value);
        }
        printf("\n");
    }
    printf("----------------------------");
}

/**
 * @brief 打印招式库中所有招式数据（ID/名字/意图）
 */
void debug_print_move_library(MoveLibrary *lib) {
    if (lib == nullptr || lib->movedata.empty()) {
        printf("\n[DEBUG] Move library is empty.\n");
        return;
    }

    printf("\n==== DEBUG: Move Library (%d entries) ====\n", lib->len);
    printf("%-4s | %-15s | %-25s\n", "ID", "Move Name", "Intentions (v1, v2, v3)");
    printf("------------------------------------------------------------\n");

    for (size_t i = 0; i < lib->movedata.size(); i++) {
        printf("%-4d | %-15s | [%-6s, %-6s, %-6s]\n",
               lib->movedata[i].id,
               lib->movedata[i].name,
               get_intent_name(lib->movedata[i].intention[0]),
               get_intent_name(lib->movedata[i].intention[1]),
               get_intent_name(lib->movedata[i].intention[2]));
    }
    printf("------------------------------------------------------------\n\n");
}

/**
 * @brief 打印敌人库中所有敌人蓝图数据（ID/名字/血量/AI/招式配置表）
 */
void debug_print_enemy_library(EnemyLibrary *lib) {
    if (lib == nullptr || lib->enemystate.empty()) {
        printf("\n[DEBUG] Enemy library is empty.\n");
        return;
    }

    printf("\n==== DEBUG: Enemy Blueprint Library (%d entries) ====\n", lib->len);
    
    for (size_t i = 0; i < lib->enemystate.size(); i++) {
        EnemyData &e = lib->enemystate[i];
        printf("ID: %-3d | Name: %-12s | MaxHP: %-3d| AI_Mode %-3d\n", e.id, e.name, e.Max_health,(int)e.mode);
        printf("Moves Config:\n");
        
        for (int j = 0; j < 7; j++) {
            if (e.move[j][0] == 0) continue;
            
            printf("  Slot %d -> MoveID: %-4d | Values: {%d, %d, %d}\n", 
                   j, e.move[j][0], e.move[j][1], e.move[j][2], e.move[j][3]);
        }
        printf("------------------------------------------------------------\n");
    }
    printf("\n");
}

/**
 * @brief 将CardEffect枚举值映射为字符串（用于调试输出中的效果名称）
 */
char * print_enum_to_string_CardEffect(CardEffect effect){
    switch (effect){
    case 1:
    return "Effect_ATTACK";
    case 2:
    return "Effect_Defend";
    case 3:
    return "Effect_Buff_self";
    case 4:
    return "Effect_Buff_enemy";
    case 5:
    return "Effect_Card_Draw";
    default:
    return "UNKONWN";
    }
}

/**
 * @brief 将CardType枚举值映射为字符串
 */
char * print_enum_to_string_CardTYpe(CardType type){
    switch (type){
    case 1:
    return "ATTACK";
    case 2:
    return "SKILL";
    case 3:
    return "ABILITY";
    case 4:
    return "CURSE";
    default:
    return "UNKONWN";
    }
}

/**
 * @brief 将IntnentType枚举值映射为简写（用于招式表显示）
 */
const char* get_intent_name(IntnentType intent) {
    if (intent == 0) return "-";
    switch(intent) {
        case intent_Attack: return "ATK";
        case intent_Defend: return "DEF";
        case intent_CardGive: return "CRD";
        case intent_selfBuff001: return "S-BUF";
        case intent_selfDeBuff101: return "S-DEB";
        case intent_selfEnemyBuff: return "S-EBUF";
        case intent_giveBuff001: return "G-BUF";
        case intent_giveDeBuff101: return "G-DEB";
        case intent_giveEnemyBuff: return "G-EBUF";
        default: return "UNK";
    }
}