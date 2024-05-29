#include "gamescene.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include<string.h>
//**  For Timer and Random functions
#define Min_Interval 0
#include <time.h>
#include <stdlib.h>
ALLEGRO_FONT *font ;
ALLEGRO_BITMAP * eatmore;
ALLEGRO_BITMAP * board;
//**

/*
   [GameScene function]
*/
Scene *New_GameScene(int label)
{
    GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
    Scene *pObj = New_Scene(label);
    
    // setting derived object member
    pDerivedObj->background = al_load_bitmap("assets/img/background.png");
    pObj->pDerivedObj = pDerivedObj;
    // register element

    _Register_elements(pObj, New_Character(Character_L));
    // setting derived object function
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;
    //** Start the timer(in second)
    start = clock();
    interval=1;
    memset(score, 0, sizeof(score));
    currentscore=0;
    counter=0;
    gameround=1;
    gamepause=0;
    EachRoundAmount=30;
    font = al_load_ttf_font("assets/font/pirulen.ttf", 12, 0);
    eatmore = al_load_bitmap("assets/img/EatDark.png");
    board= al_load_bitmap("assets/img/Board.png");
    eatmoreWidth=al_get_bitmap_width(eatmore);
    eatmoreHeight=al_get_bitmap_height(eatmore);
    boardWidth=al_get_bitmap_width(board);
    //**

    return pObj;
}
void game_scene_update(Scene *self)
{
    ElementVec allEle = _Get_all_elements(self);
if(!gamepause){
    //Random generation of game instance
        end = clock();
    if(((double)(end-start)/CLOCKS_PER_SEC)>interval&&counter<EachRoundAmount){
    //restart timer
        start= clock();
        Elements *Pro;
        int ProType=(rand()+(int)end)%gameround;
        Pro = New_Projectile(Projectile_L,50+(rand()+(int)end)%(WIDTH-150),-50,1,ProType);
        counter++;
        _Register_elements(self, Pro);
        //**
        if(!(counter%5)&&interval>Min_Interval)
            interval-=0.1;
    }
    //**
    //**Update game round and stop the game before creating new instance
    if((end-start)/ CLOCKS_PER_SEC>2){ //No new instance for 2 unit of time > gameround++     
        currentscore+=score[gameround];
        gameround++;
        interval=1;
        gamepause=1;
        counter=0;
        return;
    }
    // run interact for every element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        // run every interact object
        for (int j = 0; j < ele->inter_len; j++)
        {
            int inter_label = ele->inter_obj[j];
            ElementVec labelEle = _Get_label_elements(self, inter_label);
            for (int i = 0; i < labelEle.len; i++)
            {
                ele->Interact(ele, labelEle.arr[i]);
            }
        }
    }
}//if the game is pause> waiting for continue
else{
    ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);
    //if (mouse is inside the "eatmore" icon)
    if (mouse.x>(WIDTH-eatmoreWidth)/2 &&mouse.x<(WIDTH+eatmoreWidth)/2&&mouse.y>450&&mouse.y<450+eatmoreHeight){
        eatmore = al_load_bitmap("assets/img//EatLight.png");
        if (state.buttons&1) //if (mouse:left click)
        {
            if(gameround==6){ 
                self->scene_end = true;
                window = 0;
                Gamefinished=1;
                return;
            }
            gamepause=0;
            return;
        }
    }
    else
        eatmore = al_load_bitmap("assets/img//EatDark.png");
}
    // remove element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->dele)
            _Remove_elements(self, ele);
    }
    // update every element
    for (int i = 0; i < allEle.len; i++)
    {
        allEle.arr[i]->Update(allEle.arr[i]);
    }
}
void game_scene_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    al_draw_bitmap(gs->background, 0, 0, 0);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Draw(ele);
    }
    if(gamepause){
        al_draw_bitmap(board,  (WIDTH-boardWidth)/2,100, 0);
        al_draw_bitmap(eatmore,  (WIDTH-eatmoreWidth)/2,450, 0);
        for(int i=1;i<gameround;i++)
            al_draw_textf(font , al_map_rgb(255 , 0 , 0) ,375 ,185+(i-1)*42 , ALLEGRO_ALIGN_LEFT , "%d/%d" , score[i],EachRoundAmount);
        al_draw_textf(font , al_map_rgb(255 , 0 , 0) ,375 ,420 , ALLEGRO_ALIGN_LEFT , "%d/%d" ,currentscore,EachRoundAmount*5);
    }
    else
        al_draw_textf(font , al_map_rgb(255 , 0 , 0) , WIDTH-150 , HEIGHT-50 , ALLEGRO_ALIGN_LEFT , "score: %d/%d" , score[gameround],EachRoundAmount);

}
void game_scene_destroy(Scene *self)
{
    GameScene *Obj = ((GameScene *)(self->pDerivedObj));
    ALLEGRO_BITMAP *background = Obj->background;
    al_destroy_bitmap(background);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Destroy(ele);
    }
    free(Obj);
    free(self);
}
