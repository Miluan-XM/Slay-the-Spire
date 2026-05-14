#ifndef __CARDEFFECT_HPP__
#define __CARDEFFECT_HPP__

#include "Game.hpp"

/**
 * @brief 卡牌效果总解析器
 * 根据卡牌 ID 从库中提取 5 组效果并调用底层 Resolver 执行。
 * @param mc 玩家状态指针
 * @param enemy 敌人状态指针
 * @param lib 卡牌数据库指针
 * @param id 正在释放的卡牌 ID
 * @param cost 该牌消耗的能量
 */
void Card_Resolver(PlayerState *mc, EnemyState *enemy, CardLibrary *lib, int id, int cost);

/**
 * @brief 怪物招式总解析器
 * 根据招式 ID 和传入的特定数值，执行怪物的意图动作。
 * @param id 招式模板 ID (来自 MoveData)
 * @param move_value 对应 EnemyData 中填写的 3 个具体数值
 * @param lib 招式数据库指针
 * @param enemy 施法者（怪物）指针
 * @param mc 目标（玩家）指针
 */
void Move_Resolver(int id, int move_value[3], MoveLibrary *lib, EnemyState *enemy, PlayerState *mc);

/**
 * @brief 怪物 AI 决策中心
 * 根据怪物的 AI 类型和当前状态（回合数、血量等），计算并锁定下一个招式下标。
 * @param enemy 怪物状态指针
 * @return int 选中的招式下标 (0-6)
 */
int *Ai_Resolver(EnemyState *enemy);


/* ========================================================================== */
/*                         底层机制层 (Mechanics Resolvers)                   */
/* ========================================================================== */

/**
 * @brief Buff/DeBuff 结算器
 * 处理力量、易伤、虚弱等状态的叠加逻辑。
 * @param selfbuff 施法者的 Buff 数组
 * @param tobuff 目标的 Buff 数组
 * @param buff_id 状态枚举 ID
 * @param value 赋予的层数
 */
void Resolver_Buff(std::vector<int>& selfbuff, std::vector<int>& tobuff, int buff_id, int value);

/**
 * @brief 基础攻击结算器
 * 包含：伤害计算 = (基础 + 力量) * 虚弱 * 易伤，并处理格挡扣除和荆棘反伤。
 */
void Resolver_Attack(int value, int *self_health, int *to_health, std::vector<int>& selfbuff, std::vector<int>& tobuff, int *defend);

/**
 * @brief 基础防御结算器
 * 计算敏捷加成并增加目标的格挡值。
 */
void Resolver_Defend(int *defend, int value, std::vector<int>& selfbuff);


/* ========================================================================== */
/*                         AI 模式分支 (AI Pattern Resolvers)                 */
/* ========================================================================== */

/**
 * @brief AI 模式：强化攻击
 * 通常用于第一回合给自己加 Buff，后续一直进行攻击的逻辑（如邪教徒）。
 */
int *Resolver_AI_INTENSIFY_ATTACK(std::vector<std::vector<int>>& move, int count);

/**
 * @brief AI 模式：随机出招
 * 在指定的招式下标范围 [begin, final] 内随机抽取一个。
 */
int *Resolver_AI_RANDOM_ATTACK(std::vector<std::vector<int>>& move, int begin, int final);

/**
 * @brief AI 模式：回合触发（时间轴）
 * 根据当前的 turn_count 严格按顺序执行招式表（如第一勇者）。
 */
int *Resolver_AI_ROUND_TRIGGER(std::vector<std::vector<int>>& move, int count);

/**
 * @brief AI 模式：血量触发
 * 根据怪物的生命值百分比切换招式（如半血分裂、低血量狂暴）。
 */
int *Resolver_AI_Blood(EnemyState *enemy);


/* ========================================================================== */
/*                         辅助计算工具 (Utility Functions)                    */
/* ========================================================================== */

/**
 * @brief 数值拆分工具
 * 可能是用于将一个总数值按照某种比例（rate）拆分成多个部分，用于复杂伤害计算。
 */
int *NumBreak(int num, int rate);

/**
 * @brief 多段攻击逻辑处理器
 * 用于处理类似 6x3 这种多段伤害的数值分配逻辑。
 */
void Num_For_MultiAttack(int *a, int *b, int value);

/**
 * @brief 打印卡牌效果描述
 */
void describe_card(CardState &card);



#endif // __CARDEFFECT_HPP__