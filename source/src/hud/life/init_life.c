/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** life_init
*/

#include "include/life.h"

static void init_rect(sfIntRect *rect)
{
    rect->height = 268;
    rect->left = 0;
    rect->top = 0;
    rect->width = 120;
}

static void create_spite(life_t *life, int i, int x)
{
    init_rect(&life->life_sprite[i].rect);
    life->life_sprite[i].sprite = sfSprite_create();
    sfSprite_setTexture(life->life_sprite[i].sprite, life->life_sp, sfTrue);
    sfSprite_setTextureRect(life->life_sprite[i].sprite,
        life->life_sprite[i].rect);
    if (i < LIFE_START / DAMMAGE)
        life->life_sprite[i].if_active = 1;
    else
        life->life_sprite[i].if_active = 0;
    sfSprite_setPosition(life->life_sprite[i].sprite, (sfVector2f){x, 50});
    sfSprite_scale(life->life_sprite[i].sprite, (sfVector2f){1, 1});
}

int init_life_sprite(life_t *life, wolf_t *wolf)
{
    int x = 100;
    int i = 0;

    life->life_sp = sfTexture_createFromFile(FONT_LIFE, NULL);
    if (life->life_sp == NULL || wolf == NULL)
        return WOLF_FAIL;
    for (; i < life->nb_draw; i++) {
        create_spite(life, i, x);
        x += 65;
    }
    return EXIT_SUCCESS;
}

int init_life(wolf_t *wolf)
{
    life_t *life = c_alloc(sizeof(life_t), 1, wolf->alloc);

    if (life == NULL)
        return WOLF_FAIL;
    life->dammage = DAMMAGE;
    life->max_life = LIFE_MAX;
    life->current_life = LIFE_START;
    life->nb_draw = NUMBER_SPRITE;
    if (init_life_sprite(life, wolf) != EXIT_SUCCESS)
        return WOLF_FAIL;
    wolf->life = life;
    return EXIT_SUCCESS;
}
