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
    raycast_reset_depth_buffer(raycast);
    data->screen_width = (float)win_size.x;
    data->screen_height = (float)win_size.y;
    data->map_x = str_len(raycast->origin.map[0]);
    for (; raycast->origin.map[data->map_y]; data->map_y++);
    return RAYCAST_SUCC;
}

void ini_dda(raycast_t *raycast, ray_exec_t *data)
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

bool dda_check_for_collision(ray_exec_t *data)
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

static bool single_raycast(raycast_t *raycast, ray_exec_t *data)
{
    char cell = '\0';

    ini_dda(raycast, data);
    for (size_t i = 0; i < raycast->calculations.max_dist; i++) {
        if (!dda_check_for_collision(data))
            return false;
        cell = raycast->origin.map[data->map_cur_y][data->map_cur_x];
        if (raycast_is_collision(raycast, cell)) {
            data->hit = cell;
            return true;
        }
    }
    return false;
}

static float get_wall_height(raycast_t *raycast, ray_exec_t *data,
    float *perp_dist)
{
    float cam_angle = raycast->origin.degree * DEG_TO_RAD;
    float fov = raycast->render.degree * DEG_TO_RAD;
    float proj_dist = (data->screen_width / 2.0f) / tanf(fov / 2.0f);

    *perp_dist = data->min_dist
        * cosf(data->degree_modulo * DEG_TO_RAD - cam_angle);
    return (proj_dist / *perp_dist) * raycast->render.wall_height;
}

static float get_face_x(raycast_t *raycast, ray_exec_t *data,
    float cos_a, float sin_a)
{
    float hit_pos = 0;

    hit_pos = data->x
        ? raycast->origin.origin.y + data->min_dist * sin_a
        : raycast->origin.origin.x + data->min_dist * cos_a;
    return hit_pos - floorf(hit_pos);
}

static col_data_t get_col_data(raycast_t *raycast, ray_exec_t *data,
    float col_x, setfml_t *setfml)
{
    col_data_t col_data = {0};
    float cos_a = cosf(data->degree_modulo * DEG_TO_RAD);
    float sin_a = sinf(data->degree_modulo * DEG_TO_RAD);
    float perp_dist = 0;

    col_data.wall_height = get_wall_height(raycast, data, &perp_dist);
    col_data.setfml = setfml;
    col_data.raycast = raycast;
    col_data.distance = perp_dist;
    col_data.face_x = get_face_x(raycast, data, cos_a, sin_a);
    col_data.screen_x = col_x / data->screen_width;
    col_data.column = (size_t)col_x;
    col_data.position.x = col_x;
    col_data.position.y = (data->screen_height - col_data.wall_height
        + (raycast->render.height / perp_dist)) / 2.0f;
    col_data.hit = data->hit;
    col_data.map_x = data->map_cur_x;
    col_data.map_y = data->map_cur_y;
    return col_data;
}

static void draw_hit(raycast_t *raycast, ray_exec_t *data,
    float col_x, setfml_t *setfml)
{
    col_data_t col_data = get_col_data(raycast, data, col_x, setfml);

    raycast_store_depth(raycast, &col_data);
    if (raycast_is_occluded(raycast, &col_data))
        return;
    if (raycast->on_draw)
        raycast->on_draw(&col_data);
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
        if (single_raycast(raycast, &data))
            draw_hit(raycast, &data, col_x, setfml);
    }
    return RAYCAST_SUCC;
}
