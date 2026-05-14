#include "../include/Battle.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Battle.hpp"

/**
 * @brief 交换两个整数的值（辅助函数）
 */
void swap_value(int *a,int *b){
    int temp=*b;
    *b=*a;
    *a=temp;
}

/**
 * @brief 战斗初始化：重置BattleContext，将master_card复制到draw_pile，洗牌
 */
void battle_init_player(PlayerState *player){
    player->battle_context = BattleContext();
    
    int i=0;
    player->battle_context.draw_pile.assign(100, 0);
    player->battle_context.hand.assign(10, 0);
    player->battle_context.discard_pile.assign(100, 0);
    player->battle_context.exhaust_pile.assign(100, 0);
    
    for(i=0;i<player->master_card.master_count;i++){
        player->battle_context.draw_pile[i]=player->master_card.master[i];
        player->battle_context.draw_pile_count++;
    }
    if(player->battle_context.draw_pile.empty()){
        perror("Copy Error");
    }
    Card_Shuffier(player->battle_context.draw_pile.data(),player->battle_context.draw_pile_count);
}

/**
 * @brief 按敌人ID创建战斗实例，读取蓝图数据并初始化血量/Buff/回合计数器
 */
EnemyState battle_init_enemy(int enemy_id,EnemyLibrary *lib){
    EnemyState enemy;
    enemy = EnemyState();
    enemy.enemydata=Enemy_Searcher(enemy_id,lib);
    enemy.health=enemy.enemydata.Max_health;
    enemy.max_health=enemy.enemydata.Max_health;
    enemy.turn_count=1;
    return enemy;
}

/**
 * @brief 玩家回合：重置能量→壁垒判断→减Buff→弃手牌→抽5张→展示手牌
 */
void Turn_Player(PlayerState * mc,CardLibrary *lib){
    
    mc->CurrCost = mc->MaxCoat;
    if(mc->PlayerBuff[Buff_BARRIER]==0){
        mc->defend=0;
    }
    Turn_Buff_Changer(mc->PlayerBuff);
    for(int i=0;i<mc->battle_context.hand_count;i++){
        if(mc->battle_context.hand[i]!=0){
            mc->battle_context.discard_pile[mc->battle_context.discard_pile_count++]=mc->battle_context.hand[i];
        }
    }
    mc->battle_context.hand.assign(10,0);
    mc->battle_context.hand_count=0;
    int i=0;
    for(i=0;i<5;i++){
        if ( Card_Drawer(mc, i,lib)==0){
              break;
        }
    }
    for(i=0;i<mc->battle_context.hand_count;i++){
        printf("%d",i);
        Card_FindAndPrinter(mc->battle_context.hand[i],lib);
    }
}

/**
 * @brief 抽牌：从draw_pile栈顶取牌放入hand[i]
 * @note draw_pile为空时自动将discard_pile shuffle接入
 * @return 1=成功，0=手牌满(10张)
 */
int Card_Drawer(PlayerState *mc, int i,CardLibrary *lib){

    if (mc->battle_context.hand_count == 10){
        printf("我的手牌满了\n");
        {
            return 0;
        }
    }
    if(mc->battle_context.draw_pile_count==0){
        for(int j=0;j<mc->battle_context.discard_pile_count;j++){
            mc->battle_context.draw_pile[j]=mc->battle_context.discard_pile[j];
        }
        mc->battle_context.draw_pile_count=mc->battle_context.discard_pile_count;
        mc->battle_context.discard_pile_count=0;
        Card_Shuffier(mc->battle_context.draw_pile.data(),mc->battle_context.draw_pile_count);
    }
    mc->battle_context.hand[i] = mc->battle_context.draw_pile[mc->battle_context.draw_pile_count - 1];
    mc->battle_context.draw_pile[mc->battle_context.draw_pile_count - 1]=0;
    Card_FindAndPrinter(mc->battle_context.hand[i],lib);
    mc->battle_context.hand_count++;
    mc->battle_context.draw_pile_count--;
    return 1;
}

/**
 * @brief 查询并打印卡牌名字
 */
void Card_FindAndPrinter(int id,CardLibrary *lib){
    CardState curr_card=Card_Searcher(id,lib);
    printf(" %s\n",curr_card.name);
}

/**
 * @brief 出牌：将hand[index]移入弃牌堆→左移填补空缺→重显剩余手牌
 * @return 1=成功，0=无效索引或卡牌ID=0
 */
int play_card_from_hand(PlayerState *mc, int index, CardLibrary *lib){
    if(index<0||index>=mc->battle_context.hand_count) return 0;
    int card_id=mc->battle_context.hand[index];
    if(card_id==0) return 0;
    mc->battle_context.discard_pile[mc->battle_context.discard_pile_count++]=card_id;
    for(int i=index;i<mc->battle_context.hand_count-1;i++){
        mc->battle_context.hand[i]=mc->battle_context.hand[i+1];
    }
    mc->battle_context.hand[mc->battle_context.hand_count-1]=0;
    mc->battle_context.hand_count--;
    printf("当前手牌:\n");
    for(int i=0;i<mc->battle_context.hand_count;i++){
        printf("%d",i);
        Card_FindAndPrinter(mc->battle_context.hand[i],lib);
    }
    return 1;
}

/**
 * @brief 敌人回合：格挡清零(无壁垒)→减Buff→仪式转化为力量→回合计数+1
 */
void Turn_Enemy(EnemyState *enemy,PlayerState *mc){
    if(enemy->EnemyBuff[Buff_BARRIER]==0){
        enemy->defend=0;
    }
    Turn_Buff_Changer(enemy->EnemyBuff);
    if(enemy->EnemyBuff[EnemyBuff_RITUAL]>0){
        enemy->EnemyBuff[Buff_STRENGTH]+=enemy->EnemyBuff[EnemyBuff_RITUAL];
    }
    enemy->turn_count++;
}

/**
 * @brief 战斗奖励：展示3张随机卡牌，玩家选1张加入牌组
 */
void battle_reward(PlayerState *player,CardLibrary *lib){
    if(lib==nullptr||lib->len<=0) return;
    int picks[3];
    for(int i=0;i<3;i++){
        int j=RandNum_between(0,lib->len-1);
        while(j==picks[0]||(i>1&&j==picks[1])) j=RandNum_between(0,lib->len-1);
        picks[i]=lib->CardDataArray[j].id;
    }
    printf("\n======== 选择一张卡牌 ========\n");
    for(int i=0;i<3;i++){
        CardState cs=Card_Searcher(picks[i],lib);
        printf("%d. %s (费用:%d)\n",i+1,cs.name,cs.CardCost);
    }
    printf("请输入 1-3: ");
    int c;
    while(1){
        if(scanf("%d",&c)!=1){
            while(getchar()!='\n');
            continue;
        }
        if(c>=1&&c<=3){
            add_card_to_deck(player,picks[c-1],lib);
            break;
        }
    }
}

/**
 * @brief 战斗胜利：发放15-30金币 + 卡牌三选一奖励
 */
void Battle_Close(PlayerState *player,CardLibrary *lib){
    int gold=RandNum_between(15,30);
    player->gold+=gold;
    printf("\n===== 战斗胜利！=====\n");
    printf("获得%d金币 (当前:%d)\n",gold,player->gold);
    battle_reward(player,lib);
}

/**
 * @brief 减益Buff层数衰减：易伤/虚弱/脆弱各减1层
 */
void Turn_Buff_Changer(std::vector<int>& buff){
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