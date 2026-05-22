/*
** EPITECH PROJECT, 2025
** raycaster
** File description:
** Raycaster column coverage tracking and per-column occlusion chain.
*/

#include <limits.h>
#include "./../include/raycaster.h"
#include "./private.h"

/*
** Per-column occlusion chain max size.
** Must match the value used in raycaster_raycast.c and raycast_tops.c.
**
** Storage layout (flat arrays indexed as col * COL_CHAIN_MAX + slot):
**   raycast->col_tile_top   : chain entry tops    (uint8_t)
**   raycast->col_chain_bot  : chain entry bottoms (uint8_t)
**   raycast->col_depth2     : chain entry depths  (float)
**   raycast->col_top2       : chain length per column (uint8_t[width])
**
** The chain now stores ALL hits along the ray (in DDA order), not just
** walls with strictly increasing tops. The exact range check in
** is_blocked uses (bot, top, depth) to decide whether each wall actually
** blocks a given (h, dist), which makes floating blocks correct.
*/

static bool ini_extra_ranges(raycast_t *raycast, size_t width)
{
    size_t chain_cells = width * 16;
    uint8_t *tt = c_alloc(sizeof(uint8_t), chain_cells, raycast->alloc);
    float *d2 = c_alloc(sizeof(float), chain_cells, raycast->alloc);
    uint8_t *t2 = c_alloc(sizeof(uint8_t), width, raycast->alloc);
    uint8_t *bb = c_alloc(sizeof(uint8_t), chain_cells, raycast->alloc);

    if (!tt || !d2 || !t2 || !bb)
        return false;
    raycast->col_tile_top = tt;
    raycast->col_depth2 = d2;
    raycast->col_top2 = t2;
    raycast->col_chain_bot = bb;
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
        if (raycast->col_top2)
            raycast->col_top2[i] = 0;
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
