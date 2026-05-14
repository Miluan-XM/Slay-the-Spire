#ifndef __BATTLE_HPP__
#define __BATTLE_HPP__

#include "Game.hpp"

void battle_init_player(PlayerState *player);
EnemyState battle_init_enemy(int enemy_id,EnemyLibrary *lib);
void Card_Shuffier(int card[],int len);

void Turn_Player(PlayerState * mc,CardLibrary *lib);
void Turn_Enemy(EnemyState *enemy,PlayerState *mc);

int Card_Drawer(PlayerState *mc, int i,CardLibrary *lib);

void Card_FindAndPrinter(int id,CardLibrary *lib);
void Turn_Buff_Changer(std::vector<int>& buff);
int play_card_from_hand(PlayerState *mc, int index, CardLibrary *lib);

void Battle_Close(PlayerState *player,CardLibrary *lib);

#endif // __BATTLE_HPP__