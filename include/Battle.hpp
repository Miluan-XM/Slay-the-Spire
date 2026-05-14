#ifndef __BATTLE_HPP__
#define __BATTLE_HPP__

#include "Game.hpp"

/**
 * @brief 战斗初始化：将玩家牌组复制到抽牌堆并洗牌
 * @param player 玩家状态指针（输出，battle_context被重置）
 */
void battle_init_player(PlayerState *player);

/**
 * @brief 根据敌人ID创建战斗中的敌人实例
 * @param enemy_id 敌人蓝图ID
 * @param lib 敌人库指针
 * @return EnemyState 敌人实例（包含当前血量、Buff等战斗状态）
 */
EnemyState battle_init_enemy(int enemy_id, EnemyLibrary *lib);

/**
 * @brief Fisher-Yates洗牌
 * @param card 待洗牌的数组
 * @param len 数组长度
 */
void Card_Shuffier(int card[], int len);

/**
 * @brief 玩家回合开始：重置能量→格挡→减Buff→弃手牌→抽5张牌
 * @param mc 玩家状态指针（输出）
 * @param lib 卡牌库指针
 */
void Turn_Player(PlayerState *mc, CardLibrary *lib);

/**
 * @brief 敌人回合开始：格挡→减Buff→仪式→力量→回合计数
 * @param enemy 敌人状态指针（输出）
 * @param mc 玩家状态指针（未使用，保留接口一致性）
 */
void Turn_Enemy(EnemyState *enemy, PlayerState *mc);

/**
 * @brief 从抽牌堆顶抽取一张牌放入手牌指定位置
 * @param mc 玩家状态指针（输出）
 * @param i 手牌槽位索引
 * @param lib 卡牌库指针
 * @return int 1=成功，0=手牌已满/无牌可抽
 * @note 抽牌堆为空时自动从弃牌堆洗牌接续
 */
int Card_Drawer(PlayerState *mc, int i, CardLibrary *lib);

/**
 * @brief 按卡牌ID打印卡牌名字
 * @param id 卡牌ID
 * @param lib 卡牌库指针
 */
void Card_FindAndPrinter(int id, CardLibrary *lib);

/**
 * @brief 回合结束时减少Buff层数（易伤、虚弱、脆弱各-1）
 * @param buff Buff数组引用（输出）
 */
void Turn_Buff_Changer(std::vector<int>& buff);

/**
 * @brief 出牌：将hand[index]移入弃牌堆→左移后续手牌→重显手牌
 * @param mc 玩家状态指针（输出）
 * @param index 手牌索引
 * @param lib 卡牌库指针
 * @return int 1=成功，0=索引无效或卡牌为空
 */
int play_card_from_hand(PlayerState *mc, int index, CardLibrary *lib);

/**
 * @brief 战斗胜利结算：发放金币奖励 + 选卡奖励
 * @param player 玩家状态指针（输出）
 * @param lib 卡牌库指针
 */
void Battle_Close(PlayerState *player, CardLibrary *lib);

#endif // __BATTLE_HPP__