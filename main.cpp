#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./include/Game.hpp"
#include "./include/DataLoader.hpp"
#include "./include/Debug.hpp"
#include "./include/Battle.hpp"
#include "./include/Effect.hpp"

void battle_loop(PlayerState *mc, EnemyState *enemy, CardLibrary *card_lib, MoveLibrary *move_lib){
    battle_init_player(mc);
    EnemyState en;
    if(enemy) en=*enemy;
    else return;
    int choice=0;
    while(1){
        int *curr_move=Ai_Resolver(&en);
        for(int i=0;i<4;i++) printf("%d",curr_move[i]);
        printf("\n");
        en.curr_move=Move_Searcher(curr_move[0],move_lib);

        Print_Player_Status(mc);
        Print_Enemy_Status(&en);

        if(en.health<=0){
            free(curr_move);
            break;
        }

        Turn_Player(mc,card_lib);

        while(1){
            int judge=scanf("%d",&choice);
            if(judge==EOF){
                free(curr_move);
                return;
            }
            if(judge!=0){
                if(choice==-1) break;
                CardState cc=Card_Searcher(mc->battle_context.hand[choice],card_lib);
                if(cc.id!=0){
                    printf("→ %s ",cc.name);
                    describe_card(cc);
                    play_card_from_hand(mc,choice,card_lib);
                    Card_Resolver(mc,&en,card_lib,cc.id,cc.CardCost);
                    Print_Player_Status(mc);
                    Print_Enemy_Status(&en);
                    if(en.health<=0) break;
                }
            }else{
                printf("非法输入\n");
                while(getchar()!='\n');
            }
        }
        if(en.health<=0){ free(curr_move); break; }
        printf("回合结束\n");
        Turn_Enemy(&en,mc);
        
        int value[3];
        for(int i=0;i<3;i++) value[i]=curr_move[i+1];
        Move_Resolver(curr_move[0],value,move_lib,&en,mc);
        Print_Player_Status(mc);
        Print_Enemy_Status(&en);
        free(curr_move);

        if(mc->health<=0){
            printf("你被击败了...\n");
            return;
        }
        if(en.health<=0) break;
    }
    if(en.health<=0){
        Battle_Close(mc,card_lib);
    }
    *enemy=en;
}

int main(){
    setvbuf(stdout,NULL,_IONBF,0);
    srand((unsigned int)time(NULL));
    
    CardLibrary *Card_Lib=Data_load_init_card();
    EnemyLibrary *Enemy_Lib=Data_load_init_enemy();
    MoveLibrary *Move_Lib=Data_load_init_move();

    PlayerState mc;
    game_init_player(&mc);

    RunState run;
    
    while(!run.game_over&&!run.victory){
        printf("\n========== 第 %d 层 ==========\n",run.floor);
        generate_floor_nodes(&run,run.floor,Enemy_Lib);
        
        for(run.step=0;run.step<3;run.step++){
            show_node_choices(&run,Enemy_Lib);
            int opt=choose_option();
            MapNode *node=nullptr;
            if(opt==1) node=&run.path[run.step];
            else{
                MapNode temp;
                int r=RandNum_between(1,100);
                if(r<=50){
                    temp.type=EVENT_ENEMY;
                    temp.param=pick_random_enemy_id(run.floor,Enemy_Lib);
                }else if(r<=65){
                    temp.type=EVENT_TREASURE;
                    temp.param=30+run.floor*10;
                }else if(r<=80){
                    temp.type=EVENT_MYSTERY;
                }else{
                    temp.type=EVENT_REST;
                    temp.param=30;
                }
                run.path[run.step]=temp;
                node=&run.path[run.step];
            }
            
            printf("\n-> [%s]\n","");
            
            switch(node->type){
                case EVENT_ENEMY:{
                    EnemyState enemy=battle_init_enemy(node->param,Enemy_Lib);
                    battle_loop(&mc,&enemy,Card_Lib,Move_Lib);
                    if(mc.health<=0){
                        printf("\n=== 游戏结束 ===\n");
                        run.game_over=true;
                    }
                    break;
                }
                case EVENT_TREASURE:
                    handle_treasure(&mc,Card_Lib,node->param);
                    break;
                case EVENT_MYSTERY:
                    handle_mystery(&mc,Card_Lib);
                    break;
                case EVENT_REST:
                    handle_rest(&mc,node->param);
                    break;
                default: break;
            }
            
            if(run.game_over) break;
            if(mc.health<=0){
                printf("\n=== 游戏结束 ===\n");
                run.game_over=true;
                break;
            }
        }
        
        if(run.game_over) break;
        
        printf("\n========== Boss战 ==========\n");
        int boss_id=Enemy_Lib->len;
        EnemyState boss=battle_init_enemy(boss_id,Enemy_Lib);
        battle_loop(&mc,&boss,Card_Lib,Move_Lib);
        
        if(boss.health<=0){
            printf("\n=== Boss被击败！===\n");
            if(run.floor>=run.max_floors){
                printf("\n========== 恭喜通关！==========\n");
                printf("最终金币: %d\n",mc.gold);
                printf("牌组数量: %d\n",mc.master_card.master_count);
                run.victory=true;
            }else{
                run.floor++;
                mc.health=mc.max_health/2+mc.health/2;
                if(mc.health>mc.max_health) mc.health=mc.max_health;
                printf("进入下一层！恢复部分生命 (%d/%d)\n",mc.health,mc.max_health);
            }
        }else{
            printf("\n=== 游戏结束 ===\n");
            run.game_over=true;
        }
    }

    if(!run.game_over&&run.victory){
        printf("\n========== 最终统计 ==========\n");
        printf("生命: %d/%d\n",mc.health,mc.max_health);
        printf("金币: %d\n",mc.gold);
        printf("牌组: %d张\n",mc.master_card.master_count);
    }

    delete Card_Lib;
    delete Enemy_Lib;
    delete Move_Lib;
    return 0;
}