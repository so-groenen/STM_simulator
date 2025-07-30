#ifndef THE_GAME_H
#define THE_GAME_H
#include "Scenes/intro.h"
#include "Scenes/game_play.h"
#include "Scenes/outro.h"
#include "Scenes/scene_states.h"

extern Difficulty difficulty; 
void game_cleanup(); 
void game_update_draw_frame();
#endif