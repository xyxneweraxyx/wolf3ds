/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** life_init
*/

#include "include/mini_map.h"

map_2d_t *create_map_2d(wolf_t *wolf)
{
    map_2d_t *map_2d = c_alloc(sizeof(map_2d_t), 1, wolf->alloc);

    map_2d->len_x = str_len(wolf->map[0]);
    map_2d->len_y = 0;
    for (; wolf->map[map_2d->len_y]; map_2d->len_y++);
    map_2d->pos.x = 400;
    map_2d->pos.y = -100;
    return map_2d;
}

static sfVector2f project_iso_point(int x, int y, player_t *player)
{
    sfVector2f d_point;
    float x_radiant = player->rotation * DEG_TO_RAD;
    float y_radiant = player->rotation * DEG_TO_RAD;

    d_point.x = cosf(x_radiant) * x - cos(y_radiant) * y;
    d_point.y = sinf(y_radiant) * y + sin(x_radiant) * x;
    return d_point;
}

sfVector2f **create_2d_map(wolf_t *wolf, map_2d_t *map_2d)
{
    sfVector2f **map = c_alloc(sizeof(sfVector2f *), map_2d->len_x + 1,
        wolf->alloc);
    entity_t *entity = classhandler_fetchentityname(wolf->classhandler,
        CLASS_PLAYERS, NULL);

    for (int row = 0; row <= map_2d->len_x; row++) {
        map[row] = c_alloc(sizeof(sfVector2f *),
            map_2d->len_y + 1, wolf->alloc);
        for (int column = 0; column <= map_2d->len_y; column++) {
            map[row][column] = project_iso_point(row * 10 + map_2d->pos.x,
                column * 10 + map_2d->pos.y, (player_t *)entity->data);
        }
    }
    return map;
}

int init_map_2d(wolf_t *wolf)
{
    map_2d_t *map_2d = create_map_2d(wolf);

    map_2d->map_2d = create_2d_map(wolf, map_2d);
    map_2d->point = sfCircleShape_create();
    sfCircleShape_setRadius(map_2d->point, 5);
    sfCircleShape_setFillColor(map_2d->point, sfRed);
    sfCircleShape_setPosition(map_2d->point, (sfVector2f){1700 - 5, 150 - 5});
    map_2d->texture = sfTexture_createFromFile(FONT_MAP, 0);
    map_2d->shape = sfConvexShape_create();
    sfConvexShape_setTexture(map_2d->shape, map_2d->texture, sfTrue);
    sfConvexShape_setPointCount(map_2d->shape, 4);
    wolf->map_2d = map_2d;
    return WOLF_SUCC;
}

sfVertexArray *create_line(sfVector2f *point1, sfVector2f *point2)
{
    sfVertexArray *vertex_array = sfVertexArray_create();
    sfVertex vertex1 = {.position = *point1, .color = sfBlack};
    sfVertex vertex2 = {.position = *point2, .color = sfBlack};

    sfVertexArray_append(vertex_array, vertex1);
    sfVertexArray_append(vertex_array, vertex2);
    sfVertexArray_setPrimitiveType(vertex_array, sfLinesStrip);
    return vertex_array;
}
