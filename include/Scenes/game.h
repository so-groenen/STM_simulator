#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "Modules/spin_chain.h"
#include "Modules/stm.h"
#include "Modules/text_module.h"
#include "Modules/magnetic_data.h"
#include "Modules/game_modules.h"

#include "Scenes/scene_states.h"

#define LIGHTRED (Color){255, 100, 177, 200}        

extern Scene_t game_scene;
extern Difficulty difficulty;

#endif