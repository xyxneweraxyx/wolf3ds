/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu drawing for wolf3d
*/

#include "./../../include/menu.h"

static void draw_buttons(sfRenderWindow *window, menu_t *menu)
{
    for (int i = 0; i < MENU_BUTTON_COUNT; i++) {
        if (!menu->buttons[i].visible)
            continue;
        sfRenderWindow_drawSprite(window, menu->buttons[i].sprite, NULL);
        sfRenderWindow_drawText(window, menu->buttons[i].text, NULL);
    }
}

static void draw_main_menu(sfRenderWindow *window, menu_t *menu)
{
    sfRenderWindow_drawSprite(window, menu->background_sprite, NULL);
    sfRenderWindow_drawText(window, menu->title, NULL);
    sfRenderWindow_drawText(window, menu->subtitle, NULL);
    draw_buttons(window, menu);
}

static void draw_settings(sfRenderWindow *window, menu_t *menu)
{
    sfRectangleShape_setFillColor(menu->overlay, (sfColor){0, 0, 0, 160});
    sfRenderWindow_drawSprite(window, menu->background_sprite, NULL);
    sfRenderWindow_drawRectangleShape(window, menu->overlay, NULL);
    sfRenderWindow_drawText(window, menu->title, NULL);
    sfRenderWindow_drawText(window, menu->settings_text, NULL);
    draw_buttons(window, menu);
}

static void draw_cinematic(sfRenderWindow *window, menu_t *menu)
{
    float seconds = (float)sfClock_getElapsedTime(menu->cinematic_clock)
        .microseconds / 1000000.0f;

    sfRectangleShape_setFillColor(menu->overlay, (sfColor){0, 0, 0, 190});
    sfRenderWindow_drawSprite(window, menu->background_sprite, NULL);
    sfRenderWindow_drawRectangleShape(window, menu->overlay, NULL);
    sfRenderWindow_drawText(window, menu->cinematic_title, NULL);
    sfRenderWindow_drawText(window, menu->cinematic_text, NULL);
    if (((int)(seconds * 2.0f)) % 2 == 0)
        sfRenderWindow_drawText(window, menu->cinematic_hint, NULL);
}

static void draw_pause(sfRenderWindow *window, menu_t *menu)
{
    sfRectangleShape_setFillColor(menu->overlay, (sfColor){0, 0, 0, 165});
    sfRenderWindow_drawRectangleShape(window, menu->overlay, NULL);
    sfRenderWindow_drawText(window, menu->title, NULL);
    draw_buttons(window, menu);
}

size_t menu_on_draw(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;

    (void)userdata;
    if (!wolf || !wolf->menu || !setfml->window)
        return (size_t)SETFML_FAIL;
    if (wolf->state == GAME_MENU)
        draw_main_menu(setfml->window, wolf->menu);
    if (wolf->state == GAME_SETTINGS)
        draw_settings(setfml->window, wolf->menu);
    if (wolf->state == GAME_CINEMATIC)
        draw_cinematic(setfml->window, wolf->menu);
    if (wolf->state == GAME_PAUSED)
        draw_pause(setfml->window, wolf->menu);
    return (size_t)SETFML_SUCC;
}
