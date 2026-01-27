#include <stdio.h>
#include "../include/Debug.h"


//print card array
void print_CardLoaded_array(CardState *card,int len);
void debug_print_move_library(MoveLibrary *lib);
void debug_print_enemy_library(EnemyLibrary *lib);
//string exput
char * print_enum_to_string_CardEffect(CardEffect effect);
char * print_enum_to_string_CardTYpe(CardType type);
const char* get_intent_name(IntnentType intent);



void print_CardLoaded_array(CardState *card,int len){
    if (card == NULL || len <= 0) {
        printf("\n[DEBUG] Card library is empty or NULL.\n");
        return;
    }
        printf("Have loaded data follows:\n");
    int i=0,j=0;
        // 打印表头，使用左对齐以对齐内容
    printf("%-4s | %-12s | %-6s | %-4s | %-20s\n", "ID", "Name", "Type", "Cost", "Effects");
    printf("------------------------------------------------------------------\n");

    for(i=0;i<len;i++){
        // 1. 打印基本属性
        // %-12s 表示左对齐，占12个字符位
        printf("%-4d | %-12s | %-10s | %-4d | { ", 
                card[i].id, 
                card[i].name, 
                print_enum_to_string_CardTYpe(card[i].cardtype), 
                card[i].CardCost);
        //effect print
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

void debug_print_move_library(MoveLibrary *lib) {
    if (lib == NULL || lib->movedata == NULL) {
        printf("\n[DEBUG] Move library is empty.\n");
        return;
    }

    printf("\n==== DEBUG: Move Library (%d entries) ====\n", lib->len);
    printf("%-4s | %-15s | %-25s\n", "ID", "Move Name", "Intentions (v1, v2, v3)");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < lib->len; i++) {
        printf("%-4d | %-15s | [%-6s, %-6s, %-6s]\n",
               lib->movedata[i].id,
               lib->movedata[i].name,
               get_intent_name(lib->movedata[i].intention[0]),
               get_intent_name(lib->movedata[i].intention[1]),
               get_intent_name(lib->movedata[i].intention[2]));
    }
    printf("------------------------------------------------------------\n\n");
}


void debug_print_enemy_library(EnemyLibrary *lib) {
    if (lib == NULL || lib->enemystate == NULL) {
        printf("\n[DEBUG] Enemy library is empty.\n");
        return;
    }

    printf("\n==== DEBUG: Enemy Blueprint Library (%d entries) ====\n", lib->len);
    
    for (int i = 0; i < lib->len; i++) {
        EnemyData *e = &lib->enemystate[i];
        printf("ID: %-3d | Name: %-12s | MaxHP: %-3d\n", e->id, e->name, e->Max_health);
        printf("Moves Config:\n");
        
        // 打印该怪物的 7 个招式位
        for (int j = 0; j < 7; j++) {
            if (e->move[j][0] == 0) continue; // 跳过空招式
            
            printf("  Slot %d -> MoveID: %-4d | Values: {%d, %d, %d}\n", 
                   j, e->move[j][0], e->move[j][1], e->move[j][2], e->move[j][3]);
        }
        printf("------------------------------------------------------------\n");
    }
    printf("\n");
}



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