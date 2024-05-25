#ifndef Projectile_H_INCLUDED
#define Projectile_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
/*
   [Projectile object]
*/
//** All the type of projectile
typedef enum ProjectileType
{
    RED=0,
    BLUE,
    GREEN,
    YELLOW,
    WHITE,
} ProjectileType;
//**

typedef struct _Projectile
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    int ProjectileType;
    //**
    double v;             // the velocity of projectile
    //**
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
} Projectile;
Elements *New_Projectile(int label, int x, int y, double v,int type);
void Projectile_update(Elements *self);
void Projectile_interact(Elements *self, Elements *tar);
void Projectile_draw(Elements *self);
void Projectile_destory(Elements *self);
void _Projectile_update_position(Elements *self, int dx, int dy);
#endif
