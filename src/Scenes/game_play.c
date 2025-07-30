#include "Scenes/game_play.h"


//        Static functions
// ================================
static void game_play_init(void); //rename to game_play
static void game_play_run(void);
static void game_play_clean(void);
static void _game_play_update(void);
static void _game_play_draw(void);
// ================================

// ====== global variable ======
Scene game_scene =
{
    .next_screen    = OUTRO,
    .state          = ENTERING,
    .init           = game_play_init,
    .run            = game_play_run,
    .clean          = game_play_clean,
};
// =============================



// ==== static game global variables ====
static float temperature;
static size_t N_atoms;
static size_t STM_atom_width; 

static Camera2D camera;
static Vector2 nano_world_size;
static Graph my_graph;
static SpinChain my_chain;
static Stm stm;
static DomainWall domain_wall;
static Text title_setup;
static Text title_start;
static Text temperatur_text;
static Text spin_text;   
static Text binary_text;
static Text decimal_text;
static MagneticData my_data;
static GamePlayMode game_play_mode;
// =============================


static void game_play_init()
{
    puts("GAME LOG: GAME: init");

    switch (difficulty)
    {
    case EASY:
        {
            N_atoms        = 640;
            STM_atom_width = 40;
        }
        break;
    case MEDIUM:
        {
            N_atoms        = 320;
            STM_atom_width = 20;
        }
        break;
    case HARD:
        {
            N_atoms        = 160;
            STM_atom_width = 10;
        }
        break;
    }
    size_t N_mag_domains  = N_atoms/(2*STM_atom_width);


#ifdef DEBUG // use DEBUG_LOG
    printf("GAME_LOG: N atoms: %zu\n", N_atoms);
    printf("GAME_LOG: N atoms for STM: %zu\n", STM_atom_width);
    printf("GAME_LOG: Number of writable magnetic domains: %zu\n", N_mag_domains);
#endif

    // ==== Spin chain ====
    spin_chain_set_inter_atomic(100.0f);
    spin_chain_set_periodic_boundary(false);
    spin_chain_set_interaction(1.0f);

    spin_chain_init      (&my_chain, N_atoms);
    spin_chain_set_all   (&my_chain, SPIN_DOWN);
    spin_chain_set_energy(&my_chain);


    // ==== graph & camera ====
    float chain_to_screen = 0.9f;           // 1 means the full chain takes up all the screen width;
    spin_chain_set_camera(&camera, &my_chain, chain_to_screen);
    nano_world_size = get_nano_world_size(&camera);
    my_graph = (Graph)
    {
        .axis_color    = BLACK,
        .circle_color  = RED,
        .circle_radius = 0.007f* nano_world_size.y,
        .height        = 0.1f*   nano_world_size.y,
        .width         = my_chain.chain_length,
    };

    // ==== STM ====
    stm = (Stm) 
    {
        .height        = nano_world_size.y * 0.20f,
        .tip_height    = -nano_world_size.y * 0.10f,
        .atom_width    = STM_atom_width,
        .color         = BLUE,
        .line_thickess = 0.01f* nano_world_size.x,
    };
    stm_set_rect(&stm);
    stm_init_state(&stm);

    
    // ==== domain_wall ====
    float spin_block_width    = inter_atomic * (STM_atom_width-1);
    domain_wall = (DomainWall)
    {
        .domain_width = spin_block_width,
        .N_walls      = N_mag_domains,
        .color        = LIGHTRED,
        .rec = (Rectangle)
        {
            .height = nano_world_size.y / 3.0f ,
            .width  = STM_atom_width * inter_atomic,
            .x      = spin_block_width + 0.5f*inter_atomic,
            .y      = -nano_world_size.y / (2 * 3.0f) 
        }
    };
#ifdef DEBUG
    printf("GAME_LOG: domain_wall: N_walls %zu\n", domain_wall.N_walls);
#endif 

    // ==== Text ====
    title_setup = text_create("SETUP SPINS: Write 42 using the STM tip.", 40, DARKGREEN);
    text_set_x_centered(&title_setup, GetScreenWidth()/2);
    text_set_y         (&title_setup, 10);
 
    title_start = text_create("GO! Protect the data against fluctuations", 40, RED);
    text_set_x_centered(&title_start, GetScreenWidth()/2);
    text_set_y         (&title_start, 10);

    temperatur_text = text_create("", 30, BLACK);
    text_set_y         (&temperatur_text, GetScreenHeight() * 0.1f);

    spin_text = text_create("", 20, BLACK);
    text_set_y         (&spin_text, GetScreenHeight() * 0.7f);

    binary_text = text_create("1", 40, RED);
    text_set_y         (&binary_text, GetScreenHeight() * 0.75f);

    decimal_text = text_create("", 40, RED);
    text_set_y         (&decimal_text, GetScreenHeight() * 0.9f);


    // ==== magnetic data ====
    my_data = magnetic_data_create(N_mag_domains, 42);


    // ==== game state ====
    game_play_mode = SETUP_SPINS;
    temperature    = 0.0f;
}
 

