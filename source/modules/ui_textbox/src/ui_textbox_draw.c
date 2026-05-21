/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Reusable textbox draw helpers
*/

#include "ui_textbox.h"

static void set_vertex(sfVertex *vertex, const sfVector2f *position,
    const sfColor *color)
{
    vertex->position = *position;
    vertex->color = *color;
    vertex->texCoords = (sfVector2f){0.0f, 0.0f};
}

static void draw_rect(sfRenderWindow *window, const sfFloatRect *rect,
    const sfColor *color)
{
    sfVertex quad[4];

    set_vertex(&quad[0], &(sfVector2f){rect->left, rect->top}, color);
    set_vertex(&quad[1], &(sfVector2f){rect->left + rect->width, rect->top},
        color);
    set_vertex(&quad[2],
        &(sfVector2f){rect->left + rect->width, rect->top + rect->height},
        color);
    set_vertex(&quad[3], &(sfVector2f){rect->left, rect->top + rect->height},
        color);
    sfRenderWindow_drawPrimitives(window, quad, 4, sfQuads, NULL);
}

static void draw_border(sfRenderWindow *window, const sfFloatRect *rect,
    const sfColor *color)
{
    sfVertex line[5];

    set_vertex(&line[0], &(sfVector2f){rect->left, rect->top}, color);
    set_vertex(&line[1], &(sfVector2f){rect->left + rect->width, rect->top},
        color);
    set_vertex(&line[2],
        &(sfVector2f){rect->left + rect->width, rect->top + rect->height},
        color);
    set_vertex(&line[3], &(sfVector2f){rect->left, rect->top + rect->height},
        color);
    set_vertex(&line[4], &(sfVector2f){rect->left, rect->top}, color);
    sfRenderWindow_drawPrimitives(window, line, 5, sfLineStrip, NULL);
}

void ui_textbox_draw(sfRenderWindow *window,
    sfText *text,
    const ui_textbox_t *textbox,
    const ui_textbox_style_t *style)
{
    sfColor fill;

    if (window == NULL || text == NULL || textbox == NULL || style == NULL)
        return;
    fill = textbox->active == sfTrue ? style->fill_active : style->fill_idle;
    draw_rect(window, &textbox->bounds, &fill);
    draw_border(window, &textbox->bounds, &style->border);
    sfText_setCharacterSize(text, style->text_size);
    sfText_setFillColor(text, style->text);
    sfText_setString(text, textbox->buffer != NULL ? textbox->buffer : "");
    sfText_setPosition(text,
        (sfVector2f){textbox->bounds.left + 8.0f, textbox->bounds.top + 4.0f});
    sfRenderWindow_drawText(window, text, NULL);
}
