#include "../include/DataLoader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int file_line_count(char * filename);
CardLibrary * Data_load_init_card();
EnemyLibrary * Data_load_init_enemy();
MoveLibrary* Data_load_init_move();
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
CardLibrary * Data_load_init_card(){
    int num=file_line_count(CARDDATA);
    CardLibrary *Data_Array=(CardLibrary*)malloc(sizeof(CardLibrary));
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
                    if(field<=8){
                    Array_Card[now_rate].effect_array[field-4].effect=atoi(token);
                    }else{
                        Array_Card[now_rate].effect_array[field-9].value=atoi(token);
                    }
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
EnemyLibrary *Data_load_init_enemy(){
    FILE *file=fopen(ENEMYDATA,"r");
    if(file==NULL){
        File_OpenError_Reporter();
    }
    int num=file_line_count(ENEMYDATA);
    EnemyData * enemyData=(EnemyData*)calloc(num,sizeof(EnemyData));
    EnemyLibrary * enemylibrary=(EnemyLibrary*)malloc(sizeof(EnemyLibrary));
    if(enemyData==NULL||enemylibrary==NULL){
        Memory_error_print();
        return NULL;
    }
    char mid[512];
    fgets(mid,512,file);
    int now_rate=0;
    while(fgets(mid,512,file)){

        char *token=strtok(mid,",");
        int field=0;
        while(token!=NULL){
            switch (field)
            {
            case 0:
            enemyData[now_rate].id=atoi(token);
            break;
            case 1:
            strcpy(enemyData[now_rate].name,token);
            break;
            case 2:
            enemyData[now_rate].Max_health=atoi(token);
            break;
            default:    
                    //进入招式以及数值写入
                //三个招式数值的写入
                if(field>9){
                sscanf(token,"{%d:%d:%d}",
                    &enemyData[now_rate].move[field-10][1],
                    &enemyData[now_rate].move[field-10][2],
                    &enemyData[now_rate].move[field-10][3]
                );
                 }else{
                    enemyData[now_rate].move[field-3][0]=atoi(token);//field==9,结束
                }
                break;
            }

                    token=strtok(NULL,",");         
                    field++;
        }

        now_rate++;
    }
    printf("Have loaded %d enemys\n",now_rate);

    enemylibrary->enemystate=enemyData;
    enemylibrary->len=num;
    if(enemylibrary->enemystate==NULL){
            printf("array give error\n");
    }
    fclose(file);
    return enemylibrary;
}


MoveLibrary* Data_load_init_move(){
    FILE *file=fopen(MOVEDATA,"r");
    if(file==NULL){
        File_OpenError_Reporter();
    }
    int num=file_line_count(MOVEDATA);
    MoveLibrary *movelibrary=(MoveLibrary*)malloc(sizeof(MoveLibrary));
    MoveFact *movedata=(MoveFact*)calloc(num,sizeof(MoveFact));
    if(movelibrary==NULL||movedata==NULL){
        Memory_error_print();
        return NULL;
    }
    char mid[1024];
    int now_rate=0;
    fgets(mid,sizeof(mid),file);
    while( fgets(mid,sizeof(mid),file)!=NULL){
        char * token=strtok(mid,",");
        int field=0;
        while(token!=NULL){
            switch (field)
            {
            case 0:
            movedata[now_rate].id=atoi(token);
            break;
            case 1:
            strcpy(movedata[now_rate].name,token);
            break;
            default:
            movedata[now_rate].intention[field-2]=atoi(token);
            break;
            }
            field++;
            token=strtok(NULL,",");
        }
        now_rate++;

    }
    fclose(file);
    movelibrary->movedata=movedata;
    if(movelibrary->movedata==NULL){
            printf("array give error\n");
    }
    movelibrary->len=num;
    return movelibrary;
}