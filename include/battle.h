#ifndef __BATTLE_H__
#define __BATTLE_H__
#include "game.h"

void battle_init_player(PlayerState *player);
EnemyState battle_init_enemy(int enemy_id,EnemyLibrary *lib);
void Card_Shuffier(int card[],int len);

void Turn_Player(PlayerState * mc,CardLibrary *lib);
void Turn_Enemy(EnemyState *enemy,PlayerState *mc);

int Card_Drawer(PlayerState *mc, int i,CardLibrary *lib);

void Card_FindAndPrinter(int id,CardLibrary *lib);
void Turn_Buff_Changer(int buff[STAUS_MAX_COUNT]);

#endif
