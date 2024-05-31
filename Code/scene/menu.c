#include <allegro5/allegro_primitives.h>
#include "menu.h"
#include <stdbool.h>
#include "gamescene.h"
/*
   [Menu function]
*/

ALLEGRO_SAMPLE *song2;
ALLEGRO_SAMPLE_INSTANCE *sample_instance2;
Scene *New_Menu(int label)
{   al_destroy_sample(gamescene_song);
    al_destroy_sample_instance(gamescene_sample_instance);
    Menu *pDerivedObj = (Menu *)malloc(sizeof(Menu));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 12, 0);
    // Load sound
    pDerivedObj->song = al_load_sample("assets/sound/Splash_Around_cut.mp3");
    al_reserve_samples(20);
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    pDerivedObj->start= al_load_bitmap("assets/img/StartDark.png");
    pDerivedObj->imgControl = al_load_bitmap("assets/img/Control.png");
    pDerivedObj->title = al_load_bitmap("assets/img/title.png");
    if(Gamefinished)
        pDerivedObj->background = al_load_bitmap("assets/img/endsecne.png");
    else
        pDerivedObj->background = al_load_bitmap("assets/img/background.png");
    pDerivedObj->start_width= al_get_bitmap_width(pDerivedObj->start);
    pDerivedObj->start_height = al_get_bitmap_height(pDerivedObj->start);
    pDerivedObj->control_width= al_get_bitmap_width(pDerivedObj->imgControl);
    pDerivedObj->title_width=al_get_bitmap_width(pDerivedObj->title);
    pDerivedObj->start_x = (WIDTH -pDerivedObj->start_width)/2;
    pDerivedObj->start_y = (HEIGHT-pDerivedObj->start_height)/2+150;

    // Loop the song until the display closes
    song2 = al_load_sample("assets/sound/background.mp3");
    sample_instance2 =  al_create_sample_instance(song2);
    al_set_sample_instance_playmode(sample_instance2, ALLEGRO_PLAYMODE_ONCE);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(sample_instance2, al_get_default_mixer());
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
    //If mouse.position is inside the start
    if (mouse.x>Obj->start_x && mouse.x<(Obj->start_x+Obj->start_width) && mouse.y>Obj->start_y&& mouse.y<(Obj->start_y+Obj->start_height)){
        Obj->start = al_load_bitmap("assets/img/StartLight.png");
        if (state.buttons & 1) //if (mouse:left click)
        {
            self->scene_end = true;
            window = 1;
        }
    }
    else
        Obj->start = al_load_bitmap("assets/img/StartDark.png");
    return;
}
void menu_draw(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    al_draw_bitmap(Obj->background, 0, 0, 0);
    al_draw_bitmap(Obj->start, Obj->start_x, Obj->start_y, 0);
    al_draw_bitmap(Obj->imgControl, (WIDTH-Obj->control_width)/2, Obj->start_y-300, 0);
    al_draw_bitmap(Obj->title, (WIDTH-Obj->title_width)/2, Obj->start_y-400, 0);
    al_play_sample_instance(sample_instance2);
}
void menu_destroy(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(sample_instance2);
    free(Obj);
    free(self);
}
