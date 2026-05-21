/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu mouse handling for wolf3d
*/

#include "./../../include/menu.h"

static int button_bounds_from_state(gamestate_t state, int *last)
{
    if (state == GAME_MENU) {
        *last = 2;
        return 0;
    }
    if (state == GAME_SETTINGS) {
        *last = 8;
        return 3;
    }
    if (state == GAME_PAUSED) {
        *last = 12;
        return 9;
    }
    *last = -1;
    return -1;
}

static bool state_has_buttons(gamestate_t state)
{
    return state == GAME_MENU || state == GAME_SETTINGS ||
        state == GAME_PAUSED;
}

static int button_at(menu_t *menu,
    gamestate_t state, const sfVector2i *mouse_pos)
{
    sfFloatRect hitbox = {0};
    int first = 0;
    int last = -1;

    first = button_bounds_from_state(state, &last);
    for (int i = first; i <= last; i++) {
        hitbox = menu_get_button_hitbox(&menu->buttons[i]);
        if (mouse_pos->x >= (int)hitbox.left &&
            mouse_pos->x <= (int)(hitbox.left + hitbox.width) &&
            mouse_pos->y >= (int)hitbox.top &&
            mouse_pos->y <= (int)(hitbox.top + hitbox.height))
            return i;
    }
    return -1;
}

static void update_buttons(menu_t *menu,
    gamestate_t state, int hovered, bool keep_pressed)
{
    int first = 0;
    int last = -1;

    first = button_bounds_from_state(state, &last);
    for (int i = first; i <= last; i++) {
        if (keep_pressed && i == menu->pressed_button && i == hovered) {
            menu_set_button_state(&menu->buttons[i], MENU_BUTTON_CLICKED);
            continue;
        }
        if (i == hovered && !keep_pressed) {
            menu_set_button_state(&menu->buttons[i], MENU_BUTTON_HOVERED);
            continue;
        }
        menu_set_button_state(&menu->buttons[i], MENU_BUTTON_IDLE);
    }
}

static void queue_action(menu_t *menu, int hovered)
{
    menu->pending_button = hovered;
    menu->pending_action = menu->buttons[hovered].action;
    sfClock_restart(menu->action_clock);
    menu_set_button_state(&menu->buttons[hovered], MENU_BUTTON_CLICKED);
}

static size_t continue_from_cinematic(wolf_t *wolf)
{
    wolf->state = GAME_PLAY;
    menu_reset(wolf->menu, wolf->state);
    return (size_t)SETFML_SUCC;
}

static size_t release_menu_button(wolf_t *wolf, const sfVector2i *mouse_pos)
{
    int hovered = button_at(wolf->menu, wolf->state, mouse_pos);

    if (hovered == wolf->menu->pressed_button && hovered != -1)
        queue_action(wolf->menu, hovered);
    else
        update_buttons(wolf->menu, wolf->state, hovered, false);
    wolf->menu->pressed_button = -1;
    return (size_t)SETFML_SUCC;
}

size_t menu_on_mousemove(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    sfVector2i mouse_pos = {setfml->event.mouseMove.x,
        setfml->event.mouseMove.y};
    int hovered = -1;

    (void)userdata;
    if (!wolf || !wolf->menu)
        return (size_t)SETFML_FAIL;
    if (wolf->menu->pending_action != MENU_ACTION_NONE ||
        !state_has_buttons(wolf->state))
        return (size_t)SETFML_SUCC;
    hovered = button_at(wolf->menu, wolf->state, &mouse_pos);
    update_buttons(wolf->menu, wolf->state, hovered,
        wolf->menu->pressed_button != -1);
    return (size_t)SETFML_SUCC;
}

size_t menu_on_mousepress(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    sfVector2i mouse_pos = {setfml->event.mouseButton.x,
        setfml->event.mouseButton.y};
    int hovered = -1;

    (void)userdata;
    if (!wolf || !wolf->menu)
        return (size_t)SETFML_FAIL;
    if (setfml->event.mouseButton.button != sfMouseLeft ||
        wolf->menu->pending_action != MENU_ACTION_NONE ||
        !state_has_buttons(wolf->state))
        return (size_t)SETFML_SUCC;
    hovered = button_at(wolf->menu, wolf->state, &mouse_pos);
    if (hovered == -1)
        return (size_t)SETFML_SUCC;
    wolf->menu->pressed_button = hovered;
    menu_set_button_state(&wolf->menu->buttons[hovered], MENU_BUTTON_CLICKED);
    return (size_t)SETFML_SUCC;
}

size_t menu_on_mouserelease(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    sfVector2i mouse_pos = {setfml->event.mouseButton.x,
        setfml->event.mouseButton.y};

    (void)userdata;
    if (!wolf || !wolf->menu)
        return (size_t)SETFML_FAIL;
    if (setfml->event.mouseButton.button != sfMouseLeft)
        return (size_t)SETFML_SUCC;
    if (wolf->state == GAME_CINEMATIC)
        return continue_from_cinematic(wolf);
    if (wolf->menu->pending_action != MENU_ACTION_NONE ||
        !state_has_buttons(wolf->state))
        return (size_t)SETFML_SUCC;
    return release_menu_button(wolf, &mouse_pos);
}