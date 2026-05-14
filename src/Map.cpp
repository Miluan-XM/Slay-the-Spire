#include "../include/Game.hpp"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief 将指定ID的卡牌加入玩家牌组末尾，并打印卡牌名字
 */
void add_card_to_deck(PlayerState *player, int card_id, CardLibrary *lib){
    player->master_card.master[player->master_card.master_count]=card_id;
    player->master_card.master_count++;
    CardState cs=Card_Searcher(card_id,lib);
    printf("获得新卡: %s\n",cs.name);
}

/**
 * @brief 根据层数随机选择敌人ID：第1层1~2号，第2层1~3号，第3层全部
 * @param floor 当前层数
 * @param lib 敌人库指针（用于获取最大ID）
 * @return 敌人ID（从1开始）
 */
int pick_random_enemy_id(int floor, EnemyLibrary *lib){
    int max_id=lib->len;
    int min_id=1;
    if(floor==2&&max_id>2) max_id=3;
    else if(floor>=3) max_id=lib->len;
    int id=RandNum_between(min_id,max_id);
    return id;
}

/**
 * @brief 为当前层生成3个随机事件节点（战斗50%/宝箱15%/神秘15%/篝火20%）
 * @param run 运行状态指针（输出，path被填充）
 * @param floor 当前层数
 * @param enemy_lib 敌人库指针（用于战斗事件随机选敌）
 */
void generate_floor_nodes(RunState *run, int floor, EnemyLibrary *enemy_lib){
    run->floor=floor;
    run->step=0;
    for(int i=0;i<3;i++){
        MapNode &n=run->path[i];
        int r=RandNum_between(1,100);
        if(r<=50){
            n.type=EVENT_ENEMY;
            n.param=pick_random_enemy_id(floor,enemy_lib);
            sprintf(n.title,"战斗");
            sprintf(n.desc,"遭遇敌人！");
        }else if(r<=65){
            n.type=EVENT_TREASURE;
            n.param=30+floor*10;
            sprintf(n.title,"宝箱");
            sprintf(n.desc,"发现宝箱！获得%d金币和一张随机卡牌",n.param);
        }else if(r<=80){
            n.type=EVENT_MYSTERY;
            n.param=0;
            sprintf(n.title,"?");
            sprintf(n.desc,"未知事件");
        }else{
            n.type=EVENT_REST;
            n.param=30;
            sprintf(n.title,"篝火");
            sprintf(n.desc,"休息，恢复30%%生命值");
        }
    }
}

/**
 * @brief 生成Boss事件（当前未使用，Boss直接在main中处理）
 */
EventType generate_boss_event(EnemyLibrary *enemy_lib, int &enemy_id){
    enemy_id=enemy_lib->len;
    return EVENT_BOSS;
}

/**
 * @brief 显示当前步的两个可选事件（主选项+备选），备选随机生成
 */
void show_node_choices(RunState *run, EnemyLibrary *enemy_lib){
    MapNode &a=run->path[run->step];
    MapNode b;
    int r=RandNum_between(1,100);
    if(r<=50){
        b.type=EVENT_ENEMY;
        b.param=pick_random_enemy_id(run->floor,enemy_lib);
        sprintf(b.title,"战斗");
        sprintf(b.desc,"遭遇敌人！");
    }else if(r<=65){
        b.type=EVENT_TREASURE;
        b.param=30+run->floor*10;
        sprintf(b.title,"宝箱");
        sprintf(b.desc,"发现宝箱！获得%d金币",b.param);
    }else if(r<=80){
        b.type=EVENT_MYSTERY;
        sprintf(b.title,"?");
        sprintf(b.desc,"未知事件");
    }else{
        b.type=EVENT_REST;
        b.param=30;
        sprintf(b.title,"篝火");
        sprintf(b.desc,"休息，恢复30%%生命值");
    }

    printf("\n===== 第%d层 第%d步 =====\n",run->floor,run->step+1);
    printf("1. [%s] %s\n",a.title,a.desc);
    printf("2. [%s] %s\n",b.title,b.desc);
    printf("请选择 (1 或 2): ");
}

/**
 * @brief 等待玩家输入1或2，过滤非法字符
 * @return 1 或 2
 */
int choose_option(){
    int c;
    while(1){
        if(scanf("%d",&c)!=1){
            while(getchar()!='\n');
            printf("重新输入: ");
            continue;
        }
        if(c==1||c==2) return c;
        printf("输入1或2: ");
    }
}

/**
 * @brief 宝箱事件：获得金币 + 卡牌三选一奖励
 */
void handle_treasure(PlayerState *player, CardLibrary *lib, int gold_amt){
    player->gold+=gold_amt;
    printf("获得%d金币！当前金币: %d\n",gold_amt,player->gold);
    battle_reward(player,lib);
}

/**
 * @brief 未知事件：随机触发以下之一——回血(30%)/金币(25%)/受伤(20%)/得卡(15%)/无事(10%)
 */
void handle_mystery(PlayerState *player, CardLibrary *lib){
    int r=RandNum_between(1,100);
    if(r<=30){
        int heal=RandNum_between(10,25);
        player->health+=heal;
        if(player->health>player->max_health) player->health=player->max_health;
        printf("神秘事件：发现清泉！恢复%d生命值 (%d/%d)\n",heal,player->health,player->max_health);
    }else if(r<=55){
        int gold=RandNum_between(20,50);
        player->gold+=gold;
        printf("神秘事件：捡到钱袋！获得%d金币 (%d)\n",gold,player->gold);
    }else if(r<=75){
        int dmg=RandNum_between(5,15);
        player->health-=dmg;
        if(player->health<=0) player->health=1;
        printf("神秘事件：触发陷阱！受到%d伤害 (%d/%d)\n",dmg,player->health,player->max_health);
    }else if(r<=90){
        printf("神秘事件：获得一张随机卡牌！\n");
        battle_reward(player,lib);
    }else{
        printf("神秘事件：什么也没有发生...\n");
    }
}

/**
 * @brief 篝火事件：按最大生命值的百分比恢复
 * @param pct 恢复百分比（如30=恢复30%）
 */
void handle_rest(PlayerState *player, int pct){
    int heal=player->max_health*pct/100;
    player->health+=heal;
    if(player->health>player->max_health) player->health=player->max_health;
    printf("在篝火旁休息，恢复%d生命值 (%d/%d)\n",heal,player->health,player->max_health);
}