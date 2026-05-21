/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu text helpers for wolf3d
*/

#include "./../../include/menu.h"

void menu_center_text(sfText *text, const sfFloatRect *rect)
{
    sfFloatRect bounds = {0};
    sfVector2f position = {0};

    if (!text || !rect)
        return;
    bounds = sfText_getLocalBounds(text);
    position.x = rect->left + rect->width / 2.0f - bounds.width / 2.0f -
        bounds.left;
    position.y = rect->top + rect->height / 2.0f - bounds.height / 2.0f -
        bounds.top;
    sfText_setPosition(text, position);
}

void menu_center_full_width(sfText *text,
    const sfVector2u *size, float top, float height)
{
    sfFloatRect rect = {0, top, (float)size->x, height};

    menu_center_text(text, &rect);
}
