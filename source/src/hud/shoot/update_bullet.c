/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Private header between callback connexions.
*/

#include "include/shoot.h"

int update_bullet(bullet_t *bullet)
{
    if (!bullet->sprite)
        return 0;
    bullet->pos_x += cosf(bullet->angle * DEG_TO_RAD) * bullet->speed;
    bullet->pos_y += sinf(bullet->angle * DEG_TO_RAD) * bullet->speed;
    return 0;
}

static bool hits_wall(wolf_t *wolf, bullet_t *bullet)
{
    int x = bullet->pos_x;
    int y = bullet->pos_y;

    if (x < 0 || y < 0)
        return true;
    if (!wolf->map[y] || !wolf->map[y][x])
        return true;
    if (wolf->map[y][x] == '0')
        return false;
    return true;
}

static bullet_t *remove_bullet(wolf_t *wolf, bullet_t *prev, bullet_t *bullet)
{
    bullet_t *next = bullet->next;

    if (prev == NULL)
        wolf->weapon->bullet = next;
    else
        prev->next = next;
    sfSprite_destroy(bullet->sprite);
    return next;
}

size_t update_bullets(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    bullet_t *tmp = wolf->weapon->bullet;
    bullet_t *prev = NULL;
    bullet_t *next;

    (void)userdata;
    if (wolf->state != GAME_PLAY)
        return 0;
    while (tmp != NULL) {
        next = tmp->next;
        update_bullet(tmp);
        if (hits_wall(wolf, tmp) == true)
            tmp = remove_bullet(wolf, prev, tmp);
        else {
            prev = tmp;
            tmp = next;
        }
    }
    return 0;
}
