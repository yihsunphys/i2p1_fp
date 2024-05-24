#include <allegro5/allegro_primitives.h>
#include "menu.h"
#include <stdbool.h>
/*
   [Menu function]
*/
Scene *New_Menu(int label)
{
    Menu *pDerivedObj = (Menu *)malloc(sizeof(Menu));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 12, 0);
    // Load sound
    pDerivedObj->song = al_load_sample("assets/sound/menu.mp3");
    al_reserve_samples(20);
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    pDerivedObj->img= al_load_bitmap("assets/img/StartDark.png");
    pDerivedObj->imgControl = al_load_bitmap("assets/img/Control.png");
    pDerivedObj->title_width= al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->title_height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->control_width= al_get_bitmap_width(pDerivedObj->imgControl);
    pDerivedObj->title_x = (WIDTH -pDerivedObj->title_width)/2;
    pDerivedObj->title_y = (HEIGHT-pDerivedObj->title_height)/2+200;

    // Loop the song until the display closes
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.1);
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = menu_update;
    pObj->Draw = menu_draw;
    pObj->Destroy = menu_destroy;
    return pObj;
}
void menu_update(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);
    //If mouse.position is inside the title
    if (mouse.x>Obj->title_x && mouse.x<(Obj->title_x+Obj->title_width) && mouse.y>Obj->title_y&& mouse.y<(Obj->title_y+Obj->title_height)){
        Obj->img = al_load_bitmap("assets/img/StartLight.png");
        if (state.buttons & 1) //if (mouse:left click)
        {
            self->scene_end = true;
            window = 1;
        }
    }
    else
        Obj->img = al_load_bitmap("assets/img/StartDark.png");
    return;
}
void menu_draw(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    al_draw_bitmap(Obj->img, Obj->title_x, Obj->title_y, 0);
    al_draw_bitmap(Obj->imgControl, (WIDTH-Obj->control_width)/2, Obj->title_y-300, 0);
    al_play_sample_instance(Obj->sample_instance);
}
void menu_destroy(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    free(Obj);
    free(self);
}
