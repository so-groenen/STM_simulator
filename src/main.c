#include "the_game.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


int main(void)
{
    srand(time(NULL));
    // SetWindowState(FLAG_WINDOW_RESIZABLE); // TO DO: make it resizable & make UI elements react on resize

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

