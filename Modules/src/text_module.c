#include "text_module.h"


void text_set_size(text_t* text)
{
    text->textSize = MeasureText(text->buffer, text->fontSize);
}
 
text_t text_create(const char* title, int fontSize, Color color)
{
    text_t text =
    {
        .color    = color,
        .fontSize = fontSize,
        .x        = 0,
        .y        = 0,
    };
    strcpy(text.buffer, title);
    text_set_size(&text);
    return text; 
}

void text_set_x_centered(text_t* text, float x)
{
    text->x = x - 0.5*text->textSize;
}
void text_set_x(text_t* text, float x)
{
    text->x = x;
}
void text_set_y(text_t* text, float y)
{
    text->y = y;
}
void text_write(text_t* text, const char* input)
{
    strcpy(text->buffer, input);
    text_set_size(text);
}
void text_draw(const text_t* text)
{
    DrawText(text->buffer, 
            text->x, 
            text->y, 
            text->fontSize,
            text->color);
}