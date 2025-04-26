#include "Scenes/outro.h"

//         Static functions
// ================================
static void outro_init(void);
static void outro_run(void);
static void outro_clean(void);
static void outro_draw(void);
static void outro_update(void);
// =======+== Static vars =========
static text_t goodbye;
static text_t start_again;

// ================================


// ========== Global var ========== //
Scene_t outro_scene =
{
    .next_screen    = INTRO,
    .state          = ENTERING,
    .init           = outro_init,
    .run            = outro_run,
    .clean          = outro_clean,
};
// ================================




void outro_init()
{
    puts("GAME LOG: OUTRO: init.");
    goodbye = text_create("GAME OVER!", 40, WHITE);
    text_set_x_centered(&goodbye, 0.5f*GetScreenWidth());
    text_set_y         (&goodbye, 0.4f*GetScreenHeight());

    start_again = text_create("Press [SPACE] to start again, [ESC] to stop.", 20, WHITE);
    
    text_set_x_centered(&start_again, 0.5f*GetScreenWidth());
    text_set_y         (&start_again, 0.8f*GetScreenHeight());
}

static void outro_draw(void)
{
    BeginDrawing();
        ClearBackground(BLUE);
        text_draw(&goodbye);
        text_draw(&start_again);
    EndDrawing();       
}
static void outro_update(void)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        outro_scene.state = LEAVING;
    }
}


void outro_run()
{
    outro_update();
    outro_draw();
}

void outro_clean()
{
    puts("GAME LOG: OUTRO: clean-up.");
    screen_level_reset(&outro_scene);
}