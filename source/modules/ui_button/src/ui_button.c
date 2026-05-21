/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Reusable rectangle button helpers implementation
*/

#include "ui_button.h"

static sfBool point_in_rect(int x, int y, const sfFloatRect *rect)
{
    if ((float)x < rect->left || (float)y < rect->top)
        return sfFalse;
    if ((float)x > rect->left + rect->width)
        return sfFalse;
    if ((float)y > rect->top + rect->height)
        return sfFalse;
    return sfTrue;
}

sfBool ui_button_contains(const ui_button_t *button, int x, int y)
{
    if (button == NULL)
        return sfFalse;
    return point_in_rect(x, y, &button->bounds);
}

ui_button_state_t ui_button_get_state(const ui_button_t *button,
    int x,
    int y,
    sfBool active)
{
    if (active == sfTrue)
        return UI_BUTTON_ACTIVE;
    if (ui_button_contains(button, x, y) == sfTrue)
        return UI_BUTTON_HOVER;
    return UI_BUTTON_IDLE;
}

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

static sfColor get_fill(const ui_button_style_t *style, ui_button_state_t state)
{
    if (state == UI_BUTTON_ACTIVE)
        return style->fill_active;
    if (state == UI_BUTTON_HOVER)
        return style->fill_hover;
    return style->fill_idle;
}

static void draw_label(sfRenderWindow *window, sfText *text,
    const ui_button_t *button, const ui_button_style_t *style)
{
    sfFloatRect label_bounds;
    sfVector2f position;

    sfText_setCharacterSize(text, style->text_size);
    sfText_setFillColor(text, style->text);
    sfText_setString(text, button->label);
    label_bounds = sfText_getLocalBounds(text);
    position.x = button->bounds.left +
        (button->bounds.width - label_bounds.width) / 2.0f -
        label_bounds.left;
    position.y = button->bounds.top +
        (button->bounds.height - label_bounds.height) / 2.0f -
        label_bounds.top;
    sfText_setPosition(text, position);
    sfRenderWindow_drawText(window, text, NULL);
}

void ui_button_draw(sfRenderWindow *window,
    sfText *text,
    const ui_button_t *button,
    const ui_button_style_t *style)
{
    sfColor fill;

    if (window == NULL || text == NULL || button == NULL || style == NULL)
        return;
    fill = get_fill(style, button->state);
    draw_rect(window, &button->bounds, &fill);
    draw_border(window, &button->bounds, &style->border);
    if (button->label == NULL)
        return;
    draw_label(window, text, button, style);
}
