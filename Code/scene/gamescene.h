#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"
#include "../element/element.h"
#include "../element/charater.h"
#include "../element/floor.h"
#include "../element/teleport.h"
#include "../element/tree.h"
#include "../element/projectile.h"
/*
   [game scene object]
*/
typedef enum EleType
{
    Floor_L,
    Teleport_L,
    Tree_L,
    Character_L,
    Projectile_L
} EleType;
typedef struct _GameScene
{
    ALLEGRO_BITMAP *background;

} GameScene;
Scene *New_GameScene(int label);
void game_scene_update(Scene *self);
void game_scene_draw(Scene *self);
void game_scene_destroy(Scene *self);
int score[6]; //5 stage 1~5
int currentscore;
int gameround; //stage

clock_t start, end;
double interval;
int counter;
int eatmoreWidth,eatmoreHeight;
int boardWidth;
int gamepause;
int EachRoundAmount;
ALLEGRO_SAMPLE *gamescene_song;
ALLEGRO_SAMPLE_INSTANCE *gamescene_sample_instance;
#endif
