/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** life_init
*/

#include "include/mini_map.h"

void free_mini_map(map_2d_t *map_2d)
{
    if (map_2d == NULL)
        return;
    if (map_2d->map_2d == NULL)
        return;
    for (int row = 0; row <= map_2d->len_x; row++) {
        if (map_2d->map_2d[row] != NULL)
            free(map_2d->map_2d[row]);
    }
    free(map_2d->map_2d);
    sfCircleShape_destroy(map_2d->point);
    free(map_2d);
}
