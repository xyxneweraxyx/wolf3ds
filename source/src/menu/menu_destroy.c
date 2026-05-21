/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu destruction for wolf3d
*/

#include "./../../include/menu.h"

static void destroy_text(sfText *text)
{
    if (text)
        sfText_destroy(text);
}

static void destroy_button(menu_button_t *button)
{
    if (button->sprite)
        sfSprite_destroy(button->sprite);
    if (button->text)
        sfText_destroy(button->text);
}

static void destroy_buttons(menu_t *menu)
{
    for (int i = 0; i < MENU_BUTTON_COUNT; i++)
        destroy_button(&menu->buttons[i]);
}

static void destroy_shapes(menu_t *menu)
{
    if (menu->overlay)
        sfRectangleShape_destroy(menu->overlay);
    if (menu->background_sprite)
        sfSprite_destroy(menu->background_sprite);
    if (menu->background_texture)
        sfTexture_destroy(menu->background_texture);
    if (menu->button_texture)
        sfTexture_destroy(menu->button_texture);
}

static void destroy_extra(menu_t *menu)
{
    if (menu->font)
        sfFont_destroy(menu->font);
    if (menu->action_clock)
        sfClock_destroy(menu->action_clock);
    if (menu->cinematic_clock)
        sfClock_destroy(menu->cinematic_clock);
}

void menu_destroy(menu_t *menu)
{
    if (!menu)
        return;
    destroy_buttons(menu);
    destroy_text(menu->title);
    destroy_text(menu->subtitle);
    destroy_text(menu->settings_text);
    destroy_text(menu->cinematic_title);
    destroy_text(menu->cinematic_text);
    destroy_text(menu->cinematic_hint);
    destroy_shapes(menu);
    destroy_extra(menu);
}
