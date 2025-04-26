#include "Modules/stm.h"

stm_t stm_create(size_t N_atom_width, float heigth, float tip_height, Color color)
{
    stm_t stm = 
    {
        .rec          = {0.0f},
        .atom_current = 0,
        .atom_width   = N_atom_width,
        .height       = heigth,
        .tip_height   = tip_height,
        .orientation  = STM_UP,
        .color        = color
    };

    float width       = N_atom_width * inter_atomic;
    Rectangle stm_rec =
    {
        .height = stm.height,
        .width  = width,
        .x      = stm.atom_current * inter_atomic - 0.5*inter_atomic,
        .y      = stm.tip_height - stm.height
    };
    stm.rec = stm_rec;
    
    return stm;
}
void stm_init_state(stm_t* stm)
{
    stm->atom_current = 0;
    stm->orientation  = STM_UP;
}

void stm_set_rect(stm_t* stm)
{
    float width       = stm->atom_width * inter_atomic;
    Rectangle stm_rec =
    {
        .height = stm->height,
        .width  = width,
        .x      = stm->atom_current * inter_atomic - 0.5*inter_atomic,
        .y      = stm->tip_height - stm->height
    };
    stm->rec = stm_rec;
}
 

void stm_send_magnetic_pulse(const stm_t* stm, spin_chain_t* chain)
{
    for (size_t i = stm->atom_current; i < stm->atom_current + stm->atom_width; i++)
    {
        if (i >= chain->N)
        {
            break;
        }

        switch (stm->orientation)
        {
            case STM_UP:
                chain->spins[i] = SPIN_UP; 
                break;
            case STM_DOWN:
                chain->spins[i] = SPIN_DOWN; 
                break;
        }
    }
}

static void _stm_rec_position_update(stm_t* stm)
{
    stm->rec.x = stm->atom_current * inter_atomic - 0.5*inter_atomic;
}

void stm_move_left(stm_t* stm)
{
    if (stm->atom_current >= stm->atom_width)
    {
        stm->atom_current -= stm->atom_width;
    }
    _stm_rec_position_update(stm);
}

void stm_move_right(stm_t* stm)
{
    stm->atom_current += stm->atom_width;
    _stm_rec_position_update(stm);
}


void stm_draw(const stm_t* stm)
{
    Vector2 topV    = {stm->rec.x +  stm->rec.width/2, stm->rec.y};
    Vector2 bottomV = {stm->rec.x +  stm->rec.width/2, stm->rec.y + stm->rec.height};
    float delta_y   = 0.4 * stm->rec.height;
    float delta_x   = 0.4 * stm->rec.width;
    
    DrawRectangleRec(stm->rec, stm->color);

    switch (stm->orientation)
    {
        case STM_UP:
            DrawTriangle((Vector2){topV.x, topV.y},
                        (Vector2){topV.x-delta_x, topV.y+delta_y},
                        (Vector2){topV.x+delta_x, topV.y+delta_y},
                        BLACK);
            DrawLineEx((Vector2){topV.x, topV.y + delta_y},
                        bottomV,
                        stm->line_thickess,
                        BLACK);
            break;
        case STM_DOWN:
            DrawTriangle((Vector2){bottomV.x, bottomV.y},
                        (Vector2){bottomV.x+delta_x, bottomV.y-delta_y},
                        (Vector2){bottomV.x-delta_x, bottomV.y-delta_y},
                        BLACK);
            DrawLineEx(topV,
                        (Vector2){bottomV.x, bottomV.y - delta_y},
                        stm->line_thickess,
                        BLACK);
            break;
    }
}

void stm_draw2(const stm_t* stm, float thickness)
{
    float width       = stm->atom_width * inter_atomic;
    Rectangle stm_rec =
    {
        .height = stm->height,
        .width  = width,
        .x      = stm->atom_current * inter_atomic - 0.5*inter_atomic,
        .y      = stm->tip_height - stm->height
    };

    Vector2 topV    = {stm_rec.x +  width/2, stm_rec.y};
    Vector2 bottomV = {stm_rec.x +  width/2, stm_rec.y + stm_rec.height};
    float delta_y   = 0.4 * stm_rec.height;
    float delta_x   = 0.4 * stm_rec.width;
    
    DrawRectangleRec(stm_rec, stm->color);

    switch (stm->orientation)
    {
        case STM_UP:
            DrawTriangle((Vector2){topV.x, topV.y},
                        (Vector2){topV.x-delta_x, topV.y+delta_y},
                        (Vector2){topV.x+delta_x, topV.y+delta_y},
                        BLACK);
            DrawLineEx((Vector2){topV.x, topV.y + delta_y},
                        bottomV,
                        thickness,
                        BLACK);
            break;
        case STM_DOWN:
            DrawTriangle((Vector2){bottomV.x, bottomV.y},
                        (Vector2){bottomV.x+delta_x, bottomV.y-delta_y},
                        (Vector2){bottomV.x-delta_x, bottomV.y-delta_y},
                        BLACK);
            DrawLineEx(topV,
                        (Vector2){bottomV.x, bottomV.y - delta_y},
                        thickness,
                        BLACK);
            break;
    }
}