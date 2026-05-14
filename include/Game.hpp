#ifndef __GAME_HPP__

#define __GAME_HPP__

#include <vector>
#include <string>
#include <cstring>


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
class Master_Card {
public:
    std::vector<int> master;
    int master_count;
    Master_Card() : master_count(0) {
        master.resize(100, 0);
    }
};


class BattleContext {
public:
    std::vector<int> master_deck;
    int master_deck_count;
    std::vector<int> draw_pile;
    int draw_pile_count;
    std::vector<int> hand;
    int hand_count;
    std::vector<int> discard_pile;
    int discard_pile_count;
    std::vector<int> exhaust_pile;
    int exhaust_pile_count;

    BattleContext() : master_deck_count(0), draw_pile_count(0), hand_count(0), 
                      discard_pile_count(0), exhaust_pile_count(0) {
        master_deck.resize(100, 0);
        draw_pile.resize(100, 0);
        hand.resize(10, 0);
        discard_pile.resize(100, 0);
        exhaust_pile.resize(100, 0);
    }
};


//玩家状态结构体
class PlayerState {
public:
    HeroType hero;
    int health;
    int max_health;
    int defend;
    int CurrCost;
    int MaxCoat;
    std::vector<int> PlayerBuff;
    BattleContext battle_context;
    Master_Card master_card;
    int gold;

    PlayerState() : hero(Ironclad), health(0), max_health(0), defend(0), 
                    CurrCost(3), MaxCoat(3), gold(0) {
        PlayerBuff.resize(STAUS_MAX_COUNT, 0);
    }
};



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
class MoveFact {
public:
    int id;
    char name[50];
    std::vector<IntnentType> intention;

    MoveFact() : id(0) {
        memset(name, 0, 50);
        intention.resize(3, IntnentType(0));
    }
};

//动作目录
class MoveLibrary {
public:
    std::vector<MoveFact> movedata;
    int len;
    MoveLibrary() : len(0) {}
};
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
class EnemyData {
public:
    int id;
    char name[100];
    int Max_health;
    AI_MODE mode;
    int critical;
    std::vector<std::vector<int>> move;

    EnemyData() : id(0), Max_health(0), mode(AI_RANDOM_ATTACK), critical(0) {
        memset(name, 0, 100);
        move.resize(7, std::vector<int>(4, 0));
    }
};


//数据储存
class EnemyLibrary {
public:
    std::vector<EnemyData> enemystate; 
    int len;
    EnemyLibrary() : len(0) {}
};



//z战斗中怪物状态
class EnemyState {
public:
    EnemyData enemydata;
    int max_health;
    int health;
    int defend;
    MoveFact curr_move;
    std::vector<int> EnemyBuff;
    int turn_count;

    EnemyState() : max_health(0), health(0), defend(0), turn_count(1) {
        EnemyBuff.resize(STAUS_MAX_COUNT, 0);
    }
};




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
class EffectFact {
public:
    CardEffect effect;
    int value;
    EffectFact() : effect(Effect_ATTACK), value(0) {}
};


//卡牌结构体
class CardState {
public:
    int id;
    char name[100];
    CardType cardtype;
    int CardCost;
    std::vector<EffectFact> effect_array;

    CardState() : id(0), cardtype(ATTACK), CardCost(0) {
        memset(name, 0, 100);
        effect_array.resize(EFFECT_MAX);
    }
};

//卡牌结构体
class CardLibrary {
public:
    std::vector<CardState> CardDataArray;
    int len;
    CardLibrary() : len(0) {}
};

/**
 * @brief 玩家初始化：选择职业、设置血量、初始化卡组
 * @param player 玩家状态指针（输出）
 */
void game_init_player(PlayerState *player);

/**
 * @brief 为铁甲战士初始化初始卡组（5打击+4防御+1痛击）
 * @param player 玩家状态指针（输出）
 */
void Init_Card_Ironclad(PlayerState *player);

/**
 * @brief 敌人初始化（占位，当前未使用）
 * @param enemy 敌人状态指针（输出）
 */
void game_init_enemy(EnemyState *enemy);

/**
 * @brief 按ID查找卡牌
 * @param id 卡牌ID（从1开始）
 * @param lib 卡牌库指针
 * @return 卡牌数据的副本，越界时返回默认构造
 */
CardState Card_Searcher(int id, CardLibrary *lib);

/**
 * @brief 按ID查找招式
 * @param id 招式ID（从501开始）
 * @param lib 招式库指针
 * @return 招式数据的副本，越界时返回默认构造
 */
MoveFact Move_Searcher(int id, MoveLibrary *lib);

/**
 * @brief 按ID查找敌人蓝图
 * @param id 敌人ID（从1开始）
 * @param lib 敌人库指针
 * @return 敌人数据的副本，越界时返回默认构造
 */
