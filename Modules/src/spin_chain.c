#include "Modules/spin_chain.h"


static bool periodic_boundary = false;
static float interaction      = 1.0f;

static float _get_deltaE(const spin_chain_t* my_chain, size_t i, float dtheta);
static bool _accept_proposal(float dE, float temp);


void spin_chain_set_interaction(float J)
{
    interaction = J;
}
void spin_chain_set_periodic_boundary(bool val)
{
    periodic_boundary = val;
}

void spin_chain_init(spin_chain_t* my_chain, size_t N_atom)
{
    float* _spins = calloc(N_atom, sizeof(float));
    if (_spins == NULL)
    {
        fprintf(stderr, "alloc fail blabla");
        return;
    }
    for (size_t i = 0; i < N_atom; i++)
    {
        _spins[i] = SPIN_UP * (float)rand() / RAND_MAX;
    }
    my_chain->spins        = _spins;
    my_chain->N            = N_atom;
    my_chain->energy       = 0.0f;
    my_chain->chain_length = (N_atom-1)*inter_atomic;
}

void camera_init(Camera2D* camera, const spin_chain_t* chain)
{
    camera->target.x   = chain->chain_length / 2.0f;
    camera->target.y = 0;
    camera->offset   = (Vector2){ (float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f};
    camera->rotation = 0.0f;
    camera->zoom     = 0.9 * (float)GetScreenWidth() / chain->chain_length;
}

void spin_chain_release(spin_chain_t* my_chain)
{
    free(my_chain->spins);
}

void spin_polarize_state(spin_chain_t* my_chain)
{
    for (size_t i = 0; i < my_chain->N; i++)
    {
        my_chain->spins[i] = SPIN_UP;
    }
}
void spin_chain_set_all(spin_chain_t* my_chain, float angle)
{
    for (size_t i = 0; i < my_chain->N; i++)
    {
        my_chain->spins[i] = angle;
    }
}



void spin_chain_set_energy(spin_chain_t* my_chain)
{
    size_t last_index = my_chain->N-1;
    float E = 0;
    for (size_t i = 0; i < last_index; i++)
    {
        E += -interaction * cos(my_chain->spins[i] - my_chain->spins[i+1]);        
    }
    my_chain->energy = E;
}

float modulo_2PI(float theta)
{
    theta = fmodf(theta, 2.0f*M_PI);
    while (theta < 0)
    {
        theta += 2.0f*M_PI;
    }
    return theta;
};
float modulo_2PI_recursive(float theta)
{
    theta = fmodf(theta, 2.0f*M_PI);
    if (theta < 0)
    {
        theta += 2.0f*M_PI;
        modulo_2PI_recursive(theta);
    }
    return theta;
};

static float _get_deltaE(const spin_chain_t* my_chain, size_t i, float dtheta)
{
    float E_R = 0;
    float E_L = 0;
    float E_R_new = 0;
    float E_L_new = 0;
    float theta_0 = my_chain->spins[i];
    if (i != my_chain->N-1)
    {
        E_R     = -interaction * cos(theta_0        - my_chain->spins[i+1]); 
        E_R_new = -interaction * cos(theta_0+dtheta - my_chain->spins[i+1]); 
    }
    if(periodic_boundary && i == my_chain->N-1)
    {
        E_R     = -interaction * cos(theta_0        - my_chain->spins[0]); 
        E_R_new = -interaction * cos(theta_0+dtheta - my_chain->spins[0]); 
    }
    if (i != 0)
    {
        E_L     = -interaction * cos(theta_0        - my_chain->spins[i-1]); 
        E_L_new = -interaction * cos(theta_0+dtheta - my_chain->spins[i-1]); 
    }
    return (E_R_new + E_L_new) - (E_R + E_L);
}


static bool _accept_proposal(float dE, float temp)
{
    float r     = (float)rand() / RAND_MAX;
    float beta  = (temp == 0)? MAX_BETA : 1.f/temp; 
    float proba = exp(-dE*beta);
    return ( (dE < 0) || (r < proba) );
}

void spin_chain_update_monte_carlo(spin_chain_t* my_chain, float temperature)
{
    for (size_t i = 0; i < my_chain->N; i++)
    {
        size_t index = rand() % my_chain->N;
        float dtheta = (1.0f * M_PI / 48.0f) * (  1.0f - 2.0f * (float)rand()/RAND_MAX );

        float dE = _get_deltaE(my_chain, index, dtheta);
        if( _accept_proposal(dE, temperature))
        {
            my_chain->spins[index] = modulo_2PI(my_chain->spins[index] + dtheta);
            my_chain->energy += dE;
        }
    }
}


float spin_chain_get_block_magnetizaiton(const spin_chain_t* chain, size_t block_index, size_t block_width)
{
    float spin_Y = 0.0f;
    if(block_index*block_width + block_width >= chain->N)
    {
        fprintf(stderr, "spin_chain_get_block_magnetizaiton: overflow [N=%zu/block_width=%zu/index=%zu]\n", chain->N,
                                                                                                            block_width,
                                                                                                            block_index*block_width + block_width);
        exit(EXIT_FAILURE);
    }
    for (size_t i = block_index*block_width; i < block_index*block_width + block_width; i++)
    {
        spin_Y += sin(chain->spins[i]);
    }
    return spin_Y/(float)block_width;
}