#include "../include/Effect.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief 卡牌效果总解析器：遍历卡牌的5个效果槽，调用底层Resolver执行
 * @param mc 玩家状态指针（输出，血量/格挡/Buff被修改）
 * @param enemy 敌人状态指针（输出，血量/格挡/Buff被修改）
 * @param lib 卡牌库指针（用于按ID查询卡牌数据）
 * @param id 正在释放的卡牌ID
 * @param cost 该牌消耗的能量（检查CurrCost足够才执行）
 * @note effect>1000走Buff分支，否则按Effect_ATTACK/Defend/Card_Draw处理
 */
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

/**
 * @brief 怪物招式解析器：遍历3个意图槽，调用底层Resolver执行
 * @param id 招式ID
 * @param move_value EnemyData.move中填写的3个数值
 * @param lib 招式库指针
 * @param enemy 施法者（怪物）指针（输出）
 * @param mc 目标（玩家）指针（输出）
 * @note 与Card_Resolver结构对称，但意图枚举含Attack/Defend/Buff
 */
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

/**
 * @brief 怪物AI总入口：根据AI模式调用对应分支函数，将结果4个整数拷入新分配内存后返回
 * @param enemy 敌人状态指针
 * @return int* 4元素数组指针 [移动ID, value1, value2, value3]（调用者需free）
 */
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

/**
 * @brief Buff/DeBuff 结算器
 * @param selfbuff 施法者的Buff数组（引用，被修改）
 * @param tobuff 目标的Buff数组（引用，被修改）
 * @param buff_id Buff枚举ID（1001~1999=自Buff，2000+=目标Buff）
 * @param value 叠加层数
 * @note buff_id减去SELF_BUFF_ENUM(1000)或TO_BUFF_ENUM(2000)得到实际数组下标
 */
void Resolver_Buff(std::vector<int>& selfbuff,std::vector<int>& tobuff,int buff_id,int value){
    if(buff_id>1000&&buff_id<2000){
        int buff=buff_id-SELF_BUFF_ENUM;
        selfbuff[buff]=selfbuff[buff]+value;
    }else if(buff_id>2000){
                int buff=buff_id-TO_BUFF_ENUM;
                tobuff[buff]=tobuff[buff]+value;
        }
       
}

/**
 * @brief 基础攻击结算：力量加成→虚弱削弱→易伤放大→格挡扣除→荆棘反伤
 * @param value 基础伤害值
 * @param self_health 攻击者血量指针（输出，荆棘反伤时减少）
 * @param to_health 目标血量指针（输出，破格挡后减少）
 * @param selfbuff 攻击者Buff数组
 * @param tobuff 目标Buff数组
 * @param defend 目标格挡值指针（输出，被扣除或清空）
 */
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

/**
 * @brief 基础防御结算：脆弱减半→敏捷加成→格挡增加
 * @param defend 目标格挡值指针（输出，被增加）
 * @param value 基础防御值
 * @param selfbuff 防御者Buff数组（影响Debuff_FRAIL和Buff_DEXTERITY）
 */
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

/**
 * @brief AI_INTENSIFY_ATTACK：先强化后攻击
 *        count=1时返回move[0]（通常为Buff），之后返回第一个非空攻击招式
 * @param move 7×4招式表
 * @param count 当前回合数
 * @return int* 指向move中某行的4元素数组
 */
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

/**
 * @brief AI_RANDOM_ATTACK：在[begin,final]范围内随机选一个非空招式
 * @param move 7×4招式表
 * @param begin 起始下标
 * @param final 结束下标
 * @return int* 指向选中的招式行
 */
int *Resolver_AI_RANDOM_ATTACK(std::vector<std::vector<int>>& move, int begin, int final){
    int j=RandNum_between(begin,final);
    while(move[j][0]==0){
        j=RandNum_between(begin,final);
    }
    return move[j].data();
}

/**
 * @brief AI_ROUND_TRIGGER：严格按回合顺序执行招式表，超过7回合后循环
 * @param move 7×4招式表
 * @param count 当前回合数
 * @return int* 指向move[count-1]或move[count%7-1]
 */
int *Resolver_AI_ROUND_TRIGGER(std::vector<std::vector<int>>& move, int count){
    if(count<=7){
        return move[count-1].data();
    }else {
        return move[count%7-1].data();
    }
}

/**
 * @brief AI_BLOOD：生命触发型——健康时随机攻击，低于阈值触发特殊招式(move[0])
 * @param enemy 敌人状态指针（用于读取current health和critical threshold）
 * @return int* 指向选中的招式行
 */
int *Resolver_AI_Blood(EnemyState *enemy){
    int cri=enemy->enemydata.critical;
    int curr=enemy->health;
    if(curr>cri){
        return Resolver_AI_RANDOM_ATTACK(enemy->enemydata.move,1,6);
    }else {
        return enemy->enemydata.move[0].data();
    }
}

/**
 * @brief 数值拆分：将num按10^(rate-1)进制拆分为rate段
 * @param num 待拆分数值
 * @param rate 拆分位数
 * @return int* 5元素数组，调用者需free
 * @note 用于解析多段攻击的编码值（如10506 → [1,0,5,0,6]）
 */
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

/**
 * @brief 从编码值中提取多段攻击的「单段伤害」和「段数」
 * @param a 输出参数：单段伤害
 * @param b 输出参数：段数
 * @param value 编码值（如10506表示 10伤害×6段）
 */
void Num_For_MultiAttack(int *a,int *b,int value){
    int *num=NumBreak(value,5);
    *a=10*num[1]+num[2];
    *b=10*num[3]+num[4];
    free(num);
}

/**
 * @brief 打印卡牌效果描述（如 "攻击 6 + 防御 5"）
 * @param card 卡牌数据引用
 * @note 遍历effect_array，根据effect类型和value生成中文描述
 */
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