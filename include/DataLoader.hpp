#ifndef __DATALOADER_HPP__
#define __DATALOADER_HPP__

#include "Game.hpp"

/**
 * @brief 统计文件数据行数（跳过首行表头，以换行符计数）
 * @param filename 文件路径
 * @return 数据行数
 */
int file_line_count(char *filename);

/**
 * @brief 从 CardData.txt 加载所有卡牌数据
 * @return CardLibrary* 卡牌库指针（含所有卡牌数据），失败返回nullptr
 */
CardLibrary *Data_load_init_card();

/**
 * @brief 从 EnemyData.txt 加载所有敌人蓝图数据
 * @return EnemyLibrary* 敌人库指针，失败返回nullptr
 */
EnemyLibrary *Data_load_init_enemy();

/**
 * @brief 从 MoveData.txt 加载所有招式数据
 * @return MoveLibrary* 招式库指针，失败返回nullptr
 */
MoveLibrary *Data_load_init_move();

/**
 * @brief 打印文件打开错误信息
 */
void File_OpenError_Reporter();

/**
 * @brief 打印内存分配错误信息
 */
void Memory_error_print();

#endif // __DATALOADER_HPP__