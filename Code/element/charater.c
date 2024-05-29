#include "charater.h"
#include "../scene/sceneManager.h"
#include "projectile.h"
#include "../shapes/Rectangle.h"
#include "../algif5/src/algif.h"
#include <stdio.h>
#include <stdbool.h>
/*
   [Character function]
*/
int timer = 15;
ALLEGRO_BITMAP *imgData[5][10];
Elements *New_Character(int label)
{
    Character *pDerivedObj = (Character *)malloc(sizeof(Character));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load character images
    // initial the geometric information of new_character
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/atk_sound.wav");
    pDerivedObj->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_Sound, al_get_default_mixer());

    // initial the geometric information of character
    
    char state_string[5][10] = {"red", "blue", "green", "yellow", "white"};
    char buffer[100];
    for(int i = 0; i < 5; i++){
      for(int j = 1; j <= 5; j++){
        sprintf(buffer, "assets/img/%s%d.png", state_string[i], j);
        imgData[i][j] = al_load_bitmap(buffer);
        }
    }
    

    pDerivedObj->img = al_load_bitmap("assets/img/red1.png");
    pDerivedObj->ProjectileType = RED; // red, blue, green, yellow, white 
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = 300;
    pDerivedObj->y = 500;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
    pDerivedObj->dir = false; // true: face to right, false: face to left
    // initial the animation component
    pDerivedObj->state = 0;
    pDerivedObj->new_proj = false;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Character_draw;
    pObj->Update = Character_update;
    pObj->Interact = Character_interact;
    pObj->Destroy = Character_destory;
    return pObj;
}
void Character_update(Elements *self)
{
    Character *chara = ((Character *)(self->pDerivedObj));

    if (key_state[ALLEGRO_KEY_B]){
      chara->ProjectileType = RED;
      chara->img = al_load_bitmap("assets/img/red1.png");
    }
    else if (key_state[ALLEGRO_KEY_T]){
      chara->ProjectileType = BLUE;
      chara->img = al_load_bitmap("assets/img/blue1.png");
    }
    else if (key_state[ALLEGRO_KEY_R]){
      chara->ProjectileType = GREEN;
      chara->img = al_load_bitmap("assets/img/green1.png");
    }
    else if (key_state[ALLEGRO_KEY_E]){
      chara->ProjectileType = YELLOW;
      chara->img = al_load_bitmap("assets/img/yellow1.png");
    }
    else if (key_state[ALLEGRO_KEY_A]){
      chara->ProjectileType = WHITE;
      chara->img = al_load_bitmap("assets/img/white1.png");
    }

    if(mouse.x>WIDTH-chara->width/2||mouse.x<chara->width/2)
        return;
    // use the idea of finite state machine to deal with different state
    _Character_update_position(self,mouse.x-chara->x,0);
    chara->x = mouse.x; 
    

}
void Character_draw(Elements *self)
{
    // with the state, draw corresponding image
    Character *chara = ((Character *)(self->pDerivedObj));

    if(chara->state == NORMAL){
      al_draw_bitmap(chara->img, chara->x-(chara->width)/2, chara->y, 0);
    }
    else if(chara->state == EAT){
      // eat animation
      //al_draw_bitmap(imgData[direction][sprite_pos], x, y, 0);
      if(timer/4%2 == 0) al_draw_bitmap(imgData[chara->ProjectileType][2], chara->x-(chara->width)/2, chara->y, 0);
      else al_draw_bitmap(imgData[chara->ProjectileType][3], chara->x-(chara->width)/2, chara->y, 0);
      if(timer ){timer --;}
      else { chara->state = NORMAL;timer=15;}
    }
    else if(chara->state == EAT_WRONG){
      // eat wrong animation
      if(timer/4%2 == 0) al_draw_bitmap(imgData[chara->ProjectileType][4], chara->x-(chara->width)/2, chara->y-25, 0);
      else al_draw_bitmap(imgData[chara->ProjectileType][5], chara->x-(chara->width)/2, chara->y-25, 0);
      if(timer ){timer --;}
      else { chara->state = NORMAL;timer=15;}
    }
    

}
void Character_destory(Elements *self)
{
    Character *Obj = ((Character *)(self->pDerivedObj));
    al_destroy_sample_instance(Obj->atk_Sound);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}

void _Character_update_position(Elements *self, int dx, int dy)
{
    Character *chara = ((Character *)(self->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

void Character_interact(Elements *self, Elements *tar) {}
