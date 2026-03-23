#include "../include/Effect.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//效果实现函数
void Resolver_Defend(int *defend,int value,int selfbbuff[STAUS_MAX_COUNT]);
void Resolver_Attack(int value,int *self_health,int *to_health,int selfbuff[STAUS_MAX_COUNT],int tobuff[STAUS_MAX_COUNT],int *defend);
void Resolver_Buff(int selfbuff[STAUS_MAX_COUNT],int tobuff[STAUS_MAX_COUNT],int buff_id,int value);
//处理函数
void Card_Resolver(PlayerState *mc,EnemyState *enemy,CardLibrary *lib,int id,int cost);
void Move_Resolver(int id,int move_value[3],MoveLibrary *lib,EnemyState *enemy,PlayerState *mc);



void Card_Resolver(PlayerState *mc,EnemyState *enemy,CardLibrary *lib,int id,int cost){
    if(cost>mc->CurrCost){
        printf("能量不足\n");
        return ;
    }
    CardState card=Card_Searcher(id,lib);
    int i=0;
    for(i=0;i<5&&card.effect_array[i].effect!=0;i++){
        int effect=card.effect_array[i].effect;
        int value=card.effect_array[i].value;
        if(effect>1000){
            Resolver_Buff(mc->PlayerBuff,enemy->EnemyBuff,effect,value);
        }else {
            switch (effect){
                case Effect_ATTACK:
                    if(value>10000){
                    //多段攻击，2，3位是伤害，4，5是段数
                    int attack=0,count=0;
                    Num_For_MultiAttack(&attack,&count,value);
                    for(int j=0;j<count;j++){
                        Resolver_Attack(attack,&mc->health,&enemy->health,mc->PlayerBuff,enemy->EnemyBuff,&enemy->defend);
                    }
                    }else {
                    Resolver_Attack(value,&mc->health,&enemy->health,mc->PlayerBuff,enemy->EnemyBuff,&enemy->defend);
                    }
                break;
                case Effect_Defend:
                Resolver_Defend(&mc->defend,value,mc->PlayerBuff);
                break;
            }
        }
    }

    //花费
    mc->CurrCost=mc->CurrCost-cost;
    //出牌后所需检测
    //坚不可摧
    //余像
    //退出平静 
}

void Move_Resolver(int id,int move_value[3],MoveLibrary *lib,EnemyState *enemy,PlayerState *mc){
    MoveFact curr_move=Move_Searcher(id,lib);
    int i=0;
    for(i=0;i<3;i++&&curr_move.intention[i]!=0){
        //循环3次，实现所有存在的效果
        int intent=curr_move.intention[i];
        int value=move_value[i];
        if(intent>1000){
            Resolver_Buff(enemy->EnemyBuff,mc->PlayerBuff,intent,value);
        }else {
            switch (intent){
                //或许应该把这些函数整合到一起
                case intent_Attack:
                if(value>10000){
                    //多段攻击，2，3位是伤害，4，5是段数
                    int attack=0,count=0;
                    Num_For_MultiAttack(&attack,&count,value);
                    for(int j=0;j<count;j++){
                        Resolver_Attack(attack,&enemy->health,&mc->health,enemy->EnemyBuff,mc->PlayerBuff,&enemy->defend);
                    }
                }else {
                    Resolver_Attack(value,&enemy->health,&mc->health,enemy->EnemyBuff,mc->PlayerBuff,&enemy->defend);
                }

                break;
                case intent_Defend:
                Resolver_Defend(&enemy->defend,value,enemy->EnemyBuff);
                break;
            }
        }
    }
}
/// @brief 怪物ai处理函数
/// @param enemy 敌人数据
/// @return 
int *Ai_Resolver(EnemyState *enemy){
    int mode=enemy->enemydata.mode;
    int count=enemy->turn_count;
    int *result=(int*)calloc(sizeof(int),4);
    int i=0;
    switch (mode){
    case AI_INTENSIFY_ATTACK:
    result=Resolver_AI_INTENSIFY_ATTACK(enemy->enemydata.move,enemy->turn_count);
    break;
    case AI_RANDOM_ATTACK:
    result=Resolver_AI_RANDOM_ATTACK(enemy->enemydata.move,0,6);
    break;
    case AI_ROUND_TRIGGER:
    result=Resolver_AI_ROUND_TRIGGER(enemy->enemydata.move,enemy->turn_count);
    break;
    case AI_BLOOD:
    result=Resolver_AI_Blood(enemy);
    break;
    default:
    printf("WRONG AI");
        break;
    }
    return result;
}




