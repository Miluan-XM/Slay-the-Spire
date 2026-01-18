#ifndef __GAME_H__

#define __GAME_H__


//各个职业初始血量
#define Ironclad_MAX_Health  80  
#define Slient_MAX_Health  70
#define Defect_MAX_Health  75 
#define Watcher_MAX_Health  72

//cardeffecct max
#define EFFECT_MAX 5

//文件名字
#define CARDDATA "Data/CardData.txt"
#define ENEMYDATA "Data/EnemyData.txt"
//职业枚举
typedef enum {
    Ironclad=1,
    Slient,
    Defect,
    Watcher,
}HeroType;

//玩家





//玩家状态结构体
typedef struct {
    HeroType hero;
    int health;   //血量与最大血量
    int max_health;
    int defend;    //格挡值
    int CurrCost;
    int MaxCoat;
}PlayerState;


//怪物

//怪物的意图枚举
typedef enum {
    intent_Attack=0,            //攻击
    intent_Defend,              //防御
    intent_GiveSelfBuff,        //强化自身
    intent_GivePlayerBuff,      //给予负面
}IntnentType;


//怪物数据
typedef struct{
    int id;
    char name[100];
    int Max_health;
}EnemyData;

//怪物状态
typedef struct{
    int health;
    int max_health;
    int defend;
    IntnentType intention;
}EnemyState;




//卡牌

//卡牌类型枚举
typedef enum{
    ATTACK=1,
    SKILL,
    ABILITY,
    CURSE,
}CardType;



//卡牌效果枚举
typedef enum{
    Effect_ATTACK=1,
    Effect_Defend,
    Effect_Buff_self,
    Effect_Buff_enemy,
    Effect_Card_Draw,
}CardEffect;

//卡牌结构体
typedef struct {
    int id;
    char name[100];
    CardType cardtype;
    int CardCost;
    CardEffect effect[5];
}CardState;

//卡牌结构体
typedef struct {
    CardState *CardDataArray;
    int len;
}CardDataArray;


void game_init_player(PlayerState *player);
void game_init_enemy(EnemyState *enemy);













#endif