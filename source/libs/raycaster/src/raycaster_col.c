/*
** EPITECH PROJECT, 2025
** raycaster
** File description:
** Raycaster column coverage tracking.
*/

#include <limits.h>
#include "./../include/raycaster.h"
#include "./private.h"

bool raycast_resize_col_range(raycast_t *raycast, size_t width)
{
    int *new_top = NULL;
    int *new_bot = NULL;

    if (raycast->col_y_top && raycast->col_range_width == width)
        return true;
    new_top = c_alloc(sizeof(int), width, raycast->alloc);
    new_bot = c_alloc(sizeof(int), width, raycast->alloc);
    if (!new_top || !new_bot)
        return false;
    raycast->col_y_top = new_top;
    raycast->col_y_bot = new_bot;
    raycast->col_range_width = width;
    return true;
}

void raycast_reset_col_range(raycast_t *raycast)
{
    if (!raycast->col_y_top || !raycast->col_y_bot)
        return;
    for (size_t i = 0; i < raycast->col_range_width; i++) {
        raycast->col_y_top[i] = INT_MAX;
        raycast->col_y_bot[i] = INT_MIN;
    }
}

bool raycast_col_is_done(raycast_t *raycast, size_t col, int screen_h)
{
    if (!raycast->col_y_top || col >= raycast->col_range_width)
        return false;
    return raycast->col_y_top[col] <= 0 &&
        raycast->col_y_bot[col] >= screen_h - 1;
}

void raycast_col_mark(raycast_t *raycast, size_t col, int top, int bot)
{
    if (!raycast->col_y_top || col >= raycast->col_range_width)
        return;
    raycast->col_y_top[col] = top;
    raycast->col_y_bot[col] = bot;
}

bool raycast_col_is_occluded(raycast_t *raycast, size_t col, int top, int bot)
{
    if (!raycast->col_y_top || col >= raycast->col_range_width)
        return false;
    if (raycast->col_y_top[col] == INT_MAX)
        return false;
    return top >= raycast->col_y_top[col] && bot <= raycast->col_y_bot[col];
}