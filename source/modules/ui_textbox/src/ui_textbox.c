/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Reusable textbox input helpers
*/

#include <string.h>
#include "ui_textbox.h"

void ui_textbox_init(ui_textbox_t *textbox,
    const ui_textbox_options_t *options)
{
    if (textbox == NULL || options == NULL)
        return;
    textbox->bounds = options->bounds;
    textbox->buffer = options->buffer;
    textbox->capacity = options->capacity;
    textbox->max_length = options->max_length;
    textbox->active = sfFalse;
}

sfBool ui_textbox_contains(const ui_textbox_t *textbox, int x, int y)
{
    if (textbox == NULL)
        return sfFalse;
    if ((float)x < textbox->bounds.left || (float)y < textbox->bounds.top)
        return sfFalse;
    if ((float)x > textbox->bounds.left + textbox->bounds.width)
        return sfFalse;
    if ((float)y > textbox->bounds.top + textbox->bounds.height)
        return sfFalse;
    return sfTrue;
}

static size_t get_limit(const ui_textbox_t *textbox)
{
    size_t max_for_capacity;

    if (textbox->capacity == 0)
        return 0;
    max_for_capacity = textbox->capacity - 1;
    if (textbox->max_length == 0 || textbox->max_length > max_for_capacity)
        return max_for_capacity;
    return textbox->max_length;
}

static void delete_last_char(ui_textbox_t *textbox)
{
    size_t len;

    len = strlen(textbox->buffer);
    if (len > 0)
        textbox->buffer[len - 1] = '\0';
}

void ui_textbox_handle_digit_input(ui_textbox_t *textbox, sfUint32 unicode)
{
    size_t len;

    if (textbox == NULL || textbox->buffer == NULL)
        return;
    if (unicode == 8)
        return delete_last_char(textbox);
    if (unicode < '0' || unicode > '9')
        return;
    len = strlen(textbox->buffer);
    if (len >= get_limit(textbox))
        return;
    textbox->buffer[len] = (char)unicode;
    textbox->buffer[len + 1] = '\0';
}
