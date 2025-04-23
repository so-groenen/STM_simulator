#include "Scenes/intro.h"
 



// ======== Static functions / vars ========= 
static void intro_init(void);
static void intro_run(void);
static void intro_clean(void);
static void intro_update(void);
static void intro_draw(void);
static text_t greeting;


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
    puts("GAME LOG: INTRO: init.");
    greeting = text_create("PRESS [SPACE] TO CONTINUE", 40, WHITE);
    text_set_x_centered(&greeting, GetScreenWidth()/2);
    text_set_y         (&greeting, GetScreenHeight()/2);
}
static void intro_update(void)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        intro_scene.state = LEAVING;
    }    
}
static void intro_draw(void)
{
    BeginDrawing();
        ClearBackground(RED);
        text_draw(&greeting);
    EndDrawing();       
}

static void intro_clean()
{
    puts("GAME LOG: INTRO: clean-up.");
    screen_level_reset(&intro_scene);
    intro_scene.next_screen = GAME;
}
static void intro_run()
{
    intro_update();
    intro_draw();
}

