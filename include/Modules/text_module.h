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
} Text;

Text text_create(const char* title, int fontSize, Color color);
int text_get_x_center(const Text* text);
void text_set_size(Text* text);
void text_set_x_centered(Text* text, float x);
void text_set_x(Text* text, float x);
void text_set_y(Text* text, float y);
void text_write(Text* text, const char* input);
void text_draw(const Text* text);

#endif
