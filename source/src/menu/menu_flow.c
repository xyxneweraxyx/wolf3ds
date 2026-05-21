/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu state flow for wolf3d
*/

#include "./../../include/menu.h"

static float get_elapsed_seconds(sfClock *clock)
{
    return (float)sfClock_getElapsedTime(clock).microseconds / 1000000.0f;
}

static void clear_pending(menu_t *menu)
{
    menu->pending_action = MENU_ACTION_NONE;
    menu->pending_button = -1;
}

size_t menu_on_render(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    sfVector2u size = {0};

    (void)userdata;
    if (!wolf || !wolf->menu || !setfml->window)
        return (size_t)SETFML_FAIL;
    if (!wolf->menu->fullscreen)
        wolf->menu->windowed_size = sfRenderWindow_getSize(setfml->window);
    if (wolf->state != GAME_PLAY) {
        size = sfRenderWindow_getSize(setfml->window);
        menu_refresh(wolf->menu, &size, wolf->state);
    }
    if (wolf->menu->pending_action == MENU_ACTION_NONE)
        return (size_t)SETFML_SUCC;
    if (get_elapsed_seconds(wolf->menu->action_clock) < MENU_ACTION_DELAY)
        return (size_t)SETFML_SUCC;
    menu_apply_action(wolf, wolf->menu->pending_action);
    clear_pending(wolf->menu);
    menu_reset(wolf->menu, wolf->state);
    return (size_t)SETFML_SUCC;
}
