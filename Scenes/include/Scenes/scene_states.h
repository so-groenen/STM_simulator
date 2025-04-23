#ifndef SCENE_STATES_H
#define SCENE_STATES_H

typedef enum 
{
    INTRO,
    GAME,
    OUTRO,
} Scene_name;

typedef enum
{
    ENTERING,
    RUNNING,
    LEAVING,
} Game_state;

typedef struct
{
    Scene_name next_screen;
    Game_state state;
    void (*init)(void);
    void (*run)(void);
    void (*clean)(void);
} Scene_t;



void screen_level_reset(Scene_t* level);


#endif