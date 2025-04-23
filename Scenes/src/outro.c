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
// ================================


// ========== Global var ========== //
Scene_t outro_scene =
{
    .next_screen    = OUTRO,
    .state          = ENTERING,
    .init           = outro_init,
    .run            = outro_run,
    .clean          = outro_clean,
};
// ================================




void outro_init()
{
    goodbye = text_create("PRESS [ESC] TO CONTINUE", 40, WHITE);
    text_set_x_centered(&goodbye, GetScreenWidth()/2);
    text_set_y         (&goodbye, GetScreenHeight()/2);
    printf("GAME LOG: outro init\n");
}

static void outro_draw(void)
{
    BeginDrawing();
        ClearBackground(BLUE);
        text_draw(&goodbye);
    EndDrawing();       
}
static void outro_update(void)
{
    // if (IsKeyPressed(KEY_SPACE))
    // {
    //     state = LEAVING;
    // }
}


void outro_run()
{
    outro_update();
    outro_draw();
}

void outro_clean()
{
    printf("GAME LOG: outro clean up\n");
    screen_level_reset(&outro_scene);
}