#include "gamescene.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
//**  For Timer and Random functions
#define Min_Interval 0.2
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
    start = clock()/ CLOCKS_PER_SEC;
    interval=0.2;
    counter=0;
    gameround=1;
    gamepause=0;
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
if(!gamepause){
    //Random generation of game instance
        end = clock()/ CLOCKS_PER_SEC;
    if((double)(end-start)>interval&&counter<30){
    //restart timer
        start= clock()/ CLOCKS_PER_SEC;
        Elements *Pro;
        int ProType=(rand()+(int)end)%gameround;
        Pro = New_Projectile(Projectile_L,(rand()+(int)end)%WIDTH,-50,1,ProType);
        counter++;
        _Register_elements(self, Pro);
        //**
        if(!(counter%5)&&interval>Min_Interval)
            interval-=0.15;
    }
    //**
    //**Update game round and stop the game before creating new instance
    if(end-start>3){ //No new instance for 3 unit of time > gameround++     
        gameround++;
        interval=1;
        gamepause=1;
        counter=0;
        return;
    }
    



    // update every element
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        allEle.arr[i]->Update(allEle.arr[i]);
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
    // remove element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->dele)
            _Remove_elements(self, ele);
    }
}//if the game is pause> waiting for continue
else{
    ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);
    if (mouse.x>(WIDTH-eatmoreWidth)/2 &&mouse.x<(WIDTH+eatmoreWidth)/2&&mouse.y>450&&mouse.y<450+eatmoreHeight){
        eatmore = al_load_bitmap("assets/img//EatLight.png");
        if (state.buttons & 1) //if (mouse:left click)
        {
            gamepause=0;
            return;
        }
    }
    else
        eatmore = al_load_bitmap("assets/img//EatDark.png");
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
    }
    else
        al_draw_textf(font , al_map_rgb(255 , 0 , 0) , WIDTH-100 , HEIGHT-50 , ALLEGRO_ALIGN_LEFT , "score: %d/30" , score[gameround]);

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
