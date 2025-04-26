#ifndef TEXT_MODULE_H
#define TEXT_MODULE_H
#include "raylib.h"
#include <string.h>

typedef struct
{
    char buffer[64];
    Color color;
    int fontSize;
    int textSize;
    float x;
    float y;
} text_t;

text_t text_create(const char* title, int fontSize, Color color);
void text_set_size(text_t* text);
void text_set_x_centered(text_t* text, float x);
void text_set_x(text_t* text, float x);
void text_set_y(text_t* text, float y);
void text_write(text_t* text, const char* input);
void text_draw(const text_t* text);

#endif
