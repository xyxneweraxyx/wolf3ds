/*
** EPITECH PROJECT, 2025
** raycaster
** File description:
** A raycasting lib for wolf3d.
*/

#include "./../include/raycaster.h"

static void init_raycast_defaults(raycast_t *raycast,
    char **map, ray_twod_t *origin)
{
    raycast->origin.degree = 0;
    raycast->origin.collisions = "1";
    raycast->origin.map = map;
    raycast->origin.origin.x = origin->x;
    raycast->origin.origin.y = origin->y;
    raycast->render.degree = 70;
    raycast->render.distance = 1;
    raycast->render.wall_height = 1;
    raycast->calculations.max_dist = 100;
    raycast->depth_buffer = NULL;
    raycast->depth_width = 0;
    raycast->depth_source = NULL;
    raycast->on_draw = NULL;
    raycast->height_bottom = NULL;
    raycast->height_top = NULL;
    raycast->eye_height = (float)RAYCAST_HEIGHT_UNIT / 2.0f;
    raycast->col_y_top = NULL;
    raycast->col_y_bot = NULL;
    raycast->col_range_width = 0;
    raycast->col_tile_top = NULL;
    raycast->col_depth2 = NULL;
    raycast->col_top2 = NULL;
}

raycast_t *raycast_create(char **map, ray_twod_t *origin)
{
    c_alloc_t *alloc = c_ini((uint16_t)10);
    raycast_t *raycast = NULL;

    if (!alloc)
        return NULL;
    raycast = c_alloc(sizeof(raycast_t), 1, alloc);
    if (!raycast)
        return NULL;
    raycast->alloc = alloc;
    init_raycast_defaults(raycast, map, origin);
    return raycast;
}

void raycast_destroy(raycast_t *raycast)
{
    if (!raycast || !raycast->alloc)
        return;
    c_delete(raycast->alloc, true);
}