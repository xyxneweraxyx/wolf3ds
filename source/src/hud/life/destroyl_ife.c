/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** life_init
*/

#include "include/life.h"

int destroy_life(life_t *life)
{
    for (int i = 0; i < NUMBER_SPRITE; i++) {
        if (life->life_sprite[i].sprite)
            sfSprite_destroy(life->life_sprite[i].sprite);
    }
    if (life->life_sp)
        sfTexture_destroy(life->life_sp);
    return 0;
}
