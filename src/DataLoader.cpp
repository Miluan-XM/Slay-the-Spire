#include "../include/DataLoader.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief 统计文件数据行数：跳过首行表头，逐字符读文件统计'\n'数量
 * @param filename 文件路径
 * @return 数据行数，失败返回0
 */
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

/**
 * @brief 从 CardData.txt 加载卡牌数据
 * @details CSV格式：id,name,CardType,CardCost,effect[0-4],value[0-4]
 *          字段4-8为效果类型枚举，字段9-13为效果数值
 * @return CardLibrary* 卡牌库指针，文件打开失败返回nullptr
 */
CardLibrary * Data_load_init_card(){
    int num=file_line_count(CARDDATA);
    CardLibrary *Data_Array = new CardLibrary();
    Data_Array->CardDataArray.resize(num);
    
    FILE *file=fopen(CARDDATA,"r");
    if(file==NULL){
        File_OpenError_Reporter();
        return nullptr;
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
                    Data_Array->CardDataArray[now_rate].id=atoi(token);
                    break;
                    case 1:
                    strncpy(Data_Array->CardDataArray[now_rate].name,token,99);
                    break;
                    case 2:
                    Data_Array->CardDataArray[now_rate].cardtype=(CardType)atoi(token);
                    break;
                    case 3:
                    Data_Array->CardDataArray[now_rate].CardCost=atoi(token);
                    break;
                    default:
                    if(field<=8){
                    Data_Array->CardDataArray[now_rate].effect_array[field-4].effect=(CardEffect)atoi(token);
                    }else{
                        Data_Array->CardDataArray[now_rate].effect_array[field-9].value=atoi(token);
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
    return Data_Array;
}

/**
 * @brief 打印内存分配错误信息
 */
void Memory_error_print()
{
    printf("Memory Error");
}

/**
 * @brief 打印文件打开错误信息（使用perror）
 */
void File_OpenError_Reporter(){
    perror("FILE OPEN WRONG");
}

/**
 * @brief 从 EnemyData.txt 加载敌人蓝图数据
 * @details CSV格式：id,name,max_health,ai_mode,Critical:value,moveID[0-6],{v1:v2:v3}[0-6]
 *          字段5-11为招式ID，字段12-为招式数值（用{}包裹的3个值）
 * @return EnemyLibrary* 敌人库指针
 */
EnemyLibrary *Data_load_init_enemy(){
    FILE *file=fopen(ENEMYDATA,"r");
    if(file==NULL){
        File_OpenError_Reporter();
    }
    int num=file_line_count(ENEMYDATA);
    EnemyLibrary *enemylibrary = new EnemyLibrary();
    enemylibrary->enemystate.resize(num);
    
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
            enemylibrary->enemystate[now_rate].id=atoi(token);
            break;
            case 1:
            strncpy(enemylibrary->enemystate[now_rate].name,token,99);
            break;
            case 2:
            enemylibrary->enemystate[now_rate].Max_health=atoi(token);
            break;
            case 3:
            enemylibrary->enemystate[now_rate].mode=(AI_MODE)atoi(token);
            break;
            case 4:
            sscanf(token,"Critical:%d",&enemylibrary->enemystate[now_rate].critical);
            break;
            default:    
                if(field>11){
                sscanf(token,"{%d:%d:%d}",
                    &enemylibrary->enemystate[now_rate].move[field-12][1],
                    &enemylibrary->enemystate[now_rate].move[field-12][2],
                    &enemylibrary->enemystate[now_rate].move[field-12][3]
                );
                 }else{
                     enemylibrary->enemystate[now_rate].move[field-5][0]=atoi(token);
                 }
                break;
            }

                    token=strtok(NULL,",");         
                    field++;
        }

        now_rate++;
    }
    printf("Have loaded %d enemys\n",now_rate);

    enemylibrary->len=num;
    fclose(file);
    return enemylibrary;
}

/**
 * @brief 从 MoveData.txt 加载招式数据
 * @details CSV格式：id,name,intention[0],intention[1],intention[2]
 *          招式ID从501开始，意图枚举为IntnentType
 * @return MoveLibrary* 招式库指针
 */
MoveLibrary* Data_load_init_move(){
    FILE *file=fopen(MOVEDATA,"r");
    if(file==NULL){
        File_OpenError_Reporter();
    }
    int num=file_line_count(MOVEDATA);
    MoveLibrary *movelibrary = new MoveLibrary();
    movelibrary->movedata.resize(num);
    
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
            movelibrary->movedata[now_rate].id=atoi(token);
            break;
            case 1:
            strncpy(movelibrary->movedata[now_rate].name,token,49);
            break;
            default:
            movelibrary->movedata[now_rate].intention[field-2]=(IntnentType)atoi(token);
            break;
            }
            field++;
            token=strtok(NULL,",");
        }
        now_rate++;

    }
    fclose(file);
    movelibrary->len=num;
    return movelibrary;
}