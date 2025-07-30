// #include "Scenes/game.h"
// #include "Scenes/outro.h"
// #include "Scenes/intro.h"
// #include "Scenes/scene_states.h"

#include "the_game.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// static Scene* current_scene = &intro_scene;
// static void update_draw_frame();
// static void cleanup();
// Difficulty difficulty;

int main(void)
{
    srand(time(NULL));
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    // InitWindow(1024, 548, "STM simulator");
    InitWindow(1536, 864, "STM simulator");
    puts("GAME LOG: START");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(game_update_draw_frame, 60, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        game_update_draw_frame();
    }
#endif

    game_cleanup();    
    puts("GAME LOG: STOP");
    CloseWindow();
    return EXIT_SUCCESS;
}


// static void cleanup()
// {
//     if (WindowShouldClose() && current_scene->state == RUNNING)
//     {
//         puts("GAME LOG: [window closing], calling current_scene clean-up:");
//         current_scene->clean();
//     }
// }
// static void screen_switch()
// {
//     switch (current_scene->next_screen)
//     {
//     case INTRO:
//         current_scene = &intro_scene;
//         break;
//     case GAME:
//         current_scene = &game_scene;
//         break;
//     case OUTRO:
//         current_scene = &outro_scene;
//         break;
//     default:
//         break;
//     }
// }



// static void update_draw_frame()
// {
//     switch (current_scene->state)
//     {
//     case LEAVING:
//         current_scene->clean();
//         screen_switch();
//         /* Fall through! */
//     case ENTERING:
//         current_scene->init();
//         current_scene->state = RUNNING;
//         /* Fall through! */
//     case RUNNING:
//         current_scene->run();
//         break;
//     default:
//         break;
//     }
// }
