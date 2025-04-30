#ifndef TEXT_MODULE_H
#define TEXT_MODULE_H
#include "raylib.h"
#include <string.h>
#define MAX_TEXT_BUFFER 64

typedef struct
{
    char buffer[MAX_TEXT_BUFFER];
    Color color;
    int fontSize;
    int textSize;
    float x;
    float y;
} text_t;

text_t text_create(const char* title, int fontSize, Color color);
int text_get_x_center(const text_t* text);
void text_set_size(text_t* text);
void text_set_x_centered(text_t* text, float x);
void text_set_x(text_t* text, float x);
void text_set_y(text_t* text, float y);
void text_write(text_t* text, const char* input);
void text_draw(const text_t* text);

#endif
