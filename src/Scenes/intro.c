#include "Scenes/intro.h"
#define public volatile

// ======== Static functions / vars ========= 
static void intro_init(void);
static void intro_run(void);
static void intro_clean(void);
static void intro_update(void);
static void intro_draw(void);
static void intro_reset_ui_on_resize(void);
static float _spin_to_rotation(float spin);

// ======== Static vars ========= 
static Texture2D spin_arrow;
static Text greeting;
static Text title;
static Text sub_title;
static Text easy;
static Text medium;
static Text hard;
static Text press_to_continue;
static Rectangle sourceRec;
static Rectangle destRec;
static Rectangle selector;
static Vector2 origin;
static Camera2D camera;
static SpinChain intro_chain;
static uint32_t counter;
// static float start_zoom_ratio;
static int32_t blink_delta           = -4;
static const float intro_temperature = 0.01; //in Kelvin
static const uint32_t start_time     = 1; //sec
static const uint32_t run_time       = 5; //sec
static const uint32_t fps_time       = 60;
static const float start_zoom_ratio  = 5000.0f;
static const float zoom_multiplier   = 0.955f; //0.95  
static const float alpha_multiplier  = 0.98f;  
static const char* spin_arrow_path   =  "resources/spin_arrow.png";

// global non static:
bool first_time     = true;
float final_zoom    = 0.f;  // to be calculated during the first run!

// ======== global intro screen =======
Scene intro_scene =
{
    .next_screen    = GAME,
    .state          = ENTERING,
    .init           = intro_init,
    .run            = intro_run,
    .clean          = intro_clean,
};

typedef enum
{
    ANIMATION,
    MENU,
} OpeningScene;

typedef enum 
{
    START,
    RUN,
    STOP,
} OpeningSceneState;

static OpeningScene opening_scene;
static OpeningSceneState opening_scene_state;


static void intro_init()  
{
    counter             = 0;
    blink_delta         = -4;
    difficulty          = EASY;
    opening_scene_state = START;

    opening_scene       = first_time? ANIMATION : MENU;

    
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

    easy = text_create("easy", 30, BLACK);
    text_set_x_centered(&easy, GetScreenWidth()/6.0f);
    text_set_y         (&easy, GetScreenHeight()*0.7f);

    medium = text_create("medium", 30, BLACK);
    text_set_x_centered(&medium, 3*GetScreenWidth()/6.0f);
    text_set_y         (&medium, GetScreenHeight()*0.7f);

    hard = text_create("hard", 30, BLACK);
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

    // start_zoom_ratio = 5000.0f;
    spin_arrow = LoadTexture(spin_arrow_path);
    sourceRec  = (Rectangle){0.0f, 0.0f, (float)spin_arrow.width, (float)spin_arrow.height};
    spin_chain_set_camera(&camera, &intro_chain, start_zoom_ratio);
 
    

    float png_to_nanoworld   = 0.5*inter_atomic / (float)spin_arrow.width;
    
    destRec    = (Rectangle){intro_chain.chain_length/2.0f, 
                             0.25*inter_atomic, 
                             png_to_nanoworld * spin_arrow.width,
                             png_to_nanoworld * spin_arrow.height};
    origin     = (Vector2){0.51f*destRec.width, 0.638f*destRec.height};    // GetScreenToWorld2D((Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f}, camera);
}

static void update_animation(void)
{
    switch (opening_scene_state)
    {
        case START:
            if (counter%(start_time*fps_time) == 0 || IsKeyPressed(KEY_SPACE))
            {
                opening_scene_state = RUN;
                counter             = 0;
            }        
            break;
        case RUN:
            if (counter%2 == 0)
            {
                camera.zoom      *= zoom_multiplier;
                greeting.color.a *= alpha_multiplier;
            }
            if (counter%(run_time*fps_time) == 0)
            {
                opening_scene_state = STOP;
            }
            if (IsKeyPressed(KEY_SPACE))
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
                opening_scene_state = STOP;
            }
            break;
        case STOP:
            counter             = 0;
            final_zoom          = camera.zoom;
            opening_scene       = MENU;
            opening_scene_state = START;
            break;
        default:
            break;
    }
}

static void update_menu(void)
{
    if (opening_scene_state == START)
    {
        greeting.color.a     = 0;
        camera.zoom          = final_zoom;
        opening_scene_state  = RUN;
    }

    press_to_continue.color.a += blink_delta;
    if (press_to_continue.color.a <= abs(blink_delta) && blink_delta < 0)
    {
        blink_delta *= -1;
    }
    if (press_to_continue.color.a >= (UINT8_MAX-abs(blink_delta))  && blink_delta > 0)
    {
        blink_delta *= -1;
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        intro_scene.state = LEAVING; // => we have selected a sttate
    }
    else if (IsKeyPressed(KEY_RIGHT) && difficulty < HARD)
    {
        int delta_x = text_get_x_center(&medium) - text_get_x_center(&easy);
        difficulty++;
        selector.x += delta_x; 
    }
    else if (IsKeyPressed(KEY_LEFT) && difficulty > EASY)
    {
        int delta_x = text_get_x_center(&medium) - text_get_x_center(&easy);
        difficulty--;
        selector.x -= delta_x;  
    }
}

static void intro_update(void)
{
    counter++;
    switch (opening_scene)
    {
    case ANIMATION:
        update_animation();
        break;
    case MENU:
        update_menu();
        break;
    default:
        break;
    } 
    spin_chain_update_monte_carlo(&intro_chain, intro_temperature);
}
static void intro_draw(void)
{
    BeginDrawing();
        if (opening_scene == ANIMATION && opening_scene_state == START)
        {
            ClearBackground(BLACK);
        }
        else 
        {
            ClearBackground(MY_BEIGE);
        }
        
        BeginMode2D(camera);
            for (size_t i = 0; i < intro_chain.N; i++)
            {
                destRec.x = i*inter_atomic;
                DrawTexturePro(spin_arrow, sourceRec, destRec, origin, _spin_to_rotation(intro_chain.spins[i]), WHITE);
            }
        EndMode2D();
        
            text_draw(&greeting);
            if (opening_scene == MENU)
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
 