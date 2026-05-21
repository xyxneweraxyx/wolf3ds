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

static void reset_state_callback(setfml_t *setfml,
    btn_state_t state, button_t *button)
{
    if (state == BUTTON_CLICKED && button->state != BUTTON_CLICKED
        && button->callbacks && button->callbacks->rel)
        button->callbacks->rel(setfml, (void *)button->buttonfml);
}

static void reset_state(setfml_t *setfml, button_t *button,
    char text_name[BUFF_TEXT_NAME], bool force_hover)
{
    btn_state_t state = button->state;

    if (is_mouse_on_button(button, (size_t)setfml->event.mouseMove.x,
            (size_t)setfml->event.mouseMove.y) == (size_t)BUTTONFML_SUCC) {
        if (!force_hover)
            return;
        if (button->textures->hover[0]) {
            button->state = BUTTON_HOVERED;
            str_cat(text_name, 2, button->name, "_hover");
        } else {
            button->state = BUTTON_IDLE;
            str_cat(text_name, 2, button->name, "_idle");
        }
        reset_state_callback(setfml, state, button);
        return;
    }
    button->state = BUTTON_IDLE;
    str_cat(text_name, 2, button->name, "_idle");
    reset_state_callback(setfml, state, button);
}

static size_t buttonfml_release(setfml_t *setfml, void *userdata)
{
    buttonfml_t *buttonfml = (buttonfml_t *)userdata;
    button_t *button = NULL;
    char text_name[BUFF_TEXT_NAME] = {0};

    for (node_t *node = buttonfml->buttons->head; node; node = node->next) {
        button = (button_t *)node->data;
        if (!button->is_visible || !button->is_clickable ||
            button->state != BUTTON_CLICKED)
            continue;
        reset_state(setfml, button, text_name, true);
        setfml_spritechangetexture(setfml, button->button, text_name);
        if (button->callbacks && button->callbacks->rel)
            button->callbacks->rel(setfml, userdata);
    }
    return (size_t)BUTTONFML_SUCC;
}

static size_t buttonfml_reset(setfml_t *setfml, void *userdata)
{
    buttonfml_t *buttonfml = (buttonfml_t *)userdata;
    button_t *button = NULL;
    char text_name[BUFF_TEXT_NAME] = {0};

    for (node_t *node = buttonfml->buttons->head; node; node = node->next) {
        button = (button_t *)node->data;
        if (!button->is_visible || button->state == BUTTON_IDLE)
            continue;
        text_name[0] = 0;
        reset_state(setfml, button, text_name, false);
        if (text_name[0])
            setfml_spritechangetexture(setfml, button->button, text_name);
    }
    return (size_t)BUTTONFML_SUCC;
}

size_t connect_3(buttonfml_t *buttonfml,
    node_t *node, function_t *function)
{
    setfml_add(buttonfml->setfml, &(setfml_func_comp_t){NULL,
            &buttonfml_reset}, "buttonfml_reset", sfEvtMouseMoved);
    node = setfml_nodefromfunc(buttonfml->setfml, "buttonfml_reset",
        sfEvtMouseMoved);
    if (!node)
        return (size_t)BUTTONFML_FAIL;
    function = (function_t *)node->data;
    function->userdata = (void *)buttonfml;
    setfml_add(buttonfml->setfml, &(setfml_func_comp_t){NULL,
            &buttonfml_reset}, "buttonfml_reset2", sfEvtMouseButtonReleased);
    node = setfml_nodefromfunc(buttonfml->setfml, "buttonfml_reset2",
        sfEvtMouseButtonReleased);
    if (!node)
        return (size_t)BUTTONFML_FAIL;
    function = (function_t *)node->data;
    function->userdata = (void *)buttonfml;
    return (size_t)BUTTONFML_SUCC;
}

size_t connect_4(buttonfml_t *buttonfml,
    node_t *node, function_t *function)
{
    setfml_add(buttonfml->setfml, &(setfml_func_comp_t){NULL,
            &buttonfml_release}, "buttonfml_release", sfEvtMouseButtonReleased);
    node = setfml_nodefromfunc(buttonfml->setfml, "buttonfml_release",
        sfEvtMouseButtonReleased);
    if (!node)
        return (size_t)BUTTONFML_FAIL;
    function = (function_t *)node->data;
    function->userdata = (void *)buttonfml;
    return (size_t)BUTTONFML_SUCC;
}
