#include "game_module.h"

float inter_atomic; 
void set_inter_atomic(float x)
{
    inter_atomic = x;
} 

Vector2 get_nano_world_size(const Camera2D* camera)
{
    Vector2 screen_bottom_right = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    Vector2 screen_top_left     = {0.0f, 0.0f};

    Vector2 nano_bottom_right = GetScreenToWorld2D(screen_bottom_right, *camera);
    Vector2 nano_top_left     = GetScreenToWorld2D(screen_top_left,     *camera);

    return (Vector2){(nano_bottom_right.x - nano_top_left.x), (nano_bottom_right.y - nano_top_left.y)};
}
void graph_draw(const graph_t* my_graph)
{
    DrawLine(0,  0,                my_graph->width, 0,                 my_graph->axis_color);
    DrawLine(0,  my_graph->height, my_graph->width,  my_graph->height, my_graph->axis_color);
    DrawLine(0, -my_graph->height, my_graph->width, -my_graph->height, my_graph->axis_color);
}

void domain_wall_draw(const domain_wall_t* domain_wall)
{
    Rectangle temp_rec = domain_wall->rec;
    for (size_t i = 0; i < domain_wall->N_walls; i++)
    {
        DrawRectangleRec(temp_rec, domain_wall->color);
        temp_rec.x += 2*domain_wall->rec.width; 
    }
}
