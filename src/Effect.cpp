#include "../include/Effect.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void Resolver_Defend(int *defend,int value,std::vector<int>& selfbuff);
void Resolver_Attack(int value,int *self_health,int *to_health,std::vector<int>& selfbuff,std::vector<int>& tobuff,int *defend);
void Resolver_Buff(std::vector<int>& selfbuff,std::vector<int>& tobuff,int buff_id,int value);
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

    mc->CurrCost=mc->CurrCost-cost;
}

void Move_Resolver(int id,int move_value[3],MoveLibrary *lib,EnemyState *enemy,PlayerState *mc){
    MoveFact curr_move=Move_Searcher(id,lib);
    int i=0;
    for(i=0;i<3&&curr_move.intention[i]!=0;i++){
        int intent=curr_move.intention[i];
        int value=move_value[i];
        if(intent>1000){
            Resolver_Buff(enemy->EnemyBuff,mc->PlayerBuff,intent,value);
        }else {
            switch (intent){
                case intent_Attack:
                if(value>10000){
                    int attack=0,count=0;
                    Num_For_MultiAttack(&attack,&count,value);
                    for(int j=0;j<count;j++){
                        Resolver_Attack(attack,&enemy->health,&mc->health,enemy->EnemyBuff,mc->PlayerBuff,&mc->defend);
                    }
                }else {
                    Resolver_Attack(value,&enemy->health,&mc->health,enemy->EnemyBuff,mc->PlayerBuff,&mc->defend);
                }

                break;
                case intent_Defend:
                Resolver_Defend(&enemy->defend,value,enemy->EnemyBuff);
                break;
            }
        }
    }
}

int *Ai_Resolver(EnemyState *enemy){
    int *result=(int*)calloc(4, sizeof(int));
    int *ai_result=nullptr;
    switch (enemy->enemydata.mode){
    case AI_INTENSIFY_ATTACK:
    ai_result=Resolver_AI_INTENSIFY_ATTACK(enemy->enemydata.move,enemy->turn_count);
    break;
    case AI_RANDOM_ATTACK:
    ai_result=Resolver_AI_RANDOM_ATTACK(enemy->enemydata.move,0,6);
    break;
    case AI_ROUND_TRIGGER:
    ai_result=Resolver_AI_ROUND_TRIGGER(enemy->enemydata.move,enemy->turn_count);
    break;
    case AI_BLOOD:
    ai_result=Resolver_AI_Blood(enemy);
    break;
    default:
        printf("WRONG AI");
        break;
    }
    if(ai_result){
        for(int i=0;i<4;i++) result[i]=ai_result[i];
    }
    return result;
}



void Resolver_Buff(std::vector<int>& selfbuff,std::vector<int>& tobuff,int buff_id,int value){
    if(buff_id>1000&&buff_id<2000){
        int buff=buff_id-SELF_BUFF_ENUM;
        selfbuff[buff]=selfbuff[buff]+value;
    }else if(buff_id>2000){
                int buff=buff_id-TO_BUFF_ENUM;
                tobuff[buff]=tobuff[buff]+value;
        }
       
}
void Resolver_Attack(int value,int *self_health,int *to_health,std::vector<int>& selfbuff,std::vector<int>& tobuff,int *defend){
    int attack=value;
    int strong=selfbuff[Buff_STRENGTH];
    int thros=tobuff[Buff_THORNS];
    attack=attack+strong;
    if(selfbuff[DeBuff_WEAK]){
        attack=attack-attack/4;
    }
    if(tobuff[DeBuff_VALNERABALE]){
        attack=attack+attack/2;
    }
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
    if(thros>0){
        *self_health=*self_health-thros;
    }
}
void Resolver_Defend(int *defend,int value,std::vector<int>& selfbuff){

    int defence=value;
    int frail=selfbuff[Debuff_FRAIL];
    if(frail>0){
        defence=defence/2;
    }
    int dex=selfbuff[Buff_DEXTERITY];
    defence=defence+dex;
    *defend=*defend+defence;
    printf("获得%d点格挡\n",defence);
}
int *Resolver_AI_INTENSIFY_ATTACK(std::vector<std::vector<int>>& move, int count){
    if(count==1){
        return move[0].data();
    }
    for(int i=1;i<7;i++){
        if(move[i][0]!=0){
            return move[i].data();
        }
    }
    return move[0].data();
}

int *Resolver_AI_RANDOM_ATTACK(std::vector<std::vector<int>>& move, int begin, int final){
    int j=RandNum_between(begin,final);
    while(move[j][0]==0){
        j=RandNum_between(begin,final);
    }
    return move[j].data();
}
int *Resolver_AI_ROUND_TRIGGER(std::vector<std::vector<int>>& move, int count){
    if(count<=7){
        return move[count-1].data();
    }else {
        return move[count%7-1].data();
    }
}
int *Resolver_AI_Blood(EnemyState *enemy){
    int cri=enemy->enemydata.critical;
    int curr=enemy->health;
    if(curr>cri){
        return Resolver_AI_RANDOM_ATTACK(enemy->enemydata.move,1,6);
    }else {
        return enemy->enemydata.move[0].data();
    }
}


int *NumBreak(int num,int rate){
    int *a=(int *)calloc(5, sizeof(int));
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
    free(num);
}

void describe_card(CardState &card){
    printf("[");
    for(int i=0;i<EFFECT_MAX&&card.effect_array[i].effect!=0;i++){
        if(i>0) printf(" + ");
        int e=card.effect_array[i].effect;
        int v=card.effect_array[i].value;
        if(e==Effect_ATTACK){
            if(v>10000){
                int a=0,b=0;
                Num_For_MultiAttack(&a,&b,v);
                printf("多段攻击 %d×%d",a,b);
            }else{
                printf("攻击 %d",v);
            }
        }else if(e==Effect_Defend){
            printf("防御 %d",v);
        }else if(e==Effect_Card_Draw){
            printf("抽%d张牌",v);
        }else if(e>1000&&e<2000){
            printf("自增益[%d]",e-1000);
        }else if(e>2000){
            printf("敌减益[%d]",e-2000);
        }else{
            printf("效果[%d:%d]",e,v);
        }
    }
    printf("]\n");
}