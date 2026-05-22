/*
** EPITECH PROJECT, 2025
** raycaster
** File description:
** Raycaster column coverage tracking.
*/

#include <limits.h>
#include "./../include/raycaster.h"
#include "./private.h"

static bool ini_extra_ranges(raycast_t *raycast, size_t width)
{
    uint8_t *tt = c_alloc(sizeof(uint8_t), width, raycast->alloc);
    float *d2 = c_alloc(sizeof(float), width, raycast->alloc);
    uint8_t *t2 = c_alloc(sizeof(uint8_t), width, raycast->alloc);

    if (!tt || !d2 || !t2)
        return false;
    raycast->col_tile_top = tt;
    raycast->col_depth2 = d2;
    raycast->col_top2 = t2;
    return true;
}

bool raycast_resize_col_range(raycast_t *raycast, size_t width)
{
    int *nt = NULL;
    int *nb = NULL;

    if (raycast->col_y_top && raycast->col_range_width == width)
        return true;
    nt = c_alloc(sizeof(int), width, raycast->alloc);
    nb = c_alloc(sizeof(int), width, raycast->alloc);
    if (!nt || !nb || !ini_extra_ranges(raycast, width))
        return false;
    raycast->col_y_top = nt;
    raycast->col_y_bot = nb;
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
        if (raycast->col_tile_top) raycast->col_tile_top[i] = 0;
        if (raycast->col_depth2) raycast->col_depth2[i] = FLT_MAX;
        if (raycast->col_top2) raycast->col_top2[i] = 0;
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