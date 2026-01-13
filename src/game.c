#include "../include/game.h"
#include <stdio.h>
#include <string.h>

void game_init_player(PlayerState *player){
    printf("choose your hero:\n");
    printf("1.The Ironclad 2.The Slient 3.The defect 4.The Watcher\n");
    scanf("%d",&player->hero);
    switch (player->hero){
    case Ironclad:
        player->max_health=Ironclad_MAX_Health;
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
    player->health=player->max_health;
    player->defend=0;
    player->MaxCoat=3;
    player->CurrCost=3;
}
void game_init_enemy(EnemyState *enemy){
    
}