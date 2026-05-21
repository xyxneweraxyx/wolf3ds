/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu settings actions for wolf3d
*/

#include "./../../include/menu.h"
#include "./menu_private.h"

const sfVector2u MENU_RESOLUTIONS[MENU_RES_COUNT] = {
    {800, 600},
    {1024, 768},
    {1280, 720},
    {1920, 1080}
};

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

static void change_resolution(wolf_t *wolf, int delta)
{
    sfVector2u res = {0, 0};
    sfVideoMode mode = {0, 0, 0};
    int idx = wolf->menu->resolution_index + delta;

    if (idx < 0)
        idx = 0;
    if (idx >= MENU_RES_COUNT)
        idx = MENU_RES_COUNT - 1;
    wolf->menu->resolution_index = idx;
    res = MENU_RESOLUTIONS[idx];
    mode = (sfVideoMode){res.x, res.y, 8};
    setfml_windowrecreate(wolf->setfml, &mode, sfDefaultStyle);
    wolf->menu->fullscreen = false;
}

static void set_windowed_mode(wolf_t *wolf, sfVideoMode *mode)
{
    sfVector2u size = wolf->menu->windowed_size;
    unsigned int min_x = wolf->setfml->params.window.min_scr_res[0];
    unsigned int max_x = wolf->setfml->params.window.max_scr_res[0];
    unsigned int min_y = wolf->setfml->params.window.min_scr_res[1];
    unsigned int max_y = wolf->setfml->params.window.max_scr_res[1];

    if (size.x == 0 || size.y == 0) {
        size.x = max_x;
        size.y = max_y;
    }
    size.x = (size.x < min_x) ? min_x : (size.x > max_x ? max_x : size.x);
    size.y = (size.y < min_y) ? min_y : (size.y > max_y ? max_y : size.y);
    *mode = (sfVideoMode){size.x, size.y, 8};
}

static void toggle_fullscreen(wolf_t *wolf)
{
    sfVideoMode mode = {0, 0, 0};

    if (wolf->menu->fullscreen) {
        set_windowed_mode(wolf, &mode);
        if (setfml_windowrecreate(wolf->setfml, &mode, sfDefaultStyle)
            == (size_t)SETFML_SUCC)
            wolf->menu->fullscreen = false;
        return;
    }
    wolf->menu->windowed_size = sfRenderWindow_getSize(wolf->setfml->window);
    mode = sfVideoMode_getDesktopMode();
    if (setfml_windowrecreate(wolf->setfml, &mode, sfFullscreen)
        == (size_t)SETFML_SUCC)
        wolf->menu->fullscreen = true;
}

static void apply_game_actions(wolf_t *wolf, menu_action_t action)
{
    if (action == MENU_ACTION_START) {
        wolf->state = GAME_CINEMATIC;
        sfClock_restart(wolf->menu->cinematic_clock);
    }
    if (action == MENU_ACTION_BACK)
        wolf->state = GAME_MENU;
    if (action == MENU_ACTION_QUIT)
        setfml_windowclose(wolf->setfml);
    if (action == MENU_ACTION_SAVE)
        save_game(wolf);
    if (action == MENU_ACTION_LOAD && load_game(wolf) == WOLF_SUCC)
        wolf->state = GAME_PLAY;
}

static void apply_settings_actions(wolf_t *wolf, menu_action_t action)
{
    if (action == MENU_ACTION_SETTINGS)
        wolf->state = GAME_SETTINGS;
    if (action == MENU_ACTION_VOLUME_DOWN)
        change_volume(wolf, -10);
    if (action == MENU_ACTION_VOLUME_UP)
        change_volume(wolf, 10);
    if (action == MENU_ACTION_RES_DOWN)
        change_resolution(wolf, -1);
    if (action == MENU_ACTION_RES_UP)
        change_resolution(wolf, 1);
    if (action == MENU_ACTION_FULLSCREEN)
        toggle_fullscreen(wolf);
}

void menu_apply_action(wolf_t *wolf, menu_action_t action)
{
    apply_game_actions(wolf, action);
    apply_settings_actions(wolf, action);
}