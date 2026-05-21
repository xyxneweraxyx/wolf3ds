/*
** EPITECH PROJECT, 2025
** setfml
** File description:
** Tired of initializing csfml? Up your workflow with this simple wrapper.
*/

#include "./setfml.h"

static void fit_window_view(setfml_t *setfml)
{
    sfVector2u size = {0, 0};
    sfView *view = NULL;

    if (!setfml || !setfml->window)
        return;
    size = sfRenderWindow_getSize(setfml->window);
    view = sfView_createFromRect((sfFloatRect){0, 0, size.x, size.y});
    if (!view)
        return;
    sfRenderWindow_setView(setfml->window, view);
    sfView_destroy(view);
}

size_t setfml_windowrecreate(setfml_t *setfml,
    const sfVideoMode *mode, sfUint32 style)
{
    sfRenderWindow *window = NULL;

    if (!setfml || !mode)
        return (size_t)SETFML_FAIL;
    window = sfRenderWindow_create(*mode, setfml->params.window.title,
        style, setfml->params.window.settings);
    if (!window)
        return (size_t)SETFML_FAIL;
    if (setfml->window)
        sfRenderWindow_destroy(setfml->window);
    setfml->window = window;
    setfml->params.window.style = style;
    sfRenderWindow_setFramerateLimit(window,
        (unsigned int)setfml->params.window.fps);
    fit_window_view(setfml);
    return (size_t)SETFML_SUCC;
}

size_t setfml_windowcreate(setfml_t *setfml)
{
    sfVideoMode mode = {
        setfml->params.window.max_scr_res[0],
        setfml->params.window.max_scr_res[1],
        8};

    return setfml_windowrecreate(setfml, &mode, setfml->params.window.style);
}

size_t setfml_windowstart(setfml_t *setfml)
{
    sprite_t *sprite = NULL;
    sfVector2u windowsize = sfRenderWindow_getSize(setfml->window);

    if (!setfml || !setfml->window)
        return (size_t)SETFML_FAIL;
    for (node_t *node = setfml->sprites->head; node; node = node->next) {
        sprite = (sprite_t *)node->data;
        sprite->original_win_x = windowsize.x;
        sprite->original_win_y = windowsize.y;
    }
    sfRenderWindow_display(setfml->window);
    while (sfRenderWindow_isOpen(setfml->window))
        setfml_iteration(setfml);
    return (size_t)SETFML_SUCC;
}

size_t setfml_windowclose(setfml_t *setfml)
{
    if (!setfml || !setfml->window)
        return (size_t)SETFML_FAIL;
    sfRenderWindow_close(setfml->window);
    return (size_t)SETFML_SUCC;
}

size_t setfml_windowdestroy(setfml_t *setfml)
{
    if (!setfml || !setfml->window)
        return (size_t)SETFML_FAIL;
    sfRenderWindow_destroy(setfml->window);
    setfml->window = NULL;
    return (size_t)SETFML_SUCC;
}
