#ifndef MAGNETIC_DATA_H
#define MAGNETIC_DATA_H


#define _USE_MATH_DEFINES
#include <math.h>
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include "assert.h"

#define MAX_BINARY_BUFFER 32

typedef struct magnetic_data
{
    int binaryBuffer[MAX_BINARY_BUFFER];
    size_t bit_size;
    int decimal_value;
    int target_value;
} magnetic_data_t;

 
void magnetic_data_set_decimal(magnetic_data_t* data);
magnetic_data_t magnetic_data_create(size_t N_bits, int target_value);
bool magnetic_data_is_target(const magnetic_data_t* data);


#endif