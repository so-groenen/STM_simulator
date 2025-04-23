#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <raylib.h>
#include <stdint.h>
#include <assert.h>

#ifndef M_PI
    #define M_PI 3.141
#endif

#define SPIN_UP    (  M_PI / 2.0f)
#define SPIN_DOWN  ( -M_PI / 2.0f)
#define MAX_BETA    1E9f
#define MAX_BINARY_BUFFER 128

bool periodic_boundary = false;
float inter_atomic     = 0.0f;
float interaction      = 1.0f;

#define set_periodic_boundaries(x) ( periodic_boundary = x  )
#define set_inter_atomic(x)        ( inter_atomic = x)
#define set_interaction(x)         ( interaction = x)
#define LIGHTRED                   (Color){255, 100, 177, 200}        
#define SHOW_SPIN_PROJECTION

typedef enum game_mode
{
    SETUP = 0,
    START,
} game_mode;

typedef struct spin_chain
{
    float* spins;
    size_t N;
    float energy;
} spin_chain_t;
 

typedef bool polarization;
#define STM_UP true
#define STM_DOWN false

typedef struct stm
{
    size_t atom_width;
    size_t atom_current;
    polarization orientation;
    Color color;
    float height;
    float tip_height;
} stm_t;

void stm_init(stm_t* stm)
{
    stm->atom_current  = 0;
    stm->orientation   = STM_UP;
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

void stm_move_left(stm_t* stm)
{
    if (stm->atom_current >= stm->atom_width)
    {
        stm->atom_current -= stm->atom_width;
    }
}

void stm_move_right(stm_t* stm)
{
    stm->atom_current += stm->atom_width;
}


void stm_draw(const stm_t* stm, float thickness)
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

void spin_chain_init(spin_chain_t* my_chain, size_t N_atom, float temp)
{
    float* NewSpins = calloc(N_atom, sizeof(float));
    if (NewSpins == NULL)
    {
        fprintf(stderr, "alloc fail blabla");
        return;
    }
    for (size_t i = 0; i < N_atom; i++)
    {
        NewSpins[i] = SPIN_UP * (float)rand() / RAND_MAX;
    }
    my_chain->spins = NewSpins;
    my_chain->N     = N_atom;
    my_chain->energy            = 0.0F;
}

void camera_init(Camera2D* camera, const spin_chain_t* chain)
{
    float chain_length = (chain->N-1)*inter_atomic;

    camera->target.x   = chain_length / 2.0f;
    camera->target.y = 0;
    camera->offset   = (Vector2){ (float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f};
    camera->rotation = 0.0f;
    camera->zoom     = 0.9 * (float)GetScreenWidth() / chain_length;
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


void write_results(const char* filename, const spin_chain_t* my_chain, float temperature)
{
    FILE* results = fopen(filename, "w");
    fprintf(results, "Results for N=%zu with temp = %.2f\n", my_chain->N, temperature);
    for (size_t i = 0; i < my_chain->N; i++)
    {
        fprintf(results, "spin(%zu) = %.2f\n", i, my_chain->spins[i]);
    }
    fclose(results);
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

float spin_fmodf(float theta, float max)
{
    theta = fmodf(theta, max);
    while (theta < 0)
    {
        theta += max;
    }
    return theta;
};

float get_deltaE(const spin_chain_t* my_chain, size_t i, float dtheta)
{
    float E0 = 0;
    float E1 = 0;
    float E0_new = 0;
    float E1_new = 0;
    float theta_0 = my_chain->spins[i];
    if (i != my_chain->N-1)
    {
        E0     = -interaction * cos(theta_0        - my_chain->spins[i+1]); 
        E0_new = -interaction * cos(theta_0+dtheta - my_chain->spins[i+1]); 
    }
    if(periodic_boundary && i == my_chain->N-1)
    {
        E0     = -interaction * cos(theta_0        - my_chain->spins[0]); 
        E0_new = -interaction * cos(theta_0+dtheta - my_chain->spins[0]); 
    }
    if (i != 0)
    {
        E1     = -interaction * cos(theta_0        - my_chain->spins[i-1]); 
        E1_new = -interaction * cos(theta_0+dtheta - my_chain->spins[i-1]); 
    }
    return (E0_new + E1_new) - (E0 + E1);
}

bool accept_proposal(float dE, float temp)
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

        float dE = get_deltaE(my_chain, index, dtheta);
        if( accept_proposal(dE, temperature))
        {
            my_chain->spins[index] = spin_fmodf(my_chain->spins[index] + dtheta, 2.0f * M_PI);
            my_chain->energy += dE;
        }
    }
}


float spin_chain_get_block_magnetizaiton(const spin_chain_t* chain, size_t block_index, size_t block_width)
{
    float spin_Y = 0.0f;
    if(block_index*block_width + block_width >= chain->N)
    {
        fprintf(stderr, "spin_chain_get_block_magnetizaiton: overflow [N=%zu]", chain->N);
        exit(EXIT_FAILURE);
    }
    for (size_t i = block_index*block_width; i < block_index*block_width + block_width; i++)
    {
        spin_Y += sin(chain->spins[i]);
    }
    // spin_fmodf(avg, 2.0f * M_PI);
    return spin_Y/(float)block_width;
}

Vector2 get_nano_world_size(const Camera2D* camera)
{
    Vector2 screen_bottom_right = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    Vector2 screen_top_left     = {0.0f, 0.0f};

    Vector2 nano_bottom_right = GetScreenToWorld2D(screen_bottom_right, *camera);
    Vector2 nano_top_left     = GetScreenToWorld2D(screen_top_left,     *camera);

    return (Vector2){(nano_bottom_right.x - nano_top_left.x), (nano_bottom_right.y - nano_top_left.y)};
}

int get_decimal_from_binary(const int* bit_buffer, size_t bit_length)
{
    int decimal_value = 0;
    int power_of_two  = 1;
    for (size_t n = 0; n < bit_length; n++)
    {
        decimal_value += bit_buffer[bit_length - n - 1] * power_of_two;
        power_of_two  *= 2; 
    }
    return decimal_value;
}

#define COMMAND_LINE_ARG_COUNT 3

typedef struct
{
    char buffer[64];
    Color color;
    int fontSize;
    int textSize;
    float x;
    float y;
} text_t;

void text_set_size(text_t* text)
{
    text->textSize = MeasureText(text->buffer, text->fontSize);
}
 
text_t text_create(const char* title, int fontSize, Color color)
{
    text_t text =
    {
        .color    = color,
        .fontSize = fontSize,
        .x        = 0,
        .y        = 0,
    };
    strcpy(text.buffer, title);
    text_set_size(&text);
    return text; 
}

void text_set_x_centered(text_t* text, float x)
{
    text->x = x - 0.5*text->textSize;
}
void text_set_x(text_t* text, float x)
{
    text->x = x;
}
void text_set_y(text_t* text, float y)
{
    text->y = y;
}
void text_write(text_t* text, const char* input)
{
    strcpy(text->buffer, input);
    text_set_size(text);
}
void text_draw(const text_t* text)
{
    DrawText(text->buffer, 
            text->x, 
            text->y, 
            text->fontSize,
            text->color);
}


int main(int argc, char** argv)
{
    srand(time(NULL));

    if(argc != COMMAND_LINE_ARG_COUNT)
    {
        fprintf(stderr, "argc must be == %d", COMMAND_LINE_ARG_COUNT);
        exit(EXIT_FAILURE);
    }

    size_t N_atoms        = strtoull(argv[1], NULL, 10);
    size_t STM_atom_width = strtoull(argv[2], NULL, 10);
    float temperature     = 0.0f; 

    set_inter_atomic(100.0f);
    set_periodic_boundaries(false);
    set_interaction(1.0f);

    float chain_length    = (N_atoms-1) * inter_atomic;

    // ------
    spin_chain_t my_chain;
    spin_chain_init(&my_chain, N_atoms, temperature);
    spin_polarize_state(&my_chain);
    spin_chain_set_energy(&my_chain);


    InitWindow(1024, 548, "Spin Chain");

    // ------
    Camera2D camera = {0};
    camera_init(&camera, &my_chain);
    Vector2 nano_world_size = get_nano_world_size(&camera);

    // ------
    stm_t stm = 
    {
        .atom_current = 0,
        .atom_width   = STM_atom_width,
        .height       = nano_world_size.y * 0.20f,
        .tip_height   = - nano_world_size.y * 0.10f,
        .orientation  = STM_UP,
        .color        = BLUE
    };
    float stm_line_thickness = 0.01f* nano_world_size.x;

    // ------
    float SpinRadius    = 0.1f*   nano_world_size.y;
    float CircleRadius  = 0.007f* nano_world_size.y;
    float x0, y0, yf;
    float S_y = 0.0f;;

    // ---------
    float spin_block_width  = inter_atomic * (STM_atom_width-1);
    float domain_wall_start = spin_block_width + 0.5f*inter_atomic;
    size_t N_walls          = N_atoms/(2*STM_atom_width);
    Color domain_wall_color = LIGHTRED;
    Rectangle domain_wall   = 
    {
        .height = nano_world_size.y / 3.0f ,
        .width  = STM_atom_width * inter_atomic,
        .x      = domain_wall_start,
        .y      = - nano_world_size.y / (2 * 3.0f) 
    };

    // ------
    text_t title_setup = text_create("SETUP: Write 42 using the STM tip.", 40, GREEN);
    text_set_x_centered(&title_setup, GetScreenWidth()/2);
    text_set_y         (&title_setup, 10);
 
    text_t title_start = text_create("GO! Protect the data against fluctuations", 40, RED);
    text_set_x_centered(&title_start, GetScreenWidth()/2);
    text_set_y         (&title_start, 10);

    text_t tempText_ = text_create("", 30, BLACK);
    text_set_y         (&tempText_, GetScreenHeight() * 0.1f);

    text_t spinText_ = text_create("", 30, BLACK);
    text_set_y         (&spinText_, GetScreenHeight() * 0.7f);

    text_t binaryText_ = text_create("1", 40, RED);
    text_set_y         (&binaryText_, GetScreenHeight() * 0.75f);

    text_t deciamText_ = text_create("", 40, RED);
    text_set_y         (&deciamText_, GetScreenHeight() * 0.9f);

    //-------
    int binaryBuffer[MAX_BINARY_BUFFER] = {1};
    size_t bit_size   = N_walls;
    assert(bit_size <= MAX_BINARY_BUFFER && "Binary buffer must be bigger than available magnetic blocks");
    int decimal_value = 0;

    game_mode mode = SETUP;
    int target_value = 42;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        if (mode == SETUP && decimal_value == target_value)
        {
            temperature = 10.0f;
            mode        = START; 
        }
        else if (mode == START && decimal_value != target_value)
        {
            break;
        }
        
        // Move STM tip
        if (IsKeyPressed(KEY_LEFT))
        {
            stm_move_left(&stm);
        }
        if (stm.atom_current < my_chain.N-stm.atom_width && IsKeyPressed(KEY_RIGHT))
        {
            stm_move_right(&stm);
        }

        // switch tip orientation
        if(IsKeyPressed(KEY_TAB))
        {
            stm.orientation = !stm.orientation;
        }

        // send pulse
        if(IsKeyPressed(KEY_SPACE))  stm_send_magnetic_pulse(&stm, &my_chain);

        // reset state
        if(IsKeyPressed(KEY_HOME)) spin_polarize_state(&my_chain);


        // update spin chain
        spin_chain_update_monte_carlo(&my_chain, temperature);


        // draw spin chain
        BeginDrawing();
            ClearBackground(WHITE);

            // titles:
            switch (mode)
            {
            case SETUP:
                text_draw(&title_setup);
                break;
            case START:
                text_draw(&title_start);
                break;
            }
            
            // temperature text:
            text_write         (&tempText_, TextFormat("temperature= %.3f", temperature));
            text_set_x_centered(&tempText_, GetScreenWidth()/2);
            text_draw          (&tempText_);

            // local magnetization text & corresponding binary values
            for (size_t n = 0; n < N_walls; n++)
            {
                float spin_block_start      = 2*n * inter_atomic*STM_atom_width;
                float spin_block_mid_point  = spin_block_start +  0.5*spin_block_width;
                float magnetization         = spin_chain_get_block_magnetizaiton(&my_chain, 2*n, STM_atom_width);
                Vector2 block_mid_point_vec = GetWorldToScreen2D((Vector2){spin_block_mid_point, 0}, camera);

                text_write         (&spinText_, TextFormat("%.2f", magnetization));
                text_set_x_centered(&spinText_, block_mid_point_vec.x);
                text_draw          (&spinText_);

                int binary_value  = (int)roundf((magnetization + 1.0f) * 0.5f);

                text_write         (&binaryText_, TextFormat("%d", binary_value));
                text_set_x_centered(&binaryText_, block_mid_point_vec.x);
                text_draw          (&binaryText_);

                binaryBuffer[n] = binary_value;
            }

            // decimal text:
            decimal_value = get_decimal_from_binary(binaryBuffer, bit_size);

            text_write         (&deciamText_, TextFormat("Decimal value: %d", decimal_value));
            text_set_x_centered(&deciamText_, GetScreenWidth()/2.0f);
            text_draw          (&deciamText_);


            // Draw nanostructures & plot:
            BeginMode2D(camera);

                // domain walls:
                domain_wall.x = domain_wall_start;
                for (size_t i = 0; i < N_walls; i++)
                {
                    DrawRectangleRec(domain_wall, domain_wall_color);
                    domain_wall.x += 2*domain_wall.width; 
                }

                // STM:
                stm_draw(&stm, stm_line_thickness);

                // axis:
                DrawLine(0,  0,          chain_length, 0, BLACK);
                DrawLine(0,  SpinRadius, chain_length,  SpinRadius, BLACK);
                DrawLine(0, -SpinRadius, chain_length, -SpinRadius, BLACK);

                // spin values:
                for (size_t i = 0; i < N_atoms; i++)
                {
                    y0     = 0;
                    x0     = i * inter_atomic;
                    S_y    = - SpinRadius * sin(my_chain.spins[i]);
                    yf     = y0 + S_y;
                    DrawCircleV((Vector2){x0, yf}, CircleRadius, RED);
                }
            EndMode2D();
        EndDrawing();       
    }

    // release spin chain
    spin_chain_release(&my_chain);
    CloseWindow();
    printf("\n Learn to play!!!!!!");

    return EXIT_SUCCESS;
}