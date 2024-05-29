#include "gamescene.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
//**  For Timer and Random functions
#define Min_Interval 0.2
#include <time.h>
#include <stdlib.h>
clock_t start, end;
double interval;
int counter;
ALLEGRO_FONT *font ;

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
    _Register_elements(pObj, New_Floor(Floor_L));
    _Register_elements(pObj, New_Teleport(Teleport_L));
    _Register_elements(pObj, New_Tree(Tree_L));
    _Register_elements(pObj, New_Character(Character_L));
    // setting derived object function
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;
    //// Start the timer(in second)
    start = clock()/ CLOCKS_PER_SEC;
    interval=0.6;
    counter=0;
    gameround=5;
    font = al_load_ttf_font("assets/font/pirulen.ttf", 12, 0);
    ////
    return pObj;
}
void game_scene_update(Scene *self)
{
    //**Random generation
    end = clock()/ CLOCKS_PER_SEC;;
    if((double)(end-start)>interval){
        //restart timer
        start= clock()/ CLOCKS_PER_SEC;
        Elements *Pro;
        int ProType=(rand()+(int)end)%gameround;
        Pro = New_Projectile(Projectile_L,(rand()+(int)end)%WIDTH,-50,1,ProType);
        counter++;
        _Register_elements(self, Pro);
        if(!(counter%10)&&interval>Min_Interval)
            interval-=0.2;
    }
    //**

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
    
    al_draw_textf(font , al_map_rgb(255 , 0 , 0) , 500 , 100 , ALLEGRO_ALIGN_LEFT , "score: %d" , score[gameround]);
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
