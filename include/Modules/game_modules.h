#ifndef GAME_MODULE_H
#define GAME_MODULE_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include "assert.h"


typedef enum game_mode
{
    SETUP = 0,
    START,
} game_mode;

typedef struct domain_wall
{
    Rectangle rec;
    Color color;
    size_t N_walls;
    float domain_width;
} domain_wall_t;

typedef struct graph
{
    Color axis_color;
    Color circle_color;
    float width;
    float height;
    float circle_radius;
} graph_t;

void set_inter_atomic(float x);
Vector2 get_nano_world_size(const Camera2D* camera);
void graph_draw(const graph_t* my_graph);
void domain_wall_draw(const domain_wall_t* domain_wall);

#endif