/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Settings menu layout for wolf3d
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

static int fill_volume_number(char label[32], int volume)
{
    int index = 8;

    if (volume == 100) {
        label[index] = '1';
        label[index + 1] = '0';
        label[index + 2] = '0';
        return index + 3;
    }
    if (volume >= 10) {
        label[index] = volume / 10 + '0';
        label[index + 1] = volume % 10 + '0';
        return index + 2;
    }
    label[index] = volume + '0';
    return index + 1;
}

static void fill_volume_label(char label[32], int volume)
{
    int index = 0;

    str_cpy("VOLUME: ", label);
    index = fill_volume_number(label, volume);
    label[index] = '%';
    label[index + 1] = '\0';
}

static void set_settings_label(menu_t *menu,
    const sfVector2u *size, unsigned int text_size)
{
    char label[32] = {0};

    fill_volume_label(label, menu->volume);
    sfText_setString(menu->settings_text, label);
    sfText_setCharacterSize(menu->settings_text, text_size);
    menu_center_full_width(menu->settings_text,
        size, size->y * 0.22f, text_size * 1.5f);
}

static void set_fullscreen_label(menu_t *menu)
{
    if (menu->fullscreen)
        sfText_setString(menu->buttons[5].text, "FULLSCREEN ON");
    else
        sfText_setString(menu->buttons[5].text, "FULLSCREEN OFF");
}

static void set_settings_buttons(menu_t *menu,
    const sfVector2u *size, float button_width, unsigned int button_size)
{
    float small_width = button_width * 0.46f;
    float center_x = (float)size->x / 2.0f;
    float shift = small_width * 0.58f;

    set_fullscreen_label(menu);
    for (int i = 3; i <= 6; i++)
        sfText_setCharacterSize(menu->buttons[i].text, button_size);
    set_button_layout(&menu->buttons[3], center_x - shift,
        size->y * 0.30f, small_width);
    set_button_layout(&menu->buttons[4], center_x + shift,
        size->y * 0.30f, small_width);
    set_button_layout(&menu->buttons[5], center_x,
        size->y * 0.43f, button_width);
    set_button_layout(&menu->buttons[6], center_x,
        size->y * 0.64f, button_width);
}

void menu_set_settings_layout(menu_t *menu,
    const sfVector2u *size, float button_width)
{
    unsigned int title_size = clamp_size(size->y / 18, 30, 56);
    unsigned int text_size = clamp_size(size->y / 30, 18, 26);
    unsigned int button_size = clamp_size(size->y / 34, 16, 28);

    sfText_setString(menu->title, "SETTINGS");
    sfText_setCharacterSize(menu->title, title_size);
    menu_center_full_width(menu->title,
        size, size->y * 0.09f, title_size * 1.5f);
    set_settings_label(menu, size, text_size);
    set_settings_buttons(menu, size, button_width, button_size);
}
