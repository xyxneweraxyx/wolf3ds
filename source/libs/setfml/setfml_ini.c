/*
** EPITECH PROJECT, 2025
** setfml
** File description:
** Tired of initializing csfml? Up your workflow with this simple wrapper.
*/

#include "./setfml.h"

static size_t clamp(size_t x, size_t min, size_t max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

static size_t setfml_closewindow(setfml_t *setfml, void *userdata)
{
    (void)userdata;
    setfml_windowclose(setfml);
    return (size_t)SETFML_WINDOWCLOSING;
}

static void setfml_fit_view(setfml_t *setfml)
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

static size_t setfml_resizewindow(setfml_t *setfml, void *userdata)
{
    sfVector2u realsize = {0, 0};
    sfVector2u fixedsize = {0, 0};

    (void)userdata;
    if (setfml->params.window.style & sfFullscreen) {
        setfml_fit_view(setfml);
        return (size_t)SETFML_SUCC;
    }
    realsize = sfRenderWindow_getSize(setfml->window);
    fixedsize = sfRenderWindow_getSize(setfml->window);
    fixedsize.x = clamp(fixedsize.x, setfml->params.window.min_scr_res[0],
        setfml->params.window.max_scr_res[0]);
    fixedsize.y = clamp(fixedsize.y, setfml->params.window.min_scr_res[1],
        setfml->params.window.max_scr_res[1]);
    if (fixedsize.x != realsize.x || fixedsize.y != realsize.y)
        sfRenderWindow_setSize(setfml->window, fixedsize);
    setfml_fit_view(setfml);
    return (size_t)SETFML_SUCC;
}

void setfml_fillparams(setfml_t *setfml)
{
    if (!setfml)
        return;
    setfml->params.window.fps = (uint8_t)240;
    setfml->params.window.max_scr_res[0] = (uint16_t)1920;
    setfml->params.window.max_scr_res[1] = (uint16_t)1080;
    setfml->params.window.min_scr_res[0] = (uint16_t)800;
    setfml->params.window.min_scr_res[1] = (uint16_t)600;
    setfml->params.window.settings = NULL;
    setfml->params.window.title = "Bowser's Revenge";
    setfml->params.window.style = sfDefaultStyle;
    setfml->params.time.event = 0;
    setfml->params.time.draw = 0;
    setfml->params.time.data = 1 * MILLI_TO_NANO;
    setfml->params.time.render = 0;
}

setfml_t *setfml_ini(void *userdata)
{
    c_alloc_t *alloc = c_ini((uint16_t)C_ALLOC_BASE);
    setfml_t *setfml = NULL;

    if (!alloc)
        return NULL;
    setfml = c_alloc(sizeof(setfml_t), 1, alloc);
    if (!setfml)
        return NULL;
    setfml->alloc = alloc;
    setfml->userdata = userdata;
    setfml->sprites = linkedlist_ini();
    setfml->textures = linkedlist_ini();
    for (size_t i = 0; i < (size_t)SETFML_LINKEDLIST_AMT; i++)
        setfml->functions[i] = linkedlist_ini();
    setfml_add(setfml, &(setfml_func_comp_t){NULL, &setfml_closewindow},
        "closewindow", sfEvtClosed);
    setfml_add(setfml, &(setfml_func_comp_t){NULL, &setfml_resizewindow},
        "setfml_resizewindow", sfEvtResized);
    setfml_fillparams(setfml);
    return setfml;
}

static void destroy_textures(setfml_t *setfml)
{
    texture_t *texture = NULL;

    for (node_t *node = setfml->textures->head; node; node = node->next) {
        texture = (texture_t *)node->data;
        if (texture->texture)
            sfTexture_destroy(texture->texture);
    }
}

static void destroy_sprites(setfml_t *setfml)
{
    sprite_t *sprite = NULL;

    for (node_t *node = setfml->sprites->head; node; node = node->next) {
        sprite = (sprite_t *)node->data;
        if (sprite->sprite)
            sfSprite_destroy(sprite->sprite);
    }
}

size_t setfml_destroy(setfml_t *setfml)
{
    destroy_textures(setfml);
    destroy_sprites(setfml);
    linkedlist_destroy(setfml->sprites, false);
    linkedlist_destroy(setfml->textures, false);
    for (size_t i = 0; i < (size_t)SETFML_LINKEDLIST_AMT; i++)
        linkedlist_destroy(setfml->functions[i], false);
    if (setfml->window)
        setfml_windowdestroy(setfml);
    c_delete(setfml->alloc, true);
    return (size_t)SETFML_SUCC;
}
