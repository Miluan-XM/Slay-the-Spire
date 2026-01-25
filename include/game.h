#ifndef __GAME_H__

#define __GAME_H__


//各个职业初始血量
#define Ironclad_MAX_Health  80  
#define Slient_MAX_Health  70
#define Defect_MAX_Health  75 
#define Watcher_MAX_Health  72

//cardeffecct max
#define EFFECT_MAX 5
#define EFFECT_ENUM_NUM 1000
#define STAUS_MAX_COUNT 200
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



typedef struct {
    //玩家拥有的牌
    int master_deck[100];
    int master_deck_count;

    //抽牌堆
    int draw_pile[100];
    int draw_pile_count;

    //手牌
    int hand[10];
    int hand_count;

    //弃牌堆
    int discard_pile[100];
    int discard_pile_count;

    //消耗堆
    int exhaust_pile[100];
    int exhaust_pile_count;
}BattleContext;


//玩家状态结构体
typedef struct {
    HeroType hero;
    int health;   //血量与最大血量
    int max_health;
    int defend;    //格挡值
    int CurrCost;
    int MaxCoat;
    int PlayerBuff[STAUS_MAX_COUNT];// buff结构体数组，当前数值为层数
    //关于卡牌
    BattleContext battle_context;
    //
    int gold;
}PlayerState;




//玩家与怪物buff枚举


typedef enum {
    Buff_None=0,//无BUFF
    //通用
    //正面1-100
    Buff_STRENGTH=1,//力量
    Buff_DEXTERITY,//敏捷
    Buff_THORNS,//荆棘
    //负面100-199
    DeBuff_VALNERABALE=101,//易伤
    DeBuff_WEAK,//虚弱
    Debuff_FRAIL,//脆弱
}BuffEnum;
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
    int EnemyBuff[STAUS_MAX_COUNT];
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
    Effect_Card_Draw,

    Effect_playerBuff001=1001,

    Effect_playerDeBuff101=1101,

    Effect_enemyBuff001=2001,

    Effect_enemyDeBuff101=2101,
}CardEffect;
//效果结构体
typedef struct {
    CardEffect effect;
    int value;
}EffectFact;


//卡牌结构体
typedef struct {
    int id;
    char name[100];
    CardType cardtype;
    int CardCost;
    EffectFact effect_array[5];
}CardState;

//卡牌结构体
typedef struct {
    CardState *CardDataArray;
    int len;
}CardDataArray;

void game_init_player(PlayerState *player);
void InitCard_Ironclad(PlayerState *player);
void game_init_enemy(EnemyState *enemy);













#endif