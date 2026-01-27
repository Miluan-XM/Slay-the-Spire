#include "../include/CardEffect.h"
#include <stdio.h>

//效果实现函数

void Effect_Resolver(PlayerState *mc, EnemyState *enemy, CardEffect effect, int value,int cost);

void Resolver_Defend(int value, PlayerState *mc);

void Resolver_ATTACK(int value, PlayerState *mc, EnemyState *enemy);

void Effect_Resolver(PlayerState *mc,EnemyState *enemy,CardEffect effect,int value,int cost){
    //判断是否buff
    if(effect>1000||effect<2000){
        int buff=effect-EFFECT_ENUM_NUM;
        mc->PlayerBuff[buff]=mc->PlayerBuff[buff]+value;
    }else if(effect>2000){
                int buff=effect-EFFECT_ENUM_NUM;
                enemy->EnemyBuff[buff]=enemy->EnemyBuff[buff]+value;
    }else {
        switch (effect){
            case Effect_ATTACK:
                Resolver_ATTACK(value, mc, enemy);
                break;
            case Effect_Defend:
                Resolver_Defend(value, mc);
            case Effect_Card_Draw:
                //需要抽卡



        }
    }

    //花费
    mc->CurrCost=mc->CurrCost-cost;
    //出牌后所需检测
    //坚不可摧
    //余像
    //退出平静 
}

void Resolver_Defend(int value, PlayerState *mc)
{
    int Defence = value;
    // 敏捷
    Defence = Defence - mc->PlayerBuff[Buff_DEXTERITY];
    // 脆弱
    if (mc->PlayerBuff[Debuff_FRAIL] > 0)
    {
        Defence = Defence / 4;
    }
    mc->defend = mc->defend + Defence;
    printf("获得%d点格挡\n",Defence);
}

void Resolver_ATTACK(int value, PlayerState *mc, EnemyState *enemy)
{
    int final_attack = value;
    // 力量判断
    final_attack = final_attack + mc->PlayerBuff[Buff_STRENGTH];

    // 虚弱判断
    if (mc->PlayerBuff[DeBuff_WEAK] > 0)
    {
        final_attack = final_attack / 4;
    }

    // 易伤判断
    if (enemy->EnemyBuff[DeBuff_VALNERABALE] > 0)
    {
        final_attack = final_attack + final_attack / 4;
    }
    //格挡
    final_attack=final_attack-enemy->defend;
    enemy->health = enemy->health - final_attack;
    printf("造成%d点伤害\n",final_attack);
    // 荆棘
    mc->health = mc->health - enemy->EnemyBuff[Buff_THORNS];
}
