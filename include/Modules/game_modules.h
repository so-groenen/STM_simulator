#ifndef GAME_MODULE_H
#define GAME_MODULE_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include "assert.h"


typedef enum game_play_mode
{
    SETUP_SPINS = 0,
    PROTECT_SPINS,
} GamePlayMode;

typedef struct domain_wall
{
    Rectangle rec;
    Color color;
    size_t N_walls;
    float domain_width;
} DomainWall;

typedef struct graph
{
    Color axis_color;
    Color circle_color;
    float width;
    float height;
    float circle_radius;
} Graph;

void set_inter_atomic(float x);
Vector2 get_nano_world_size(const Camera2D* camera);
void graph_draw(const Graph* my_graph);
void domain_wall_draw(const DomainWall* domain_wall);

#endif