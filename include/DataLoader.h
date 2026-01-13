#ifndef __DATALOADER_H__
#define __DATALOADER_H__

#include "game.h"

CardState *Data_load_init_card();
void Memory_error_print();
void File_OpenError_Reporter();
int file_line_count(char * filename);
EnemyState *Data_load_init_enemy();



#endif