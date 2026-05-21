/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu initialization for wolf3d
*/

#include "./../../include/menu.h"

static sfText *create_text(menu_t *menu, const char *string)
{
    sfText *text = sfText_create();

    if (!text)
        return NULL;
    sfText_setFont(text, menu->font);
    sfText_setString(text, string);
    sfText_setFillColor(text, sfWhite);
    sfText_setOutlineColor(text, sfBlack);
    sfText_setOutlineThickness(text, 3.0f);
    return text;
}

static bool init_texts(menu_t *menu)
{
    menu->title = create_text(menu, "WOLF3D");
    menu->subtitle = create_text(menu, "ENTER THE FACILITY");
    menu->settings_text = create_text(menu, "VOLUME: 100%");
    menu->cinematic_title = create_text(menu, "CINEMATIC");
    menu->cinematic_text = create_text(menu,
        "PLACEHOLDER SCREEN\nFOR THE INTRO SEQUENCE");
    menu->cinematic_hint = create_text(menu,
        "PRESS ENTER OR CLICK");
    if (!menu->title || !menu->subtitle || !menu->settings_text ||
        !menu->cinematic_title || !menu->cinematic_text ||
        !menu->cinematic_hint)
        return false;
    return true;
}

static bool init_button(menu_t *menu, menu_button_t *button,
    const char *label, menu_action_t action)
{
    button->sprite = sfSprite_create();
    button->text = create_text(menu, label);
    if (!button->sprite || !button->text)
        return false;
    sfSprite_setTexture(button->sprite, menu->button_texture, sfTrue);
    button->action = action;
    button->visible = false;
    menu_set_button_state(button, MENU_BUTTON_IDLE);
    return true;
}

static bool init_main_buttons(menu_t *menu)
{
    if (!init_button(menu, &menu->buttons[0], "START", MENU_ACTION_START))
        return false;
    if (!init_button(menu, &menu->buttons[1], "SETTINGS",
            MENU_ACTION_SETTINGS))
        return false;
    if (!init_button(menu, &menu->buttons[2], "QUIT", MENU_ACTION_QUIT))
        return false;
    return true;
}

static bool init_settings_buttons(menu_t *menu)
{
    if (!init_button(menu, &menu->buttons[3], "VOL -",
            MENU_ACTION_VOLUME_DOWN))
        return false;
    if (!init_button(menu, &menu->buttons[4], "VOL +",
            MENU_ACTION_VOLUME_UP))
        return false;
    if (!init_button(menu, &menu->buttons[5], "FULLSCREEN OFF",
            MENU_ACTION_FULLSCREEN))
        return false;
    if (!init_button(menu, &menu->buttons[6], "BACK", MENU_ACTION_BACK))
        return false;
    return true;
}

static bool init_pause_buttons(menu_t *menu)
{
    if (!init_button(menu, &menu->buttons[7], "SAVE", MENU_ACTION_SAVE))
        return false;
    if (!init_button(menu, &menu->buttons[8], "LOAD", MENU_ACTION_LOAD))
        return false;
    if (!init_button(menu, &menu->buttons[9], "BACK TO MENU",
            MENU_ACTION_BACK))
        return false;
    if (!init_button(menu, &menu->buttons[10], "EXIT", MENU_ACTION_QUIT))
        return false;
    return true;
}

static bool init_buttons(menu_t *menu)
{
    if (!init_main_buttons(menu) || !init_settings_buttons(menu))
        return false;
    if (!init_pause_buttons(menu))
        return false;
    return true;
}

static bool init_assets(menu_t *menu)
{
    menu->background_texture = sfTexture_createFromFile(MENU_TXT_BACKGROUND,
        NULL);
    menu->button_texture = sfTexture_createFromFile(MENU_TXT_BUTTONS, NULL);
    menu->font = sfFont_createFromFile(MENU_FONT_MAIN);
    menu->background_sprite = sfSprite_create();
    menu->overlay = sfRectangleShape_create();
    menu->action_clock = sfClock_create();
    menu->cinematic_clock = sfClock_create();
    if (!menu->background_texture || !menu->button_texture || !menu->font ||
        !menu->background_sprite || !menu->overlay || !menu->action_clock ||
        !menu->cinematic_clock)
        return false;
    sfSprite_setTexture(menu->background_sprite, menu->background_texture,
        sfTrue);
    sfRectangleShape_setFillColor(menu->overlay, (sfColor){0, 0, 0, 170});
    return true;
}

int menu_init(wolf_t *wolf)
{
    menu_t *menu = NULL;

    if (!wolf || !wolf->alloc)
        return WOLF_FAIL;
    menu = c_alloc(sizeof(menu_t), 1, wolf->alloc);
    if (!menu)
        return WOLF_FAIL;
    wolf->menu = menu;
    menu->windowed_size = (sfVector2u){0, 0};
    menu->fullscreen = false;
    menu->volume = 100;
    menu->pressed_button = -1;
    menu->pending_button = -1;
    menu->pending_action = MENU_ACTION_NONE;
    if (!init_assets(menu) || !init_texts(menu) || !init_buttons(menu))
        return WOLF_FAIL;
    menu_reset(menu, wolf->state);
    return WOLF_SUCC;
}
