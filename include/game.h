#ifndef __GAME_H__

#define __GAME_H__


//各个职业初始血量
#define Ironclad_MAX_Health  80  
#define Slient_MAX_Health  70
#define Defect_MAX_Health  75 
#define Watcher_MAX_Health  72

//cardeffecct max
#define EFFECT_MAX 5
#define SELF_BUFF_ENUM 1000
#define TO_BUFF_ENUM 2000
#define STAUS_MAX_COUNT 900
//文件名字
#define CARDDATA "Data/CardData.txt"
#define ENEMYDATA "Data/EnemyData.txt"
#define MOVEDATA "Data/MoveData.txt"
/**怪物的特殊招式的位次 */
#define SPECIAL_MOVE_RATE 0



//职业枚举
typedef enum {
    Ironclad=1,
    Slient,
    Defect,
    Watcher,
}HeroType;

//玩家


typedef struct {
    int master[100];
    int master_count;
}Master_Card;


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
    Master_Card master_card;
    //
    int gold;
}PlayerState;




//玩家与怪物buff枚举


typedef enum {
    /** 无状态 */
    Buff_None = 0,

    /* --- 通用正面状态 (1-300) --- */

    /** 力量：每层增加攻击伤害值 */
    Buff_STRENGTH = 1,

    /** 敏捷：每层增加格挡时获得的护甲值 */
    Buff_DEXTERITY,

    /** 荆棘：受到攻击时，对攻击者反弹等同于层数的伤害 */
    Buff_THORNS,

    /** 壁垒：回合开始时，格挡值不再自动清零 */
    Buff_BARRIER,


    /* --- 通用负面状态 (301-600) --- */

    /** 易伤：受到的攻击伤害增加 25% (或按公式计算) */
    DeBuff_VALNERABALE = 301,

    /** 虚弱：造成的攻击伤害减少 25% */
    DeBuff_WEAK,

    /** 脆弱：通过卡牌获得的格挡值减少 25% */
    Debuff_FRAIL,


    /* --- 怪物专属状态 (601-900) --- */

    /** 仪式：每回合结束时，获得等同于层数的力量 */
    EnemyBuff_RITUAL = 601,

}BuffEnum;
//怪物

//怪物的意图枚举
typedef enum {
    intent_Attack=1,            //攻击
    intent_Defend,              //防御
    intent_CardGive,            //塞牌
    /**意图：召唤 */
    intent_Dummon,
    intent_selfBuff001=1001,



    intent_selfDeBuff101=1301,


    intent_selfEnemyBuff=1601,


    intent_giveBuff001=2001,


    intent_giveDeBuff101=2301,


    intent_giveEnemyBuff=2601,

}IntnentType;
//意图结构体
typedef struct {
    int id;
    char name[50];
    IntnentType intention[3];//一个动作可以是多个意图组合
}MoveFact;

//动作目录
typedef struct{
    MoveFact *movedata;
    int len;
}MoveLibrary;
//怪物ai
typedef enum {
    /**先强化后攻击型 */
    AI_INTENSIFY_ATTACK=1,
    /**一直随机攻击型 */
    AI_RANDOM_ATTACK=2,
    /**回合触发型 */
    AI_ROUND_TRIGGER=3,
    /**生命触发型 */
    AI_BLOOD=4,
}AI_MODE;
//怪物数据
typedef struct{
    int id;
    char name[100];
    int Max_health;
    AI_MODE mode;
    int critical;
    int move[7][4];//一个怪物最多七个意图的id,一个招式最多3个数值
}EnemyData;


//数据储存
typedef struct{
    EnemyData *enemystate; 
    int len;
}EnemyLibrary;



//z战斗中怪物状态
typedef struct{
    EnemyData enemydata;
    int max_health;
    int health;
    int defend;
    MoveFact curr_move;
    int EnemyBuff[STAUS_MAX_COUNT];
    int turn_count;
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

    Effect_playerDeBuff101=1301,

    Effect_enemyBuff001=2001,

    Effect_enemyDeBuff101=2301,
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
}CardLibrary;

void game_init_player(PlayerState *player);
void Init_Card_Ironclad(PlayerState *player);
void game_init_enemy(EnemyState *enemy);
//查阅函数
CardState  Card_Searcher(int id,CardLibrary *lib);
MoveFact  Move_Searcher(int id,MoveLibrary *lib);
EnemyData Enemy_Searcher(int id,EnemyLibrary *lib);


void Card_Shuffier(int card[],int len);
int RandNum_between(int a,int b);


void swap_value(int *a,int *b);
const char* UI_Get_Buff_Name(int index);

void Print_Player_Status(PlayerState *p) ;


void Print_Enemy_Status(EnemyState *e) ;







#endif