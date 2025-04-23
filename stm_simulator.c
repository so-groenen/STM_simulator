#include "Scenes/game.h"
#include "Scenes/outro.h"
#include "Scenes/intro.h"
#include "game_states.h"

 
size_t N_atoms;
size_t STM_atom_width;

static Scene_t* current_scene = &intro_scene;
static void update_draw_frame();


int main(int argc, char** argv)
{
    srand(time(NULL));
    InitWindow(1024, 548, "STM simulator");
    SetTargetFPS(60);
    puts("===============");
    puts("GAME LOG: START");

    N_atoms        = 640;
    STM_atom_width = 40;

    while (!WindowShouldClose())
    {
        update_draw_frame();
    }


    if (WindowShouldClose() && current_scene->state == RUNNING)
    {
        printf("GAME LOG: [window closing], calling current_scene clean-up:\n");
        current_scene->clean();
    }
    CloseWindow();
    return EXIT_SUCCESS;
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

// void screen_selector()
// {
//     switch (my_screen)
//     {
//     case INTRO:
//         current_scene = &intro_screen;
//         break;
//     case GAME:
//         current_scene = &game_screen;
//         break;
//     case OUTRO:
//         current_scene = &outro_screen;
//         break;
//     }
// }
// void screen_manager()
// {
//     switch (state)
//     {
//     case ENTERING:
//         current_scene->init();
//         state = RUNNING;
//         break;
//     case RUNNING:
//         current_scene->run();
//         break;
//     case LEAVING:
//         current_scene->clean();
//         break;
//     }
// }
/////////////////////
// void UpdateDrawFrame()
// {
//     screen_selector();
//     screen_manager();
// }


// void UpdateDrawFrame_ORIGINAL()
// {
//     switch (state)
//     {
//     case ENTERING:
//         screen_init();
//         break;
//     case RUNNING:
//         screen_run();
//         break;
//     case LEAVING:
//         screen_clean();
//         break;
//     }
// }

// static void screen_init()
// {
//     switch (my_screen)
//     {
//     case INTRO:
//         intro_init();
//         break;
//     case GAME:
//         game_init();
//         break;
//     case OUTRO:
//         outro_init();
//         break;
//     default:
//         break;
//     }
//     state = RUNNING;
// }
// static void screen_run()
// {
//     switch (my_screen)
//     {
//     case INTRO:
//         intro_run();
//         break;
//     case GAME:
//         game_run();
//         break;
//     case OUTRO:
//         outro_run();
//         break;
//     default:
//         break;
//     }
// }
// static void screen_clean()
// {
//     switch (my_screen)
//     {
//     case INTRO:
//         intro_clean();
//         break;
//     case GAME:
//         game_clean();
//         break;
//     default:
//         break;
//     }
// }
