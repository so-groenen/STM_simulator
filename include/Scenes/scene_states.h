#ifndef SCENE_STATES_H
#define SCENE_STATES_H

typedef enum 
{
    INTRO,
    GAME,
    OUTRO,
} SceneName;

typedef enum
{
    EASY,
    MEDIUM,
    HARD,
} Difficulty;

typedef enum
{
    ENTERING,
    RUNNING,
    LEAVING,
} GameState;

typedef struct
{
    SceneName next_screen;
    GameState state;
    void (*init)(void);
    void (*run)(void);
    void (*clean)(void);
} Scene;

void screen_level_reset(Scene* level);

#endif