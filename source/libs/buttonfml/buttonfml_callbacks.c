/*
** EPITECH PROJECT, 2025
** buttonfml
** File description:
** Wrapper on the setfml library to create simple buttons easily.
*/

#include "./buttonfml.h"

static size_t is_mouse_on_button(button_t *button, size_t x, size_t y)
{
    sfFloatRect rect = sfSprite_getGlobalBounds(button->button->sprite);
    sfVector2u window = {0, 0};

    if (!button->setfml->window)
        return (size_t)BUTTONFML_SUCC;
    window = sfRenderWindow_getSize(button->setfml->window);
    rect.width = rect.width * ((float)window.x /
        (float)button->button->original_win_x);
    rect.height = rect.height * ((float)window.y /
        (float)button->button->original_win_y);
    rect.left = rect.left * ((float)window.x /
        (float)button->button->original_win_x);
    rect.top = rect.top * ((float)window.y /
        (float)button->button->original_win_y);
    if (x < (size_t)rect.left || x > (size_t)(rect.left + rect.width))
        return (size_t)BUTTONFML_FAIL;
    if (y < (size_t)rect.top || y > (size_t)(rect.top + rect.height))
        return (size_t)BUTTONFML_FAIL;
    return (size_t)BUTTONFML_SUCC;
}

static size_t buttonfml_mousemove(setfml_t *setfml, void *userdata)
{
    buttonfml_t *buttonfml = (buttonfml_t *)userdata;
    button_t *button = NULL;
    char text_name[BUFF_TEXT_NAME] = {0};

    for (node_t *node = buttonfml->buttons->head; node; node = node->next) {
        button = (button_t *)node->data;
        if (!button->is_visible || button->state != BUTTON_IDLE)
            continue;
        if (is_mouse_on_button(button, (size_t)setfml->event.mouseMove.x,
                (size_t)setfml->event.mouseMove.y) == (size_t)BUTTONFML_FAIL)
            continue;
        button->state = BUTTON_HOVERED;
        if (button->textures->hover[0]) {
            str_cat(text_name, 2, button->name, "_hover");
            setfml_spritechangetexture(setfml, button->button, text_name);
        }
        if (button->callbacks && button->callbacks->hover)
            button->callbacks->hover(setfml, userdata);
    }
    return (size_t)BUTTONFML_SUCC;
}

static size_t buttonfml_mouseclick(setfml_t *setfml, void *userdata)
{
    buttonfml_t *buttonfml = (buttonfml_t *)userdata;
    button_t *button = NULL;
    char text_name[BUFF_TEXT_NAME] = {0};

    for (node_t *node = buttonfml->buttons->head; node; node = node->next) {
        button = (button_t *)node->data;
        if (!button->is_visible || !button->is_clickable)
            continue;
        if (is_mouse_on_button(button, (size_t)setfml->event.mouseButton.x,
                (size_t)setfml->event.mouseButton.y) == (size_t)BUTTONFML_FAIL)
            continue;
        button->state = BUTTON_CLICKED;
        if (button->textures->click[0]) {
            str_cat(text_name, 2, button->name, "_click");
            setfml_spritechangetexture(setfml, button->button, text_name);
        }
        if (button->callbacks && button->callbacks->click)
            button->callbacks->click(setfml, userdata);
    }
    return (size_t)BUTTONFML_SUCC;
}

static size_t buttonfml_frame(setfml_t *setfml, void *userdata)
{
    buttonfml_t *buttonfml = (buttonfml_t *)userdata;
    button_t *button = NULL;

    for (node_t *node = buttonfml->buttons->head; node; node = node->next) {
        button = (button_t *)node->data;
        if (button->callbacks && button->callbacks->frame)
            button->callbacks->frame(setfml, userdata);
    }
    return (size_t)BUTTONFML_SUCC;
}

static size_t buttonfml_draw(setfml_t *setfml, void *userdata)
{
    buttonfml_t *buttonfml = (buttonfml_t *)userdata;
    button_t *button = NULL;

    for (node_t *node = buttonfml->buttons->head; node; node = node->next) {
        button = (button_t *)node->data;
        if (!button->is_visible)
            continue;
        sfRenderWindow_drawSprite(setfml->window,
            button->button->sprite, NULL);
    }
    return (size_t)BUTTONFML_SUCC;
}

static size_t connect_1(buttonfml_t *buttonfml,
    node_t *node, function_t *function)
{
    setfml_add(buttonfml->setfml, &(setfml_func_comp_t){NULL,
            &buttonfml_draw}, "buttonfml_draw", LOOP_DRAW);
    node = setfml_nodefromfunc(buttonfml->setfml, "buttonfml_draw",
        LOOP_DRAW);
    if (!node)
        return (size_t)BUTTONFML_FAIL;
    function = (function_t *)node->data;
    function->userdata = (void *)buttonfml;
    setfml_add(buttonfml->setfml, &(setfml_func_comp_t){NULL,
            &buttonfml_mousemove}, "buttonfml_mousemove", sfEvtMouseMoved);
    node = setfml_nodefromfunc(buttonfml->setfml, "buttonfml_mousemove",
        sfEvtMouseMoved);
    if (!node)
        return (size_t)BUTTONFML_FAIL;
    function = (function_t *)node->data;
    function->userdata = (void *)buttonfml;
    return (size_t)BUTTONFML_SUCC;
}

static size_t connect_2(buttonfml_t *buttonfml,
    node_t *node, function_t *function)
{
    setfml_add(buttonfml->setfml, &(setfml_func_comp_t){NULL,
            &buttonfml_mouseclick}, "buttonfml_mouseclick",
        sfEvtMouseButtonPressed);
    node = setfml_nodefromfunc(buttonfml->setfml, "buttonfml_mouseclick",
        sfEvtMouseButtonPressed);
    if (!node)
        return (size_t)BUTTONFML_FAIL;
    function = (function_t *)node->data;
    function->userdata = (void *)buttonfml;
    setfml_add(buttonfml->setfml, &(setfml_func_comp_t){NULL,
            &buttonfml_frame}, "buttonfml_frame", LOOP_RENDER);
    node = setfml_nodefromfunc(buttonfml->setfml, "buttonfml_frame",
        LOOP_RENDER);
    if (!node)
        return (size_t)BUTTONFML_FAIL;
    function = (function_t *)node->data;
    function->userdata = (void *)buttonfml;
    return (size_t)BUTTONFML_SUCC;
}

size_t buttonfml_connectcallbacks(buttonfml_t *buttonfml)
{
    node_t *node = NULL;
    function_t *function = NULL;

    if (connect_1(buttonfml, node, function) == (size_t)BUTTONFML_FAIL)
        return (size_t)BUTTONFML_FAIL;
    if (connect_2(buttonfml, node, function) == (size_t)BUTTONFML_FAIL)
        return (size_t)BUTTONFML_FAIL;
    if (connect_3(buttonfml, node, function) == (size_t)BUTTONFML_FAIL)
        return (size_t)BUTTONFML_FAIL;
    if (connect_4(buttonfml, node, function) == (size_t)BUTTONFML_FAIL)
        return (size_t)BUTTONFML_FAIL;
    return (size_t)BUTTONFML_SUCC;
}
