#include "../include/game.h"
#include <stdio.h>
#include <string.h>
#include "game.h"

void game_init_player(PlayerState *player){
    
    //卡牌
    memset(&(player->battle_context),0,sizeof(BattleContext));

    //角色选择
    printf("choose your hero:\n");
    printf("1.The Ironclad 2.The Slient 3.The defect 4.The Watcher\n");
    scanf("%d",&player->hero);
    switch (player->hero){
    case Ironclad:
        player->max_health=Ironclad_MAX_Health;
            InitCard_Ironclad(player);
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
 void InitCard_Ironclad(PlayerState *player){
    int i = 0;
    //打击
    for (i = 0; i < 5; i++)
    {
        player->battle_context.master_deck[i] = 1;
        player->battle_context.master_deck_count++;
    }
    // 防御初始
    for (i = 5; i < 9; i++)
    {
        player->battle_context.master_deck[i] = 2;
        player->battle_context.master_deck_count++;
    }
    // 痛击
    player->battle_context.master_deck[9] = 3;
    player->battle_context.master_deck_count++;
    player->gold = 90;
}
