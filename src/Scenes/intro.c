#include "Scenes/intro.h"
#define public volatile

// ======== Static functions / vars ========= 
static void intro_init(void);
static void intro_run(void);
static void intro_clean(void);
static void intro_update(void);
static void intro_draw(void);
static float _spin_to_rotation(float spin);

// ======== Static vars ========= 
static Texture2D spin_arrow;
static text_t greeting;
static text_t title;
static text_t sub_title;
static text_t easy;
static text_t medium;
static text_t hard;
static text_t press_to_continue;
static Rectangle sourceRec;
static Rectangle destRec;
static Rectangle selector;
static Vector2 origin;
static Camera2D camera;
static spin_chain_t intro_chain;
static uint32_t counter;
static float zoom_ratio;
static int32_t blink_delta    = -4;

static const float intro_temperature = 0.01;
static const uint32_t start_time     = 1;
static const uint32_t run_time       = 5;
static const uint32_t fps_time       = 60;
static const float zoom_multiplier   = 0.95;  
static const float alpha_multiplier  = 0.98;  
static const char* spin_arrow_path   =  "resources/spin_arrow.png";
// ======== global intro screen =======
Scene_t intro_scene =
{
    .next_screen    = GAME,
    .state          = ENTERING,
    .init           = intro_init,
    .run            = intro_run,
    .clean          = intro_clean,
};

typedef enum 
{
    START,
    RUN,
    STOP,
} truc;


static truc timer;


