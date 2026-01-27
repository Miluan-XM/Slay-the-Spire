#ifndef __DATALOADER_H__
#define __DATALOADER_H__

#include "game.h"



int file_line_count(char * filename);
CardLibrary * Data_load_init_card();
EnemyLibrary * Data_load_init_enemy();
MoveLibrary* Data_load_init_move();
void File_OpenError_Reporter();
void   Memory_error_print();

#endif