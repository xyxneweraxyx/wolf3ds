/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Private header between callback connexions.
*/

#define _GNU_SOURCE
#include "include/shoot.h"

static void init_rect(sfIntRect *rect)
{
    rect->height = 229;
    rect->left = 0;
    rect->top = 0;
    rect->width = 200;
}

void init_bullet(bullet_t *new_bullet, player_t *player,
    wolf_t *wolf, float rad)
{
    new_bullet->pos_x = player->pos.x + cosf(rad) * 0.5;
    new_bullet->pos_y = player->pos.z + sinf(rad) * 0.5;
    new_bullet->angle = player->rotation;
    new_bullet->time = 0;
    new_bullet->scale = (sfVector2f){2, 2};
    new_bullet->next = wolf->weapon->bullet;
    new_bullet->speed = 0.1;
}

int create_bullet(wolf_t *wolf, player_t *player)
{
    bullet_t *new_bullet = c_alloc(sizeof(bullet_t), 1, wolf->alloc);
    sfIntRect rect;
    float rad = player->rotation * DEG_TO_RAD;

    init_rect(&rect);
    if (!new_bullet)
        return WOLF_FAIL;
    new_bullet->sprite = sfSprite_create();
    sfSprite_setTexture(new_bullet->sprite,
        wolf->weapon->bullet_texture, sfFalse);
    sfSprite_setTextureRect(new_bullet->sprite, rect);
    sfSprite_setOrigin(new_bullet->sprite,
        (sfVector2f){rect.width / 2, rect.height / 2 + 100});
    init_bullet(new_bullet, player, wolf, rad);
    wolf->weapon->bullet = new_bullet;
    return WOLF_SUCC;
}

static bool can_fire(wolf_t *wolf)
{
    struct timespec now;
    long elapsed_ns;

    clock_gettime(CLOCK_MONOTONIC, &now);
    elapsed_ns = (now.tv_sec - wolf->weapon->start.tv_sec) * SEC_TO_NANO
        + (now.tv_nsec - wolf->weapon->start.tv_nsec);
    if (elapsed_ns < BULLET_FIRE_RATE_NS)
        return false;
    wolf->weapon->start = now;
    return true;
}

size_t shoot(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    entity_t *entity;

    (void)userdata;
    if (wolf->state != GAME_PLAY)
        return 0;
    if (!sfMouse_isButtonPressed(sfMouseLeft))
        return 0;
    if (wolf->weapon->nb_bullet <= 0)
        return 0;
    if (!can_fire(wolf))
        return 0;
    entity = classhandler_fetchentityname(wolf->classhandler,
        CLASS_PLAYERS, NULL);
    wolf->weapon->nb_bullet--;
    create_bullet(wolf, (player_t *)entity->data);
    sfSound_play(wolf->shoot_sound->sound);
    return WOLF_SUCC;
}
