#include "Scenes/intro.h"
#define public volatile

// ======== Static functions / vars ========= 
static void intro_init(void);
static void intro_run(void);
static void intro_clean(void);
static void intro_update(void);
static void intro_draw(void);
static float _spin_to_rotation(float spin);
static Texture2D spin_arrow;
static text_t greeting;
static Rectangle sourceRec;
static Rectangle destRec;
static Vector2 origin;
static Camera2D camera;
static spin_chain_t intro_chain;

static float ratio;
static int rotation;
static float intro_temperature;
// ======== global intro screen =======
Scene_t intro_scene =
{
    .next_screen    = GAME,
    .state          = ENTERING,
    .init           = intro_init,
    .run            = intro_run,
    .clean          = intro_clean,
};




static void intro_init()
{
    puts("GAME LOG: INTRO: init:");
    greeting = text_create("PRESS [SPACE] TO CONTINUE", 40, WHITE);
    const char* spin_arrow_path =  "resources/spin_arrow.png";

    text_set_x_centered(&greeting, GetScreenWidth()/2);
    text_set_y         (&greeting, GetScreenHeight()/4);

    size_t N_of_spins = 41;
    intro_temperature = 0.01;

    spin_chain_set_periodic_boundary(true);
    spin_chain_set_interaction(1.0f);
    spin_chain_set_inter_atomic(100.0f);
    spin_chain_init           (&intro_chain, N_of_spins);
    spin_chain_set_all        (&intro_chain, SPIN_UP);
    spin_chain_set_energy     (&intro_chain);

    ratio      = 0.6f;
    spin_arrow = LoadTexture(spin_arrow_path);
    sourceRec  = (Rectangle){0.0f, 0.0f, (float)spin_arrow.width, (float)spin_arrow.height};
    spin_chain_set_camera(&camera, &intro_chain, ratio);

    float png_to_nanoworld   = 0.5*inter_atomic / (float)spin_arrow.width;
    
    destRec    = (Rectangle){intro_chain.chain_length/2.0f, 
                             0.25*inter_atomic, 
                             png_to_nanoworld * spin_arrow.width,
                             png_to_nanoworld * spin_arrow.height};
    origin     = (Vector2){0.51f*destRec.width, 0.638f*destRec.height};    // GetScreenToWorld2D((Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f}, camera);

}
static void intro_update(void)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        intro_scene.state = LEAVING;
    }    
    else if (IsKeyDown(KEY_DOWN))
    {
        // ratio -= 0.05f;
        camera.zoom *= 0.95;
    }
    else if (IsKeyDown(KEY_UP))
    {
        camera.zoom *= 1.05;
    }
 


    spin_chain_update_monte_carlo(&intro_chain, intro_temperature);
}
static void intro_draw(void)
{
    BeginDrawing();
        ClearBackground(WHITE);
        text_draw(&greeting);
        
        BeginMode2D(camera);

            DrawLine((int)camera.target.x, -GetScreenHeight()*10, (int)camera.target.x, GetScreenHeight()*10, GREEN);
            DrawLine(-GetScreenWidth()*10, (int)camera.target.y, GetScreenWidth()*10, (int)camera.target.y, GREEN);

            
            // DrawLine((int)origin.x, -GetScreenHeight()*10, (int)origin.x, GetScreenHeight()*10, BLUE);
            // DrawLine(-GetScreenWidth()*10, (int)origin.y, GetScreenWidth()*10, (int)origin.y, BLUE);
            for (size_t i = 0; i < intro_chain.N; i++)
            {
                destRec.x = i*inter_atomic;
                DrawTexturePro(spin_arrow, sourceRec, destRec, origin, _spin_to_rotation(intro_chain.spins[i]), WHITE);
            }
            

        EndMode2D();

    EndDrawing();       
}

static void intro_clean()
{
    puts("GAME LOG: INTRO: clean-up:");
    UnloadTexture(spin_arrow);
    spin_chain_release(&intro_chain);

    screen_level_reset(&intro_scene);
    intro_scene.next_screen = GAME;
}
static void intro_run()
{
    intro_update();
    intro_draw();
}

static float _spin_to_rotation(float spin)
{
    return spin*RAD2DEG - 90.0f;
}