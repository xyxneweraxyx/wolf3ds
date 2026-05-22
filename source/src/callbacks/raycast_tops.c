/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Wall top surface rendering (per-column floor cast).
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

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

static bool tops_col_hidden(ground_draw_t *draw, int col, int row_y)
{
    int *ct = draw->raycast->col_y_top;
    size_t cw = draw->raycast->col_range_width;

    if (!ct || col >= (int)cw || ct[col] == INT_MAX)
        return false;
    return row_y >= ct[col];
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
        if (tops_col_hidden(draw, col, row->y))
            continue;
        t = ((float)col + 1.0f) / win_w;
        world.x = row->floor_left.x + t * (row->floor_right.x - row->floor_left.x);
        world.y = row->floor_left.y + t * (row->floor_right.y - row->floor_left.y);
        mx = (int)world.x;
        my = (int)world.y;
        if (!tops_tile_matches(draw, mx, my, h))
            continue;
        tops_append_col(draw, row, col, &world);
    }
}

void tops_fill_height(ground_draw_t *draw, uint8_t h)
{
    ground_row_t row = {0};
    float orig_cam_h = draw->camera_height;

    if (draw->raycast->eye_height <= (float)h || draw->map_h == 0)
        return;
    draw->camera_height = orig_cam_h
        * (draw->raycast->eye_height - (float)h) / draw->raycast->eye_height;
    for (row.y = (int)draw->center_y; row.y < (int)draw->win_size.y; row.y++) {
        set_ground_row(draw, &row);
        if (row.distance <= 0.0f)
            continue;
        tops_scan_row(draw, &row, h);
    }
    draw->camera_height = orig_cam_h;
}