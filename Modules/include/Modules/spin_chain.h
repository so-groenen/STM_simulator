#ifndef SPIN_CHAIN_H
#define SPIN_CHAIN_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define SPIN_UP     (        M_PI / 2.0f)
#define SPIN_DOWN  ( -1.0f * M_PI / 2.0f)
#define MAX_BETA    1E9f


extern float inter_atomic;

typedef struct spin_chain
{
    float* spins;
    size_t N;
    float energy;
    float chain_length;
} spin_chain_t;


void spin_chain_set_interaction(float J);
void spin_chain_set_periodic_boundary(bool val);
void spin_chain_init(spin_chain_t* my_chain, size_t N_atom);
void camera_init(Camera2D* camera, const spin_chain_t* chain);
void spin_chain_release(spin_chain_t* my_chain);
void spin_polarize_state(spin_chain_t* my_chain);
void spin_chain_set_energy(spin_chain_t* my_chain);
void spin_chain_set_all(spin_chain_t* my_chain, float angle);

float modulo_2PI(float theta);
void spin_chain_update_monte_carlo(spin_chain_t* my_chain, float temperature);
float spin_chain_get_block_magnetizaiton(const spin_chain_t* chain, size_t block_index, size_t block_width);


#endif