static void intro_init()
{
    counter      = 0;
    blink_delta  = -4;
    timer        = START;
    difficulty   = EASY;

    puts("GAME LOG: INTRO: init:");

    greeting = text_create("RAYLIB PRESENTS...", 50, YELLOW);
    text_set_x_centered(&greeting, GetScreenWidth()/2);
    text_set_y         (&greeting, GetScreenHeight()/3.0f);

    title         = text_create("STM SIMULATOR '85", 60, RED);    
    text_set_x_centered(&title, GetScreenWidth()/2);
    text_set_y         (&title,    0.2f*GetScreenHeight());

    sub_title         = text_create("[powered by Markov-chain Monte-Carlo]", 20, ORANGE);    
    text_set_x_centered(&sub_title, GetScreenWidth()/2);
    text_set_y         (&sub_title, 0.3f*GetScreenHeight());

    press_to_continue = text_create("PRESS [ENTER] TO CONTINUE", 20, BLACK);
    text_set_x_centered(&press_to_continue, GetScreenWidth()/2);
    text_set_y         (&press_to_continue, GetScreenHeight()*0.8f);
    press_to_continue.color.a = 254;

    easy = text_create("easy", 20, BLACK);
    text_set_x_centered(&easy, GetScreenWidth()/6.0f);
    text_set_y         (&easy, GetScreenHeight()*0.7f);

    medium = text_create("medium", 20, BLACK);
    text_set_x_centered(&medium, 3*GetScreenWidth()/6.0f);
    text_set_y         (&medium, GetScreenHeight()*0.7f);

    hard = text_create("hard", 20, BLACK);
    text_set_x_centered(&hard, 5*GetScreenWidth()/6.0f);
    text_set_y         (&hard, GetScreenHeight()*0.7f);
 
    float width  = GetScreenWidth()/6.0f;
    float height = GetScreenHeight()*0.11f;
    selector = (Rectangle){text_get_x_center(&easy) - width/2.0f,
                           easy.y - 0.35f*height,
                           width,
                           height};

    size_t N_of_spins = 101;
    spin_chain_set_periodic_boundary(true);
    spin_chain_set_interaction(1.0f);
    spin_chain_set_inter_atomic(100.0f);
    spin_chain_init            (&intro_chain, N_of_spins);
    spin_chain_set_all         (&intro_chain, SPIN_UP);
    spin_chain_set_energy      (&intro_chain);

    zoom_ratio      = 5000.0f;
    spin_arrow = LoadTexture(spin_arrow_path);
    sourceRec  = (Rectangle){0.0f, 0.0f, (float)spin_arrow.width, (float)spin_arrow.height};
    spin_chain_set_camera(&camera, &intro_chain, zoom_ratio);

    float png_to_nanoworld   = 0.5*inter_atomic / (float)spin_arrow.width;
    
    destRec    = (Rectangle){intro_chain.chain_length/2.0f, 
                             0.25*inter_atomic, 
                             png_to_nanoworld * spin_arrow.width,
                             png_to_nanoworld * spin_arrow.height};
    origin     = (Vector2){0.51f*destRec.width, 0.638f*destRec.height};    // GetScreenToWorld2D((Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f}, camera);

}
static void intro_update(void)
{
    counter++;
    switch (timer)
    {
        case START:
        {
            if (counter%( start_time * fps_time) == 0)
            {
                timer   = RUN;
                counter = 0;
                // puts("START");
            }
        }
        break;
        case RUN:
        {
            if (counter%(2) == 0)
            {
                camera.zoom      *= zoom_multiplier;
                greeting.color.a *= alpha_multiplier;
            }
            if (counter%(run_time*fps_time) == 0)
            {
                timer = STOP;
                counter = 0;
                // puts("STOP");
                // printf("ZOOM = %f\n", camera.zoom);
            }
        }
        break;
        case STOP:
        {
            press_to_continue.color.a += blink_delta;
            if (press_to_continue.color.a <= abs(blink_delta) && blink_delta < 0)
            {
                blink_delta *= -1;
            }
            if (press_to_continue.color.a >= (UINT8_MAX-abs(blink_delta))  && blink_delta > 0)
            {
                blink_delta *= -1;
            }
        }
        break;
    } 


    if (IsKeyPressed(KEY_RIGHT) && difficulty < HARD)
    {
        difficulty++;
        selector.x += (GetScreenWidth()/3.0f); // replace by get_x_center(medium) - get_...(easy):
    }
    else if (IsKeyPressed(KEY_LEFT) && difficulty > EASY)
    {
        difficulty--;
        selector.x -= GetScreenWidth()/3.0f; // ditto
    }

    if (IsKeyPressed(KEY_SPACE) && timer == START)
    {
        counter = 0;
        timer   = RUN;
    }
    else if (IsKeyPressed(KEY_SPACE) && timer == RUN)
    {
        while (counter%(run_time*fps_time) != 0)
        {
            if (counter%2 == 0)
            {
                camera.zoom      *= zoom_multiplier;
                greeting.color.a *= alpha_multiplier;
            }
            counter++;
        }
        timer   = STOP;
        counter = 0;
    }
    else if (IsKeyPressed(KEY_SPACE) && timer == STOP)
    {
        intro_scene.state = LEAVING;
    }

    spin_chain_update_monte_carlo(&intro_chain, intro_temperature);
}
static void intro_draw(void)
{
    BeginDrawing();
        if (timer == START)
        {
            ClearBackground(BLACK);
        }
        else
        {
            ClearBackground(WHITE);
        }
        
        
        BeginMode2D(camera);

            // DrawLine((int)camera.target.x, -GetScreenHeight()*10, (int)camera.target.x, GetScreenHeight()*10, GREEN);
            // DrawLine(-GetScreenWidth()*10, (int)camera.target.y, GetScreenWidth()*10, (int)camera.target.y, GREEN);

            for (size_t i = 0; i < intro_chain.N; i++)
            {
                destRec.x = i*inter_atomic;
                DrawTexturePro(spin_arrow, sourceRec, destRec, origin, _spin_to_rotation(intro_chain.spins[i]), WHITE);
            }
        EndMode2D();
        
            text_draw(&greeting);
            if (timer == STOP)
            {
                text_draw(&title);
                text_draw(&sub_title);
                text_draw(&press_to_continue);
                text_draw(&easy);
                text_draw(&medium);
                text_draw(&hard);
                DrawRectangleLinesEx(selector, 2, BLACK);
            }
 
        
        

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