//buff处理函数，左边是自己，右边是对方
void Resolver_Buff(int selfbuff[STAUS_MAX_COUNT],int tobuff[STAUS_MAX_COUNT],int buff_id,int value){
    if(buff_id>1000||buff_id<2000){
        int buff=buff_id-SELF_BUFF_ENUM;
        selfbuff[buff]=selfbuff[buff]+value;
    }else if(buff_id>2000){
                int buff=buff_id-TO_BUFF_ENUM;
                tobuff[buff]=tobuff[buff]+value;
        }
       
}
void Resolver_Attack(int value,int *self_health,int *to_health,int selfbuff[STAUS_MAX_COUNT],int tobuff[STAUS_MAX_COUNT],int *defend){
    int attack=value;
    int strong=selfbuff[Buff_STRENGTH];
    int thros=tobuff[Buff_THORNS];
    //力量
        attack=attack+strong;
    //虚弱
    if(selfbuff[DeBuff_WEAK]){
        attack=attack-attack/4;
    }
    //易伤
    if(tobuff[DeBuff_VALNERABALE]){
        attack=attack+attack/2;
    }
    //格挡
    int temp=attack-*defend;
    if(temp>0){
        *to_health=*to_health-temp;
        *defend=0;
        printf("造成%d点伤害\n",temp);
    }else {
        *defend=*defend-attack;
        printf("格挡！\n");
    }

    if(*defend<0){
        *defend=0;
    }
    //荆棘
    if(thros>0){
        *self_health=*self_health-thros;
    }
}
void Resolver_Defend(int *defend,int value,int selfbbuff[STAUS_MAX_COUNT]){

    int defence=value;
    //脆弱
    int frail=selfbbuff[Debuff_FRAIL];
    if(frail>0){
        defence=defence/2;
    }
    int dex=selfbbuff[Buff_DEXTERITY];
    defence=defence+dex;
    *defend=*defend+defence;
    printf("获得%d点格挡\n",defence);
}
int *Resolver_AI_INTENSIFY_ATTACK(int move[7][4],int count){
    if(count==1){
        return move[0];
    }else{
        int j=RandNum_between(1,6);
        for(j;j<7;j++){
                if(move[j][0]!=0){
                    return move[j];
                }
        }

    }
}

int *Resolver_AI_RANDOM_ATTACK(int move[7][4],int begin,int final){
    int j=RandNum_between(begin,final);
    while(move[j][0]==0){
        j=RandNum_between(begin,final);
    }
    return move[j];
}
int *Resolver_AI_ROUND_TRIGGER(int move[7][4],int count){
    if(count<=7){
        return move[count-1];
    }else {
        return move[(count%7)-1];
    }
}
int *Resolver_AI_Blood(EnemyState *enemy){
    int cri=enemy->enemydata.critical;
    int curr=enemy->health;
    if(curr>cri){
        return Resolver_AI_RANDOM_ATTACK(enemy->enemydata.move,1,6);
    }else {
        return enemy->enemydata.move[0];
    }
}


int *NumBreak(int num,int rate){
    int *a=(int *)calloc(sizeof(int),5);
    int n=pow(10,(rate-1));
    printf("nfor "" =%d",n);
    int i=0;
    while(n>0){
        a[i]=num%n;
        num=num/n;
        n=n/10;
        i++;
    }
    return a;
}
void Num_For_MultiAttack(int *a,int *b,int value){
    int *num=NumBreak(value,5);
    *a=10*num[1]+num[2];
    *b=10*num[3]+num[4];

}