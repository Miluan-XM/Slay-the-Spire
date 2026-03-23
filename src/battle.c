#include "../include/battle.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "battle.h"


void battle_init_player(PlayerState *player);

EnemyState battle_init_enemy(int enemy_id,EnemyLibrary *lib);

void Turn_Player(PlayerState * mc,CardLibrary *lib);

void Turn_Enemy(EnemyState *enemy,PlayerState *mc);

int Card_Drawer(PlayerState *mc, int i,CardLibrary *lib);

void Card_FindAndPrinter(int id,CardLibrary *lib);
void Turn_Buff_Changer(int buff[STAUS_MAX_COUNT]);

void swap_value(int *a,int *b){
    int temp=*b;
    *b=*a;
    *a=temp;
}
void battle_init_player(PlayerState *player){
    //清空
    memset(&player->battle_context,0,sizeof(BattleContext));
    //主要还是卡牌
    int i=0;
    //复制卡组
    for(i=0;i<player->master_card.master_count;i++){
        player->battle_context.draw_pile[i]=player->master_card.master[i];
        player->battle_context.draw_pile_count++;
    }
    if(player->battle_context.draw_pile==NULL){
        perror("Copy Error");
    }
    //打乱
    Card_Shuffier(player->battle_context.draw_pile,player->battle_context.draw_pile_count);
    //抽卡还是别在这里写了

}

EnemyState battle_init_enemy(int enemy_id,EnemyLibrary *lib){
    EnemyState enemy;
    memset(&enemy,0,sizeof(EnemyState));
    enemy.enemydata=Enemy_Searcher(enemy_id,lib);
    enemy.health=enemy.enemydata.Max_health;
    enemy.max_health=enemy.enemydata.Max_health;
    enemy.turn_count=1;
    return enemy;
}

//玩家回合
void Turn_Player(PlayerState * mc,CardLibrary *lib){
    
    //格挡清零，壁垒判断
    if(mc->PlayerBuff[Buff_BARRIER]==0){
        mc->defend=0;
    }
    //需要减少的buff们，要专门的函数
    Turn_Buff_Changer(mc->PlayerBuff);
    //抽牌，抽五张
    int i=0;
    //每次抽牌到十张为止
    for(i=0;i<5;i++){
        if ( Card_Drawer(mc, i,lib)==0){
              break;
        }
    }
    //手牌展示
    for(i=0;i<mc->battle_context.hand_count;i++){
        printf("%d",i);
        Card_FindAndPrinter(mc->battle_context.hand[i],lib);
    }
    

}


int Card_Drawer(PlayerState *mc, int i,CardLibrary *lib){

    if (mc->battle_context.hand_count == 10){
        printf("我的手牌满了\n");
        {
            return 0;
        }
    }
    mc->battle_context.hand[i] = mc->battle_context.draw_pile[mc->battle_context.draw_pile_count - 1];
    mc->battle_context.draw_pile[mc->battle_context.draw_pile_count - 1]=0;
    Card_FindAndPrinter(mc->battle_context.hand[i],lib);
    mc->battle_context.hand_count++;
    mc->battle_context.draw_pile_count--;
    return 1;
}
void Card_FindAndPrinter(int id,CardLibrary *lib){
    CardState curr_card=Card_Searcher(id,lib);
    printf(" %s\n",curr_card.name);
}
//怪物办事
void Turn_Enemy(EnemyState *enemy,PlayerState *mc){
    if(enemy->EnemyBuff[Buff_BARRIER]==0){
        enemy->defend=0;
    }
    Turn_Buff_Changer(enemy->EnemyBuff);
    
}

//战斗结束
void Battle_Close(PlayerState *player,CardLibrary *lib){
    int i=0;
    printf("选择一张牌");
    //战斗后随机牌
    for (i=0;i<2;i++){
        int j=rand()%(lib->len);
        CardState give_card=lib->CardDataArray[j];
        printf("%d.%s\n",(i+1),give_card.name);
    }
}

void Turn_Buff_Changer(int buff[STAUS_MAX_COUNT]){
    if(buff[DeBuff_VALNERABALE]>0){
        buff[DeBuff_VALNERABALE]--;
    }
    if(buff[DeBuff_WEAK]>0){
        buff[DeBuff_WEAK]--;
    }
    if(buff[Debuff_FRAIL]>0){
        buff[Debuff_FRAIL]--;
    }
}