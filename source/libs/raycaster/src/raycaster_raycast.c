/*
** EPITECH PROJECT, 2025
** raycaster
** File description:
** A raycasting lib for wolf3d.
*/

#include "./../include/raycaster.h"
#include "./private.h"

static int ini_vals(raycast_t *raycast, ray_exec_t *data,
    sfRenderWindow *window)
{
    sfVector2u win_size = sfRenderWindow_getSize(window);

    if (!raycast_resize_depth_buffer(raycast, (size_t)win_size.x))
        return RAYCAST_FAIL;
    if (!raycast_resize_col_range(raycast, (size_t)win_size.x))
        return RAYCAST_FAIL;
    raycast_reset_depth_buffer(raycast);
    raycast_reset_col_range(raycast);
    data->screen_width = (float)win_size.x;
    data->screen_height = (float)win_size.y;
    data->map_x = str_len(raycast->origin.map[0]);
    for (; raycast->origin.map[data->map_y]; data->map_y++);
    return RAYCAST_SUCC;
}

static void ini_dda(raycast_t *raycast, ray_exec_t *data)
{
    float cos_a = cosf(data->degree_modulo * DEG_TO_RAD);
    float sin_a = sinf(data->degree_modulo * DEG_TO_RAD);
    float frac_x = 0;
    float frac_y = 0;

    data->map_cur_x = (int)raycast->origin.origin.x;
    data->map_cur_y = (int)raycast->origin.origin.y;
    data->dx = (cos_a >= 0) ? 1 : -1;
    data->dy = (sin_a >= 0) ? 1 : -1;
    data->delta_dist_x = (fabsf(cos_a) < 1e-3) ? FLT_MAX : fabsf(1 / cos_a);
    data->delta_dist_y = (fabsf(sin_a) < 1e-3) ? FLT_MAX : fabsf(1 / sin_a);
    frac_x = raycast->origin.origin.x - floorf(raycast->origin.origin.x);
    frac_y = raycast->origin.origin.y - floorf(raycast->origin.origin.y);
    data->dist_from_x = (data->dx > 0) ? (1 - frac_x) * data->delta_dist_x
        : frac_x * data->delta_dist_x;
    data->dist_from_y = (data->dy > 0) ? (1 - frac_y) * data->delta_dist_y
        : frac_y * data->delta_dist_y;
}

static bool dda_check_for_collision(ray_exec_t *data)
{
    if (data->dist_from_x < data->dist_from_y) {
        data->min_dist = data->dist_from_x;
        data->dist_from_x += data->delta_dist_x;
        data->map_cur_x += data->dx;
        data->x = true;
    } else {
        data->min_dist = data->dist_from_y;
        data->dist_from_y += data->delta_dist_y;
        data->map_cur_y += data->dy;
        data->x = false;
    }
    if (data->map_cur_x < 0 || data->map_cur_y < 0 ||
        (size_t)data->map_cur_x >= data->map_x ||
        (size_t)data->map_cur_y >= data->map_y)
        return false;
    return true;
}

static void get_tile_heights(raycast_t *raycast, ray_exec_t *data)
{
    int x = data->map_cur_x;
    int y = data->map_cur_y;

    data->tile_h_bottom = 0;
    data->tile_h_top = (uint8_t)RAYCAST_HEIGHT_UNIT;
    if (!raycast->height_bottom || !raycast->height_top)
        return;
    data->tile_h_bottom = raycast->height_bottom[y][x];
    data->tile_h_top = raycast->height_top[y][x];
}

static void fill_col_screen(col_data_t *col, ray_exec_t *data,
    raycast_t *raycast, float col_x)
{
    float cam_a = raycast->origin.degree * DEG_TO_RAD;
    float fov = raycast->render.degree * DEG_TO_RAD;
    float proj_dist = (data->screen_width / 2.0f) / tanf(fov / 2.0f);
    float perp_dist = 0;
    float ppu = 0;
    float center_y = data->screen_height / 2.0f;

    perp_dist = data->min_dist
        * cosf(data->degree_modulo * DEG_TO_RAD - cam_a);
    ppu = proj_dist / (perp_dist * (float)RAYCAST_HEIGHT_UNIT);
    col->distance = perp_dist;
    col->screen_y_top = center_y
        - ((float)data->tile_h_top - raycast->eye_height) * ppu;
    col->screen_y_bottom = center_y
        - ((float)data->tile_h_bottom - raycast->eye_height) * ppu;
    col->wall_height = col->screen_y_bottom - col->screen_y_top;
    col->position = (sfVector2f){col_x, col->screen_y_top};
}

static col_data_t get_wall_segment(raycast_t *raycast, ray_exec_t *data,
    float col_x)
{
    col_data_t col = {0};
    float cos_a = cosf(data->degree_modulo * DEG_TO_RAD);
    float sin_a = sinf(data->degree_modulo * DEG_TO_RAD);
    float hit_pos = data->x
        ? raycast->origin.origin.y + data->min_dist * sin_a
        : raycast->origin.origin.x + data->min_dist * cos_a;

    fill_col_screen(&col, data, raycast, col_x);
    col.raycast = raycast;
    col.face_x = hit_pos - floorf(hit_pos);
    col.screen_x = col_x / data->screen_width;
    col.column = (size_t)col_x;
    col.tile_bottom = data->tile_h_bottom;
    col.tile_top = data->tile_h_top;
    col.hit = data->hit;
    col.map_x = data->map_cur_x;
    col.map_y = data->map_cur_y;
    return col;
}

static int collect_hits(raycast_t *raycast, ray_exec_t *data,
    col_data_t hits[32], float col_x)
{
    int count = 0;
    char cell = '\0';

    ini_dda(raycast, data);
    for (size_t i = 0; i < raycast->calculations.max_dist; i++) {
        if (!dda_check_for_collision(data))
            break;
        cell = raycast->origin.map[data->map_cur_y][data->map_cur_x];
        if (!raycast_is_collision(raycast, cell))
            continue;
        data->hit = cell;
        get_tile_heights(raycast, data);
        hits[count] = get_wall_segment(raycast, data, col_x);
        if (hits[count].wall_height > 0.0f && count < 31)
            count++;
    }
    return count;
}

static void raycast_column(raycast_t *raycast, ray_exec_t *data,
    float col_x, setfml_t *setfml)
{
    col_data_t hits[32] = {{0}};
    int count = collect_hits(raycast, data, hits, col_x);

    for (int i = 0; i < count; i++)
        hits[i].setfml = setfml;
    if (count > 0)
        raycast_store_depth(raycast, &hits[0]);
    for (int i = count - 1; i >= 0; i--) {
        if (hits[i].wall_height > 0.0f && !raycast_is_occluded(raycast,
                &hits[i]) && raycast->on_draw)
            raycast->on_draw(&hits[i]);
    }
}

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