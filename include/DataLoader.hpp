#ifndef __DATALOADER_HPP__
#define __DATALOADER_HPP__

#include "Game.hpp"

int file_line_count(char * filename);
CardLibrary * Data_load_init_card();
EnemyLibrary * Data_load_init_enemy();
MoveLibrary* Data_load_init_move();
void File_OpenError_Reporter();
void   Memory_error_print();

#endif // __DATALOADER_HPP__