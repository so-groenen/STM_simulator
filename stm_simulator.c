#include "Scenes/game.h"
#include "Scenes/outro.h"
#include "Scenes/intro.h"
#include "Scenes/scene_states.h"

 

static Scene_t* current_scene = &intro_scene;
static void update_draw_frame();
static void cleanup();
Difficulty difficulty;

int main(void)
{
    srand(time(NULL));
    InitWindow(1024, 548, "STM simulator");
    SetTargetFPS(60);

    puts("===============");
    puts("GAME LOG: START");

    while (!WindowShouldClose())
    {
        update_draw_frame();
    }
    cleanup();

    puts("GAME LOG: STOP");
    puts("===============");
    CloseWindow();
    return EXIT_SUCCESS;
}


static void cleanup()
{
    if (WindowShouldClose() && current_scene->state == RUNNING)
    {
        puts("GAME LOG: [window closing], calling current_scene clean-up:");
        current_scene->clean();
    }
}
static void screen_switch()
{
    switch (current_scene->next_screen)
    {
    case INTRO:
        current_scene = &intro_scene;
        break;
    case GAME:
        current_scene = &game_scene;
        break;
    case OUTRO:
        current_scene = &outro_scene;
        break;
    default:
        break;
    }
}



static void update_draw_frame()
{
    switch (current_scene->state)
    {
    case LEAVING:
        current_scene->clean();
        screen_switch();
        // break;
    case ENTERING:
        current_scene->init();
        current_scene->state = RUNNING;
        // break;
    case RUNNING:
        current_scene->run();
        break;
    default:
        break;
    }
}
