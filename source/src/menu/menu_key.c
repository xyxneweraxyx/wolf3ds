/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu keyboard handling for wolf3d
*/

#include "./../../include/menu.h"

static void handle_menu_key(wolf_t *wolf, sfKeyCode key)
{
    if (key == sfKeyEscape)
        setfml_windowclose(wolf->setfml);
}

static void handle_settings_key(wolf_t *wolf, sfKeyCode key)
{
    if (key != sfKeyEscape && key != sfKeyBackspace)
        return;
    wolf->state = GAME_MENU;
    menu_reset(wolf->menu, wolf->state);
}

static void handle_cinematic_key(wolf_t *wolf, sfKeyCode key)
{
    if (key != sfKeyEnter && key != sfKeySpace && key != sfKeyEscape)
        return;
    wolf->state = GAME_PLAY;
    menu_reset(wolf->menu, wolf->state);
}

static void stop_step_sound(wolf_t *wolf)
{
    if (wolf->step_sound && wolf->step_sound->sound)
        sfSound_stop(wolf->step_sound->sound);
}

static void handle_play_key(wolf_t *wolf, sfKeyCode key)
{
    if (key != sfKeyEscape)
        return;
    wolf->state = GAME_PAUSED;
    stop_step_sound(wolf);
    menu_reset(wolf->menu, wolf->state);
}

static void handle_paused_key(wolf_t *wolf, sfKeyCode key)
{
    if (key != sfKeyEscape)
        return;
    wolf->state = GAME_PLAY;
    menu_reset(wolf->menu, wolf->state);
}

static void handle_key_state(wolf_t *wolf, sfKeyCode key, gamestate_t state)
{
    if (state == GAME_MENU)
        handle_menu_key(wolf, key);
    if (state == GAME_SETTINGS)
        handle_settings_key(wolf, key);
    if (state == GAME_CINEMATIC)
        handle_cinematic_key(wolf, key);
    if (state == GAME_PLAY)
        handle_play_key(wolf, key);
    if (state == GAME_PAUSED)
        handle_paused_key(wolf, key);
}

size_t menu_on_keypress(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    sfKeyCode key = setfml->event.key.code;

    (void)userdata;
    if (!wolf || !wolf->menu)
        return (size_t)SETFML_FAIL;
    if (wolf->menu->pending_action != MENU_ACTION_NONE)
        return (size_t)SETFML_SUCC;
    handle_key_state(wolf, key, wolf->state);
    return (size_t)SETFML_SUCC;
}