EnemyData Enemy_Searcher(int id, EnemyLibrary *lib);

/**
 * @brief Fisher-Yates 洗牌算法
 * @param card 待洗牌的数组
 * @param len 数组长度
 */
void Card_Shuffier(int card[], int len);

/**
 * @brief 生成 [a, b] 闭区间的随机整数
 * @param a 下限
 * @param b 上限（自动处理 a>b 的情况）
 * @return 区间内的随机整数
 */
int RandNum_between(int a, int b);

/**
 * @brief 交换两个整数的值
 * @param a 指针1
 * @param b 指针2
 */
void swap_value(int *a, int *b);

/**
 * @brief 根据Buff枚举ID获取其简写名称（用于UI显示）
 * @param index Buff枚举值
 * @return 简写字符串，如 "STR"、"VUL"、"RIT"；未知返回 "???"
 */
const char* UI_Get_Buff_Name(int index);

/**
 * @brief 打印玩家状态UI（HP条、格挡、能量、Buff）
 * @param p 玩家状态指针
 */
void Print_Player_Status(PlayerState *p);

/**
 * @brief 打印敌人状态UI（名字、意图、HP条、格挡、Buff）
 * @param e 敌人状态指针
 */
void Print_Enemy_Status(EnemyState *e);

// ========== 地图/事件系统 ==========

typedef enum {
    EVENT_ENEMY,    ///< 战斗事件
    EVENT_TREASURE, ///< 宝箱事件
    EVENT_MYSTERY,  ///< 未知事件
    EVENT_REST,     ///< 篝火恢复
    EVENT_BOSS,     ///< Boss战
} EventType;

class MapNode {
public:
    EventType type; ///< 事件类型
    char title[50]; ///< 事件标题
    char desc[200]; ///< 事件描述
    int param;      ///< 参数：敌人ID/金币量/恢复百分比
    MapNode() : type(EVENT_ENEMY), param(0) {
        memset(title, 0, 50);
        memset(desc, 0, 200);
    }
};

class RunState {
public:
    int floor;          ///< 当前层数（从1开始）
    int max_floors;     ///< 总层数
    int step;           ///< 当前层内进度（0-2）
    MapNode path[3];    ///< 本层已选的3个事件节点
    bool game_over;     ///< 游戏结束标志
    bool victory;       ///< 通关标志
    RunState() : floor(1), max_floors(3), step(0), game_over(false), victory(false) {}
};

/**
 * @brief 将指定卡牌加入玩家牌组
 * @param player 玩家状态指针（输出）
 * @param card_id 卡牌ID
 * @param lib 卡牌库指针（用于打印卡名）
 */
void add_card_to_deck(PlayerState *player, int card_id, CardLibrary *lib);

/**
 * @brief 根据层数随机选择敌人ID（难度递增）
 * @param floor 当前层数
 * @param lib 敌人库指针
 * @return 敌人ID（从1开始）
 */
int pick_random_enemy_id(int floor, EnemyLibrary *lib);

/**
 * @brief 为当前层生成3个随机事件节点
 * @param run 运行状态指针（输出）
 * @param floor 当前层数
 * @param enemy_lib 敌人库指针（用于战斗事件）
 */
void generate_floor_nodes(RunState *run, int floor, EnemyLibrary *enemy_lib);

/**
 * @brief 显示当前步的2个可选事件
 * @param run 运行状态指针
 * @param enemy_lib 敌人库指针
 */
void show_node_choices(RunState *run, EnemyLibrary *enemy_lib);

/**
 * @brief 让玩家从两个选项中做出选择（1或2）
 * @return 玩家选择的选项（1或2）
 */
int choose_option();

/**
 * @brief 战斗后奖励：金币 + 选卡
 * @param player 玩家状态指针（输出）
 * @param lib 卡牌库指针
 */
void battle_reward(PlayerState *player, CardLibrary *lib);

/**
 * @brief 宝箱事件：获得金币 + 选卡
 * @param player 玩家状态指针（输出）
 * @param lib 卡牌库指针
 * @param gold_amt 获得的金币数量
 */
void handle_treasure(PlayerState *player, CardLibrary *lib, int gold_amt);

/**
 * @brief 未知事件：随机触发回血/金币/受伤/得卡/无事
 * @param player 玩家状态指针（输出）
 * @param lib 卡牌库指针
 */
void handle_mystery(PlayerState *player, CardLibrary *lib);

/**
 * @brief 篝火事件：按百分比恢复生命值
 * @param player 玩家状态指针（输出）
 * @param pct 恢复百分比（如30=30%）
 */
void handle_rest(PlayerState *player, int pct);

#endif // __GAME_HPP__