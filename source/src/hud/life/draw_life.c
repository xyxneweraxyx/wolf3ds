/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** life_init
*/

#include "include/life.h"

size_t draw_life(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    int i = 0;

    (void)userdata;
    if (wolf->state == GAME_MENU)
        return 0;
    if (wolf->state == GAME_SETTINGS)
        return 0;
    if (wolf->state == GAME_CINEMATIC)
        return 0;
    for (; i < wolf->life->nb_draw; i++) {
        if (wolf->life->life_sprite[i].if_active == 1)
            sfRenderWindow_drawSprite(setfml->window,
                wolf->life->life_sprite[i].sprite, NULL);
    }
    return 0;
}
