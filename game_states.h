#ifndef GAME_STATES_H
#define GAME_STATES_H

typedef enum Scene_name // 
{
    INTRO,
    GAME,
    OUTRO,
} Scene_name;

typedef enum Game_state
{
    ENTERING,
    RUNNING,
    LEAVING,
} Game_state;

typedef struct Scene
{
    Scene_name next_screen;
    Game_state state;
    void (*init)(void);
    void (*run)(void);
    void (*clean)(void);
} Scene_t;

void screen_level_reset(Scene_t* level);


#endif