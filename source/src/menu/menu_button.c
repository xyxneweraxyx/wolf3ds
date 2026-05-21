/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu button helpers for wolf3d
*/

#include "./../../include/menu.h"

static sfIntRect get_button_rect(menu_button_state_t state)
{
    int top = MENU_BUTTON_IDLE_TOP;

    if (state == MENU_BUTTON_HOVERED)
        top = MENU_BUTTON_HOVERED_TOP;
    if (state == MENU_BUTTON_CLICKED)
        top = MENU_BUTTON_CLICKED_TOP;
    return (sfIntRect){MENU_BUTTON_BODY_X,
        (int)state * MENU_BUTTON_FRAME_HEIGHT + top,
        MENU_BUTTON_BODY_WIDTH, MENU_BUTTON_BODY_HEIGHT};
}

void menu_set_button_state(menu_button_t *button, menu_button_state_t state)
{
    if (!button || !button->sprite)
        return;
    button->state = state;
    sfSprite_setTextureRect(button->sprite, get_button_rect(state));
}

sfFloatRect menu_get_button_hitbox(const menu_button_t *button)
{
    return (sfFloatRect){
        button->position.x,
        button->position.y,
        button->scale.x * MENU_BUTTON_BODY_WIDTH,
        button->scale.y * MENU_BUTTON_BODY_HEIGHT
    };
}

static void set_buttons_visibility(menu_t *menu, int first, int last)
{
    for (int i = first; i <= last; i++)
        menu->buttons[i].visible = true;
}

void menu_reset(menu_t *menu, gamestate_t state)
{
    if (!menu)
        return;
    for (int i = 0; i < MENU_BUTTON_COUNT; i++) {
        menu->buttons[i].visible = false;
        menu_set_button_state(&menu->buttons[i], MENU_BUTTON_IDLE);
    }
    if (state == GAME_MENU)
        set_buttons_visibility(menu, 0, 2);
    if (state == GAME_SETTINGS)
        set_buttons_visibility(menu, 3, 8);
    if (state == GAME_PAUSED)
        set_buttons_visibility(menu, 9, 12);
    menu->pressed_button = -1;
    menu->pending_button = -1;
}