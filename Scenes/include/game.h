#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "..\..\spin_chain.h"
#include "..\..\stm.h"
#include "..\..\text_module.h"
#include "..\..\magnetic_data.h"
#include "..\..\game_module.h"
#include "..\..\game_states.h"

#define LIGHTRED (Color){255, 100, 177, 200}        
extern size_t N_atoms;
extern size_t STM_atom_width; 


// void game_init(void);
// void game_run(void);
// void game_clean(void);

extern Scene_t game_scene;
//  =
// {
//     .NAME  = GAME,
//     .init  = game_init,
//     .run   = game_run,
//     .clean = game_clean,
// };
  
#endif