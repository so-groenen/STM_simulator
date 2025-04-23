#include "magnetic_data.h"



void magnetic_data_set_decimal(magnetic_data_t* data)
{
    int decimal_value = 0;
    int power_of_two  = 1;
    for (size_t n = 0; n < data->bit_size; n++)
    {
        decimal_value += data->binaryBuffer[data->bit_size - n - 1] * power_of_two;
        power_of_two  *= 2; 
    }
    data->decimal_value  = decimal_value;
}


magnetic_data_t magnetic_data_create(size_t N_bits, int target_value)
{
    assert(N_bits <= MAX_BINARY_BUFFER && "Binary buffer must be bigger than available magnetic blocks");
    magnetic_data_t data = 
    {
        .binaryBuffer = {1},
        .bit_size      = N_bits,
        .decimal_value = 0,
        .target_value  = target_value,
    };
    magnetic_data_set_decimal(&data);
    return data;
}

bool magnetic_data_is_target(const magnetic_data_t* data)
{
    return (data->decimal_value == data->target_value);
}

