/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** life_init
*/

#include "include/life.h"

static int get_frame(int life_sprite)
{
    if (life_sprite >= 20)
        return 0;
    if (life_sprite >= 15)
        return 1;
    if (life_sprite >= 10)
        return 2;
    return 3;
}

static void update_rect(sfIntRect *rect, int frame)
{
    rect->top = 0;
    rect->height = 268;
    rect->width = 122;
    rect->left = frame * 122;
}

static int get_life(int current_life, int i)
{
    int life_restant = current_life - (i * DAMMAGE);

    if (life_restant <= 0)
        return 0;
    if (life_restant > DAMMAGE)
        return DAMMAGE;
    return life_restant;
}

static void update_sprite(life_sprite_t *sprite, int life_sprite)
{
    sprite->if_active = 1;
    update_rect(&sprite->rect, get_frame(life_sprite));
    sfSprite_setTextureRect(sprite->sprite, sprite->rect);
}

static void update_life_sprite(wolf_t *wolf)
{
    int life = 0;

    for (int i = 0; i < wolf->life->nb_draw; i++) {
        life = get_life(wolf->life->current_life, i);
        if (life == 0)
            wolf->life->life_sprite[i].if_active = 0;
        else
            update_sprite(&wolf->life->life_sprite[i], life);
    }
}

int add_life(wolf_t *wolf, int dammage)
{
    if (wolf->life->current_life >= wolf->life->max_life)
        return EXIT_SUCCESS;
    wolf->life->current_life += dammage;
    update_life_sprite(wolf);
    return EXIT_SUCCESS;
}

int del_life(wolf_t *wolf, int dammage)
{
    if (wolf->life->current_life <= 0)
        return LOSE;
    wolf->life->current_life -= dammage;
    if (wolf->life->current_life <= 0)
        return LOSE;
    update_life_sprite(wolf);
    return EXIT_SUCCESS;
}

int update_life(int action, wolf_t *wolf, int dammage)
{
    if (action == 1)
        return add_life(wolf, dammage);
    else
        return del_life(wolf, dammage);
    return 0;
}
