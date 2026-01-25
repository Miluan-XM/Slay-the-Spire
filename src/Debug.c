#include <stdio.h>
#include "../include/Debug.h"


//print card array
void print_CardLoaded_array(CardState *card,int len);
//string exput
char * print_enum_to_string_CardEffect(CardEffect effect);
char * print_enum_to_string_CardTYpe(CardType type);


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