#include "../include/DataLoader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int file_line_count(char * filename);
CardState * Data_load_init_card();
EnemyState * Data_load_init_enemy();
void File_OpenError_Reporter();
void   Memory_error_print();

//行数统计
int file_line_count(char * filename){
    FILE *file=fopen(filename,"r");
    if(file==NULL){
        File_OpenError_Reporter();
        return 0;
    }
    char ch='\0';
    int count=0;
    int line_was_new_line=0;
    while(ch!=EOF){
        ch=getc(file);
        if(ch=='\n'){
            count++;
            line_was_new_line=1;
        }else {
            line_was_new_line=0;
        }
    }
    if(line_was_new_line==0&&count>=0){
        count++;
    }
    printf("Have loaded %d lines\n",count);
    fclose(file);
    return count;
}
//卡牌初始化
CardState * Data_load_init_card(){
    int num=file_line_count(CARDDATA);
    CardState *Array_Card=(CardState*)calloc(num,sizeof(CardState));
    if(Array_Card==NULL){
        Memory_error_print();
        return NULL;
    }
    FILE *file=fopen(CARDDATA,"r");
    if(file==NULL){
        File_OpenError_Reporter();
        return 0;
    }
    char midStr[256];
    int now_rate=0;
    while(fgets(midStr, sizeof(midStr), file) != NULL){
            fgets(midStr,sizeof(midStr),file);
            midStr[strcspn(midStr,"\r\n")]=0;
            char *token=strtok(midStr,",");
            int field=0;

            while(token!=NULL){
                switch(field){
                    case 0:
                    Array_Card[now_rate].id=atoi(token);
                    break;
                    case 1:
                    strncpy(Array_Card[now_rate].name,token,100);
                    break;
                    case 2:
                    Array_Card[now_rate].cardtype=atoi(token);
                    break;
                    case 3:
                    Array_Card[now_rate].CardCost=atoi(token);
                    break;
                    case 4:
                    Array_Card[now_rate].effect=atoi(token);
                    break;
                    default:
                    printf("enough\n");
                }
                token=strtok(NULL,",");
                field++;
            }
            now_rate++;
    }
    printf("Have loaded %d cards\n",now_rate+1);
    return Array_Card;
}

//内存分配报错
void Memory_error_print()
{
    printf("Memory Error");
}

void File_OpenError_Reporter(){
    perror("FILE OPEN WRONG");
}


//
EnemyState *Data_load_init_enemy();