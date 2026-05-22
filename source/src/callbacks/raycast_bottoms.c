/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Wall bottom surface rendering (underside of floating blocks).
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

/*
** Linear-interpolate floor_left and floor_right at parameter t into out.
*/
static void compute_world(ground_row_t *row, float t, sfVector2f *out)
{
    out->x = row->floor_left.x +
        t * (row->floor_right.x - row->floor_left.x);
    out->y = row->floor_left.y +
        t * (row->floor_right.y - row->floor_left.y);
}

/*
** True iff cell (mx, my) is a collision tile whose tile_bottom equals h.
*/
static bool bot_tile_matches(ground_draw_t *draw, int mx, int my, uint8_t h)
{
    if (mx < 0 || my < 0 || mx >= draw->map_w || my >= draw->map_h
        || !draw->raycast->height_bottom)
        return false;
    if (!draw->raycast->origin.map[my])
        return false;
    if (!raycast_is_collision(draw->raycast,
            draw->raycast->origin.map[my][mx]))
        return false;
    return draw->raycast->height_bottom[my][mx] == h;
}

/*
** Exact ray_h-in-[bot,top] blocking test for a single chain entry.
*/
static bool entry_blocks(raycast_t *r, size_t idx, float dist, uint8_t h)
{
    float d_w;
    float ray_h;

    d_w = r->col_depth2[idx];
    if (d_w >= dist)
        return false;
    ray_h = r->eye_height - (d_w / dist) * (r->eye_height - (float)h);
    return ray_h >= (float)r->col_chain_bot[idx] &&
        ray_h <= (float)r->col_tile_top[idx];
}

/*
** True iff some wall in the column chain blocks the surface at (col, dist, h).
*/
static bool is_blocked(ground_draw_t *draw, int col, float dist, uint8_t h)
{
    raycast_t *r;
    size_t base;
    uint8_t chain_len;
    uint8_t i;

    r = draw->raycast;
    base = (size_t)col * 16;
    if (!r->col_tile_top || !r->col_depth2 || !r->col_top2
        || !r->col_chain_bot)
        return false;
    if (col < 0 || (size_t)col >= r->col_range_width)
        return false;
    chain_len = r->col_top2[col];
    for (i = 0; i < chain_len; i++) {
        if (r->col_depth2[base + i] >= dist)
            return false;
        if (entry_blocks(r, base + i, dist, h))
            return true;
    }
    return false;
}

/*
** Append a 2-pixel-wide quad to the vertex array for the bottom surface
** visible at column col on row row.
*/
static void bot_append_col(ground_draw_t *draw, ground_row_t *row,
    int col, sfVector2f *world)
{
    int shade;
    sfVertex v = {0};

    shade = 200 - (int)(row->distance * 12.0f);
    if (shade < 35)
        shade = 35;
    if (shade > 200)
        shade = 200;
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

/*
** For each 2-pixel column on row, draw the bottom surface of any wall cell
** whose tile_bottom equals h that isn't occluded by the column's chain.
*/
static void bot_scan_row(ground_draw_t *draw, ground_row_t *row, uint8_t h)
{
    sfVector2f world = {0};
    float win_w;
    float t;
    int mx;
    int my;
    int col;

    win_w = (float)draw->win_size.x;
    for (col = 0; col < (int)draw->win_size.x; col += 2) {
        t = ((float)col + 1.0f) / win_w;
        compute_world(row, t, &world);
        mx = (int)world.x;
        my = (int)world.y;
        if (!bot_tile_matches(draw, mx, my, h))
            continue;
        if (is_blocked(draw, col, row->distance, h))
            continue;
        bot_append_col(draw, row, col, &world);
    }
}

/*
** Render the underside of every wall cell whose tile_bottom equals h.
** Active only when eye_height < h; scans rows above the horizon and
** negates camera_height so set_ground_row produces a positive row.distance
** from the negative (row.y - center_y) above horizon.
*/
void bottoms_fill_height(ground_draw_t *draw, uint8_t h)
{
    ground_row_t row = {0};
    float orig_cam_h;
    float eff;

    orig_cam_h = draw->camera_height;
    eff = (float)h - draw->raycast->eye_height;
    if (eff <= 0.0f || draw->map_h == 0)
        return;
    draw->camera_height = -orig_cam_h * eff / draw->raycast->eye_height;
    for (row.y = 0; row.y < (int)draw->center_y; row.y++) {
        set_ground_row(draw, &row);
        if (row.distance <= 0.0f)
            continue;
        bot_scan_row(draw, &row, h);
    }
    draw->camera_height = orig_cam_h;
}
