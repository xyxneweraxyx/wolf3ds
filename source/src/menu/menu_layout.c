/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu layout for wolf3d
*/

#include "./../../include/menu.h"

static unsigned int clamp_size(unsigned int value,
    unsigned int min, unsigned int max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

static void set_button_layout(menu_button_t *button,
    float center_x, float top_y, float width)
{
    float scale = width / (float)MENU_BUTTON_BODY_WIDTH;
    sfFloatRect hitbox = {0};

    button->scale = (sfVector2f){scale, scale};
    button->position = (sfVector2f){center_x - width / 2.0f, top_y};
    sfSprite_setScale(button->sprite, button->scale);
    sfSprite_setPosition(button->sprite, button->position);
    menu_set_button_state(button, button->state);
    hitbox = menu_get_button_hitbox(button);
    menu_center_text(button->text, &hitbox);
}

static void set_main_texts(menu_t *menu, const sfVector2u *size,
    unsigned int title_size, unsigned int small_size)
{
    sfText_setString(menu->title, "WOLF3D");
    sfText_setCharacterSize(menu->title, title_size);
    sfText_setCharacterSize(menu->subtitle, small_size);
    menu_center_full_width(menu->title,
        size, size->y * 0.12f, title_size * 1.5f);
    menu_center_full_width(menu->subtitle,
        size, size->y * 0.24f, small_size * 1.5f);
}

static void set_main_buttons(menu_t *menu,
    const sfVector2u *size, float button_width, unsigned int button_size)
{
    float button_height = button_width * (float)MENU_BUTTON_BODY_HEIGHT /
        (float)MENU_BUTTON_BODY_WIDTH;
    float gap = button_height + (float)size->y * 0.035f;
    float top = (float)size->y * 0.38f;

    for (int i = 0; i < 3; i++)
        sfText_setCharacterSize(menu->buttons[i].text, button_size);
    set_button_layout(&menu->buttons[0], size->x / 2.0f, top, button_width);
    set_button_layout(&menu->buttons[1], size->x / 2.0f, top + gap,
        button_width);
    set_button_layout(&menu->buttons[2], size->x / 2.0f, top + gap * 2.0f,
        button_width);
}

static void set_main_layout(menu_t *menu,
    const sfVector2u *size, float button_width)
{
    unsigned int title_size = clamp_size(size->y / 18, 32, 60);
    unsigned int small_size = clamp_size(size->y / 36, 14, 22);
    unsigned int button_size = clamp_size(size->y / 34, 16, 28);

    set_main_texts(menu, size, title_size, small_size);
    set_main_buttons(menu, size, button_width, button_size);
}

static void set_cinematic_layout(menu_t *menu, const sfVector2u *size)
{
    unsigned int title_size = clamp_size(size->y / 18, 30, 56);
    unsigned int text_size = clamp_size(size->y / 30, 16, 28);
    unsigned int hint_size = clamp_size(size->y / 40, 12, 18);

    sfText_setCharacterSize(menu->cinematic_title, title_size);
    sfText_setCharacterSize(menu->cinematic_text, text_size);
    sfText_setCharacterSize(menu->cinematic_hint, hint_size);
    menu_center_full_width(menu->cinematic_title,
        size, size->y * 0.16f, title_size * 1.5f);
    menu_center_full_width(menu->cinematic_text,
        size, size->y * 0.38f, text_size * 3.0f);
    menu_center_full_width(menu->cinematic_hint,
        size, size->y * 0.74f, hint_size * 1.5f);
}

static void set_pause_buttons(menu_t *menu,
    const sfVector2u *size, float button_width, unsigned int button_size)
{
    float button_height = 0.0f;
    float gap = 0.0f;
    float top = (float)size->y * 0.18f;

    if (button_width > 500.0f)
        button_width = 500.0f;
    button_height = button_width * (float)MENU_BUTTON_BODY_HEIGHT /
        (float)MENU_BUTTON_BODY_WIDTH;
    gap = button_height + (float)size->y * 0.008f;
    for (int i = 9; i <= 12; i++)
        sfText_setCharacterSize(menu->buttons[i].text, button_size);
    for (int i = 9; i <= 12; i++)
        set_button_layout(&menu->buttons[i], size->x / 2.0f,
            top + gap * (float)(i - 9), button_width);
}

static void set_pause_layout(menu_t *menu,
    const sfVector2u *size, float button_width)
{
    unsigned int title_size = clamp_size(size->y / 18, 30, 56);
    unsigned int button_size = clamp_size(size->y / 36, 14, 26);

    sfText_setString(menu->title, "PAUSED");
    sfText_setCharacterSize(menu->title, title_size);
    menu_center_full_width(menu->title,
        size, size->y * 0.055f, title_size * 1.5f);
    set_pause_buttons(menu, size, button_width, button_size);
}

static void set_layout_for_state(menu_t *menu,
    const sfVector2u *size, float button_width, gamestate_t state)
{
    if (state == GAME_MENU)
        set_main_layout(menu, size, button_width);
    if (state == GAME_SETTINGS)
        menu_set_settings_layout(menu, size, button_width);
    if (state == GAME_CINEMATIC)
        set_cinematic_layout(menu, size);
    if (state == GAME_PAUSED)
        set_pause_layout(menu, size, button_width);
}

void menu_refresh(menu_t *menu, const sfVector2u *size, gamestate_t state)
{
    float button_width = (float)size->x * 0.78f;
    sfVector2f scale = {0};
    sfVector2f overlay_size = {0};

    if (!menu || !size)
        return;
    if (button_width > 720.0f)
        button_width = 720.0f;
    if (button_width < 500.0f)
        button_width = 500.0f;
    scale.x = (float)size->x / 1536.0f;
    scale.y = (float)size->y / 1024.0f;
    overlay_size.x = (float)size->x;
    overlay_size.y = (float)size->y;
    sfSprite_setScale(menu->background_sprite, scale);
    sfRectangleShape_setSize(menu->overlay, overlay_size);
    set_layout_for_state(menu, size, button_width, state);
}
