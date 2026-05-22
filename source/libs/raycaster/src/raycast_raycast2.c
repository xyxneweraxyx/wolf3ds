/*
** EPITECH PROJECT, 2025
** raycaster
** File description:
** A raycasting lib for wolf3d.
*/

#include "./../include/raycaster.h"
#include "./private.h"

bool raycast_is_collision(raycast_t *raycast, char cell)
{
    const char *collisions = NULL;

    if (!raycast || !raycast->origin.collisions)
        return false;
    collisions = raycast->origin.collisions;
    for (int i = 0; collisions[i]; i++) {
        if (collisions[i] == cell)
            return true;
    }
    return false;
}

size_t raycast_raycast(raycast_t *raycast, setfml_t *setfml)
{
    ray_exec_t data = {0};
    float col_x = 0;
    float degree = 0;

    if (!raycast || !raycast->origin.map ||
        !raycast->origin.map[0] || !setfml->window)
        return RAYCAST_FAIL;
    if (ini_vals(raycast, &data, setfml->window) == RAYCAST_FAIL)
        return RAYCAST_FAIL;
    for (; col_x < data.screen_width; col_x += 1) {
        degree = raycast->origin.degree + atanf(
            (2.0f * (col_x + 0.5f) / data.screen_width - 1.0f) *
            tanf(raycast->render.degree * DEG_TO_RAD / 2.0f))
            * 180.0f / PI;
        data.degree_modulo = (((int)degree + 360) % 360)
            + (degree - (int)degree);
        raycast_column(raycast, &data, col_x, setfml);
    }
    return RAYCAST_SUCC;
}
