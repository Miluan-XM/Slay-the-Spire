#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

#include "Game.hpp"

/**
 * @brief 调试打印：以表格形式输出所有已加载卡牌的ID/名字/类型/费用/效果
 * @param card 卡牌数组
 * @param len 数组长度
 */
void print_CardLoaded_array(CardState *card, int len);

/**
 * @brief 调试打印：输出招式库中所有招式的ID/名字/意图列表
 * @param lib 招式库指针
 */
void debug_print_move_library(MoveLibrary *lib);

/**
 * @brief 调试打印：输出敌人库中所有敌人的ID/名字/血量/AI/招式配置
 * @param lib 敌人库指针
 */
void debug_print_enemy_library(EnemyLibrary *lib);

/**
 * @brief 将 CardEffect 枚举值映射为可读字符串（用于调试输出）
 * @param effect 效果枚举值
 * @return 字符串指针（如 "Effect_ATTACK"、"UNKONWN"）
 */
char *print_enum_to_string_CardEffect(CardEffect effect);

/**
 * @brief 将 CardType 枚举值映射为可读字符串
 * @param type 卡牌类型枚举值
 * @return 字符串指针（如 "ATTACK"、"SKILL"）
 */
char *print_enum_to_string_CardTYpe(CardType type);

/**
 * @brief 将 IntnentType 枚举值映射为简写名称
 * @param intent 意图枚举值
 * @return 简写字符串（如 "ATK"、"DEF"），0返回"-"，未知返回"UNK"
 */
const char *get_intent_name(IntnentType intent);

#endif // __DEBUG_HPP__