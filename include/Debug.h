#ifndef __DEBUG_H__
#define __DEBUG_H__
#include "game.h"
//print card array
void print_CardLoaded_array(CardState *card,int len);
void debug_print_move_library(MoveLibrary *lib);
void debug_print_enemy_library(EnemyLibrary *lib);
//string exput
char * print_enum_to_string_CardEffect(CardEffect effect);
char * print_enum_to_string_CardTYpe(CardType type);
const char* get_intent_name(IntnentType intent);


#endif