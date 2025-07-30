#ifndef STM_H
#define STM_H


#define _USE_MATH_DEFINES
#include <math.h>
#include "raylib.h"
#include <stdint.h>
#include "Modules/spin_chain.h"

typedef bool polarization;
#define STM_UP true
#define STM_DOWN false

extern float inter_atomic;

typedef struct scanning_tip_microscope
{
    Rectangle rec;
    Color color;
    size_t atom_width;
    size_t atom_current;
    float height;
    float tip_height;
    polarization orientation;
    float line_thickess;
} Stm;

Stm stm_create(size_t N_atom_width, float heigth, float tip_height, Color color);
void stm_send_magnetic_pulse(const Stm* stm, SpinChain* chain);
void stm_move_left(Stm* stm);
void stm_move_right(Stm* stm);
void stm_init_state(Stm* stm);
void stm_set_rect(Stm* stm);
void stm_draw(const Stm* stm);

#endif
