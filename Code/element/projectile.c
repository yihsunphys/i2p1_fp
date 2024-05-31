#include "projectile.h"
#include "../shapes/Circle.h"
#include "../scene/gamescene.h"
/*
   [Projectile function]
*/
//**
const char *ProjectileTypeaArray[] = {
    "meat", 
    "fish", 
    "vegetable", 
    "banana", 
    "milk", 
};
//**
ALLEGRO_SAMPLE *song;
ALLEGRO_SAMPLE *song2;
ALLEGRO_SAMPLE_INSTANCE *sample_instance;
ALLEGRO_SAMPLE_INSTANCE *sample_instance2;
Elements *New_Projectile(int label, int x, int y,double v,int type)
{   
    song = al_load_sample("assets/sound/eat_correct.mp3");
    sample_instance =  al_create_sample_instance(song);
    al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_ONCE);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());

    song2 = al_load_sample("assets/sound/eat_wrong.mp3");
    sample_instance2 =  al_create_sample_instance(song2);
    al_set_sample_instance_playmode(sample_instance2, ALLEGRO_PLAYMODE_ONCE);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(sample_instance2, al_get_default_mixer());
    Projectile *pDerivedObj = (Projectile *)malloc(sizeof(Projectile));
    Elements *pObj = New_Elements(label);
    // setting derived object member

    //**
    char Buffer[30];
    sprintf(Buffer,"assets/img/%s.png",ProjectileTypeaArray[type]);
    //printf("%s", Buffer);
    pDerivedObj->img = al_load_bitmap(Buffer);
    //**
    
    pDerivedObj->ProjectileType = type;
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = x;
    pDerivedObj->y = y;
    pDerivedObj->v = v;
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x + pDerivedObj->width / 2,
                                     pDerivedObj->y + pDerivedObj->height / 2,
                                     min(pDerivedObj->width, pDerivedObj->height) / 2);
    // setting the interact object
    //**
    pObj->inter_obj[pObj->inter_len++] = Character_L;
    //**
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Projectile_update;
    pObj->Interact = Projectile_interact;
    pObj->Draw = Projectile_draw;
    pObj->Destroy = Projectile_destory;
   
    return pObj;
}
void Projectile_update(Elements *self)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
     
    if(Gamefinished) Obj->v+=0.5;
    else Obj->v+=0.15;
    _Projectile_update_position(self, 0, Obj->v);
}
void _Projectile_update_position(Elements *self, int dx, int dy)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    Obj->x += dx;
    Obj->y += dy;
    Shape *hitbox = Obj->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
    //**
    if(Obj->y>HEIGHT)
        self->dele = true;
    //**
}
void Projectile_interact(Elements *self, Elements *tar)
{

    
    if (tar->label == Character_L)
    {   
        Projectile *Obj = ((Projectile *)(self->pDerivedObj));
        Character *chara = ((Character *)(tar->pDerivedObj));
        if(chara->hitbox->overlap(chara->hitbox, Obj->hitbox) &&
           chara->ProjectileType == Obj->ProjectileType && Obj->y>400
           ){   
                score[gameround]++;
                chara->state = EAT;
                self->dele = true;
                al_play_sample_instance(sample_instance);
        }
        else if(chara->hitbox->overlap(chara->hitbox, Obj->hitbox) &&
                chara->ProjectileType != Obj->ProjectileType &&
                Obj->y>400
           ){   
                chara->state = EAT_WRONG;
                al_play_sample_instance(sample_instance2);
        }

    }
}
void Projectile_draw(Elements *self)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    if (Obj->v > 0)
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, ALLEGRO_FLIP_HORIZONTAL);
    else
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}
void Projectile_destory(Elements *self)
{
    Projectile *Obj = ((Projectile *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
