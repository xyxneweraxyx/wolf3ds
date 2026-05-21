/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** life_init
*/

#include "include/mini_map.h"

sfVector2f project_iso_point(int x, int y, player_t *player)
{
    sfVector2f d_point;
    float x_radiant = player->rotation * DEG_TO_RAD;
    float y_radiant = player->rotation * DEG_TO_RAD;

    d_point.x = cos(x_radiant) * x - cos(y_radiant) * y;
    d_point.y = sin(y_radiant) * y + sin(x_radiant) * x;
    d_point.x += player->pos.x;
    d_point.y += player->pos.z;
    return d_point;
}

static void get_map_origin(setfml_t *setfml, sfVector2f *origin)
{
    sfVector2u win = sfRenderWindow_getSize(setfml->window);

    origin->x = (float)win.x - 220.0f;
    origin->y = 150.0f;
}

static void update_map_2d(wolf_t *wolf, player_t *player,
    const sfVector2f *origin)
{
    map_2d_t *map_2d = wolf->map_2d;
    float angle = player->rotation * DEG_TO_RAD + 45.5;
    float distance_x = 0;
    float distance_z = 0;

    sfCircleShape_setPosition(map_2d->point,
        (sfVector2f){origin->x - 5.0f, origin->y - 5.0f});
    for (int row = 0; row < map_2d->len_x + 1; row++) {
        for (int column = 0; column <= map_2d->len_y; column++) {
            distance_x = (row - player->pos.x) * 10;
            distance_z = (column - player->pos.z) * 10;
            map_2d->map_2d[row][column].x =
                distance_x * cosf(angle) - distance_z * sinf(angle) +
                origin->x;
            map_2d->map_2d[row][column].y =
                distance_x * sinf(angle) + distance_z * cosf(angle) +
                origin->y;
        }
    }
}

size_t update_map_2d_with_player(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    entity_t *entity = NULL;
    player_t *player = NULL;
    sfVector2f origin = {0, 0};

    (void)userdata;
    if (wolf->state != GAME_PLAY)
        return 0;
    entity = classhandler_fetchentityname(wolf->classhandler,
        CLASS_PLAYERS, NULL);
    if (!entity || !entity->data)
        return 0;
    player = (player_t *)entity->data;
    get_map_origin(setfml, &origin);
    update_map_2d(wolf, player, &origin);
    return 0;
}
