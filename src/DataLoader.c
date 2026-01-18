#include "../include/DataLoader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int file_line_count(char * filename);
CardDataArray * Data_load_init_card();
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
        char midStr[512];
    fgets(midStr, sizeof(midStr), file);
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
CardDataArray * Data_load_init_card(){
    int num=file_line_count(CARDDATA);
    CardDataArray *Data_Array=(CardDataArray*)malloc(sizeof(CardDataArray));
    CardState *Array_Card=(CardState*)calloc(num,sizeof(CardState));
    if(Array_Card==NULL||Data_Array==NULL){
        Memory_error_print();
        return NULL;
    }
    FILE *file=fopen(CARDDATA,"r");
    if(file==NULL){
        File_OpenError_Reporter();
        return 0;
    }
    char midStr[512];
    int now_rate=0;
    fgets(midStr, sizeof(midStr), file);
    while(fgets(midStr, sizeof(midStr), file) != NULL){
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
                    default:
                    Array_Card[now_rate].effect[field - 4] = atoi(token);
                }
                token=strtok(NULL,",");
                field++;
            }
            now_rate++;
    }
    printf("Have loaded %d cards\n",now_rate);
    fclose(file);
    Data_Array->len=num;
    Data_Array->CardDataArray=Array_Card;
    if(Data_Array->CardDataArray==NULL){
        printf("array give error\n");
    }
    return Data_Array;
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