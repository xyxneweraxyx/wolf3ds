/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu settings actions for wolf3d
*/

#include "./../../include/menu.h"

static void set_sound_volume(sound_t *sound, float volume)
{
    if (sound && sound->sound)
        sfSound_setVolume(sound->sound, volume);
}

static void change_volume(wolf_t *wolf, int amount)
{
    float ratio = 0.0f;

    wolf->menu->volume += amount;
    if (wolf->menu->volume < 0)
        wolf->menu->volume = 0;
    if (wolf->menu->volume > 100)
        wolf->menu->volume = 100;
    ratio = (float)wolf->menu->volume / 100.0f;
    set_sound_volume(wolf->step_sound, SFML_STEP_VOLUME * ratio);
    set_sound_volume(wolf->shoot_sound, SFML_SHOOT_VOLUME * ratio);
}

static unsigned int clamp_dimension(unsigned int value,
    unsigned int min, unsigned int max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

static void set_windowed_mode(wolf_t *wolf, sfVideoMode *mode)
{
    sfVector2u size = wolf->menu->windowed_size;

    if (size.x == 0 || size.y == 0) {
        size.x = wolf->setfml->params.window.max_scr_res[0];
        size.y = wolf->setfml->params.window.max_scr_res[1];
    }
    size.x = clamp_dimension(size.x, wolf->setfml->params.window.min_scr_res[0],
        wolf->setfml->params.window.max_scr_res[0]);
    size.y = clamp_dimension(size.y, wolf->setfml->params.window.min_scr_res[1],
        wolf->setfml->params.window.max_scr_res[1]);
    *mode = (sfVideoMode){size.x, size.y, 8};
}

static void open_windowed(wolf_t *wolf)
{
    sfVideoMode mode = {0, 0, 0};

    set_windowed_mode(wolf, &mode);
    if (setfml_windowrecreate(wolf->setfml, &mode, sfDefaultStyle) ==
        (size_t)SETFML_SUCC)
        wolf->menu->fullscreen = false;
}

static void open_fullscreen(wolf_t *wolf)
{
    sfVideoMode mode = sfVideoMode_getDesktopMode();

    if (setfml_windowrecreate(wolf->setfml, &mode, sfFullscreen) ==
        (size_t)SETFML_SUCC)
        wolf->menu->fullscreen = true;
}

static void toggle_fullscreen(wolf_t *wolf)
{
    if (wolf->menu->fullscreen) {
        open_windowed(wolf);
        return;
    }
    wolf->menu->windowed_size = sfRenderWindow_getSize(wolf->setfml->window);
    open_fullscreen(wolf);
}

void menu_apply_action(wolf_t *wolf, menu_action_t action)
{
    if (action == MENU_ACTION_START) {
        wolf->state = GAME_CINEMATIC;
        sfClock_restart(wolf->menu->cinematic_clock);
    }
    if (action == MENU_ACTION_SETTINGS)
        wolf->state = GAME_SETTINGS;
    if (action == MENU_ACTION_VOLUME_DOWN)
        change_volume(wolf, -10);
    if (action == MENU_ACTION_VOLUME_UP)
        change_volume(wolf, 10);
    if (action == MENU_ACTION_FULLSCREEN)
        toggle_fullscreen(wolf);
    if (action == MENU_ACTION_BACK)
        wolf->state = GAME_MENU;
    if (action == MENU_ACTION_QUIT)
        setfml_windowclose(wolf->setfml);
}
