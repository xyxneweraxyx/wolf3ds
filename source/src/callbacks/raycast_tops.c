/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Wall top surface rendering (per-column floor cast).
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

/*
** Per-column occlusion chain max size.
** Must match the value used in raycaster_raycast.c and raycaster_col.c.
*/
#define COL_CHAIN_MAX 16

static bool tops_tile_matches(ground_draw_t *draw, int mx, int my, uint8_t h)
{
    if (mx < 0 || my < 0 || mx >= draw->map_w || my >= draw->map_h
        || !draw->raycast->height_top)
        return false;
    if (!draw->raycast->origin.map[my])
        return false;
    if (!raycast_is_collision(draw->raycast,
            draw->raycast->origin.map[my][mx]))
        return false;
    return draw->raycast->height_top[my][mx] == h;
}

/*
** Exact per-entry blocking test.
** A wall at distance d_w with vertical extent [bot, top] blocks a ray
** going from eye_h to a surface at (dist, h) iff the ray's height at
** d_w lies inside [bot, top]. We use ray_h = eye - (d_w/dist)*(eye - h),
** which works for both descending (eye > h) and ascending rays.
*/
static bool entry_blocks(raycast_t *r, size_t idx, float dist, uint8_t h)
{
    float d_w = r->col_depth2[idx];
    float ray_h;

    if (d_w >= dist)
        return false;
    ray_h = r->eye_height - (d_w / dist) * (r->eye_height - (float)h);
    return ray_h >= (float)r->col_chain_bot[idx] &&
        ray_h <= (float)r->col_tile_top[idx];
}

/*
** Per-column occlusion check.
** Walks the chain stored by raycaster_raycast in increasing-depth order
** and returns true as soon as some entry actually blocks the ray. The
** early-exit on d_w >= dist makes the typical case fast: only the few
** entries closer than the surface get tested.
*/
static bool is_blocked(ground_draw_t *draw, int col, float dist, uint8_t h)
{
    raycast_t *r = draw->raycast;
    size_t base = (size_t)col * COL_CHAIN_MAX;
    uint8_t chain_len = 0;

    if (!r->col_tile_top || !r->col_depth2 || !r->col_top2 ||
        !r->col_chain_bot)
        return false;
    if (col < 0 || (size_t)col >= r->col_range_width)
        return false;
    chain_len = r->col_top2[col];
    for (uint8_t i = 0; i < chain_len; i++) {
        if (r->col_depth2[base + i] >= dist)
            return false;
        if (entry_blocks(r, base + i, dist, h))
            return true;
    }
    return false;
}

static void tops_append_col(ground_draw_t *draw, ground_row_t *row,
    int col, sfVector2f *world)
{
    int shade = 255 - (int)(row->distance * 12.0f);
    sfVertex v = {0};

    if (shade < 45) shade = 45;
    if (shade > 255) shade = 255;
    v.color = (sfColor){shade, shade, shade, 255};
    v.texCoords = (sfVector2f){world->x * (float)draw->tex_size.x,
        world->y * (float)draw->tex_size.y};
    v.position = (sfVector2f){(float)col, (float)row->y};
    sfVertexArray_append(draw->vertices, v);
    v.position = (sfVector2f){(float)(col + 2), (float)row->y};
    sfVertexArray_append(draw->vertices, v);
    v.position = (sfVector2f){(float)(col + 2), (float)(row->y + 1)};
    sfVertexArray_append(draw->vertices, v);
    v.position = (sfVector2f){(float)col, (float)(row->y + 1)};
    sfVertexArray_append(draw->vertices, v);
}

static void tops_scan_row(ground_draw_t *draw, ground_row_t *row, uint8_t h)
{
    sfVector2f world = {0};
    float win_w = (float)draw->win_size.x;
    float t = 0.0f;
    int mx = 0;
    int my = 0;

    for (int col = 0; col < (int)draw->win_size.x; col += 2) {
        t = ((float)col + 1.0f) / win_w;
        world.x = row->floor_left.x + t * (row->floor_right.x - row->floor_left.x);
        world.y = row->floor_left.y + t * (row->floor_right.y - row->floor_left.y);
        mx = (int)world.x;
        my = (int)world.y;
        if (!tops_tile_matches(draw, mx, my, h))
            continue;
        if (is_blocked(draw, col, row->distance, h))
            continue;
        tops_append_col(draw, row, col, &world);
    }
}

void tops_fill_height(ground_draw_t *draw, uint8_t h)
{
    ground_row_t row = {0};
    float orig_cam_h = draw->camera_height;
    float eff = draw->raycast->eye_height - (float)h;

    if (eff <= 0.0f || draw->map_h == 0)
        return;
    draw->camera_height = orig_cam_h * eff / draw->raycast->eye_height;
    for (row.y = (int)draw->center_y; row.y < (int)draw->win_size.y; row.y++) {
        set_ground_row(draw, &row);
        if (row.distance <= 0.0f)
            continue;
        tops_scan_row(draw, &row, h);
    }
    draw->camera_height = orig_cam_h;
}