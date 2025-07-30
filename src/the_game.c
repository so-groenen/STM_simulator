#include "the_game.h"

static Scene* current_scene = &intro_scene;
static void  screen_switch();
Difficulty difficulty;
 
void game_cleanup()
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
        current_scene = &game_scene; //gameplay scene
        break;
    case OUTRO:
        current_scene = &outro_scene;
        break;
    default:
        break;
    }
}
void game_update_draw_frame()
{
    switch (current_scene->state)
    {
    case LEAVING:
        current_scene->clean();
        screen_switch();
        /* Fall through! */
    case ENTERING:
        current_scene->init();
        current_scene->state = RUNNING;
        /* Fall through! */
    case RUNNING:
        current_scene->run();
        break;
    default:
        break;
    }
}
