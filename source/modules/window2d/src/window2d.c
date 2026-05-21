/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Generic 2D window helper implementation
*/

#include "window2d.h"

int window2d_create(window2d_t *window, const window2d_options_t *options)
{
    sfVideoMode mode;

    if (window == NULL || options == NULL || options->title == NULL)
        return 84;
    mode.width = options->width;
    mode.height = options->height;
    mode.bitsPerPixel = 32;
    window->handle = sfRenderWindow_create(mode,
        options->title,
        options->style,
        NULL);
    if (window->handle == NULL)
        return 84;
    window->width = options->width;
    window->height = options->height;
    sfRenderWindow_setFramerateLimit(window->handle, options->framerate);
    return 0;
}

void window2d_destroy(window2d_t *window)
{
    if (window == NULL)
        return;
    if (window->handle != NULL)
        sfRenderWindow_destroy(window->handle);
    window->handle = NULL;
}

void window2d_clear(window2d_t *window, const sfColor *color)
{
    if (window == NULL || window->handle == NULL || color == NULL)
        return;
    sfRenderWindow_clear(window->handle, *color);
}

void window2d_display(window2d_t *window)
{
    if (window == NULL || window->handle == NULL)
        return;
    sfRenderWindow_display(window->handle);
}

sfBool window2d_poll_event(window2d_t *window, sfEvent *event)
{
    if (window == NULL || window->handle == NULL || event == NULL)
        return sfFalse;
    return sfRenderWindow_pollEvent(window->handle, event);
}
