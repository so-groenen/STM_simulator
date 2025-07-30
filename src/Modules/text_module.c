#include "Modules/text_module.h"


void text_set_size(Text* text)
{
    text->textSize = MeasureText(text->buffer, text->fontSize);
}
int text_get_x_center(const Text* text)
{
    return text->x + 0.5f*text->textSize; 
}
Text text_create(const char* title, int fontSize, Color color)
{
    Text text =
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

void text_set_x_centered(Text* text, float x)
{
    text->x = x - 0.5*text->textSize;
}
void text_set_x(Text* text, float x)
{
    text->x = x;
}
void text_set_y(Text* text, float y)
{
    text->y = y;
}
void text_write(Text* text, const char* input)
{
    strcpy(text->buffer, input);
    text_set_size(text);
}
void text_draw(const Text* text)
{
    DrawText(text->buffer, 
            text->x, 
            text->y, 
            text->fontSize,
            text->color);
}