static void _game_play_update(void)
{
    if (game_play_mode == SETUP_SPINS && magnetic_data_is_target(&my_data))
    {
        temperature = 10.0f;
        game_play_mode = PROTECT_SPINS; 
    }
    else if (game_play_mode == PROTECT_SPINS && !magnetic_data_is_target(&my_data))
    {
        game_scene.state = LEAVING;
    }
    
    if (IsKeyPressed(KEY_LEFT))
    {
        stm_move_left(&stm);
    }
    if (stm.atom_current < my_chain.N-stm.atom_width && IsKeyPressed(KEY_RIGHT))
    {
        stm_move_right(&stm);
    }

    if(IsKeyPressed(KEY_TAB))
    {
        stm.orientation = !stm.orientation;
    }

    if(IsKeyPressed(KEY_SPACE))  
        stm_send_magnetic_pulse(&stm, &my_chain);

    if(IsKeyPressed(KEY_HOME)) 
        spin_chain_set_all(&my_chain, SPIN_DOWN);

    spin_chain_update_monte_carlo(&my_chain, temperature); 
}


static void _game_play_draw(void)
{
    BeginDrawing();
        ClearBackground(MY_BEIGE);

        switch (game_play_mode)
        {
        case SETUP_SPINS:
            text_draw(&title_setup);
            break;
        case PROTECT_SPINS:
            text_draw(&title_start);
            break;
        }
        
        text_write         (&temperatur_text, TextFormat("temperature= %.2f", temperature));
        text_set_x_centered(&temperatur_text, GetScreenWidth()/2);
        text_draw          (&temperatur_text);

        for (size_t n = 0; n < domain_wall.N_walls; n++)
        {
            float spin_block_start      = 2*n * inter_atomic*STM_atom_width;
            float spin_block_mid_point  = spin_block_start +  0.5* domain_wall.domain_width;
            float magnetization         = spin_chain_get_block_magnetizaiton(&my_chain, 2*n, STM_atom_width);
            Vector2 block_mid_point_vec = GetWorldToScreen2D((Vector2){spin_block_mid_point, 0}, camera);

            text_write         (&spin_text, TextFormat("%.2f", magnetization));
            text_set_x_centered(&spin_text, block_mid_point_vec.x);
            text_draw          (&spin_text);

            int binary_value  = (int)roundf((magnetization + 1.0f) * 0.5f);

            text_write         (&binary_text, TextFormat("%d", binary_value));
            text_set_x_centered(&binary_text, block_mid_point_vec.x);
            text_draw          (&binary_text);

            my_data.binaryBuffer[n] = binary_value;
        } 

        magnetic_data_set_decimal(&my_data);

        text_write         (&decimal_text, TextFormat("Base 10 value: %d", my_data.decimal_value));
        text_set_x_centered(&decimal_text, GetScreenWidth()/2.0f);
        text_draw          (&decimal_text);

        BeginMode2D(camera);

            domain_wall_draw(&domain_wall);

            stm_draw(&stm);

            graph_draw(&my_graph);

            // spin values:
            for (size_t i = 0; i < N_atoms; i++)
            {
                float y0     = 0;
                float x0     = i * inter_atomic;
                float S_y    = - my_graph.height * sin(my_chain.spins[i]);
                float yf     = y0 + S_y;
                DrawCircleV((Vector2){x0, yf}, my_graph.circle_radius, my_graph.circle_color);
            }
        EndMode2D();
    EndDrawing();       
}

static void game_play_run()
{
    _game_play_update();
    _game_play_draw();
}

static void game_play_clean()
{
    puts("GAME LOG: GAME: clean-up.");
    spin_chain_release(&my_chain);
    screen_level_reset(&game_scene);
    game_scene.next_screen = OUTRO;
}