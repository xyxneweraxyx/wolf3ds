/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Isometric camera model implementation
*/

#include "camera_iso.h"

static const float MIN_ANGLE_Y = 0.20f;
static const float MAX_ANGLE_Y = 1.35f;

static float wrap_angle(float value)
{
    const float pi = 3.1415926535f;
    const float full = 6.283185307f;

    while (value > pi)
        value -= full;
    while (value < -pi)
        value += full;
    return value;
}

void camera_iso_init(camera_iso_t *camera,
    const camera_iso_options_t *options)
{
    if (camera == NULL || options == NULL)
        return;
    camera->offset.x = options->offset_x;
    camera->offset.y = options->offset_y;
    camera->zoom = options->zoom;
    camera->angle_x = options->angle_x;
    camera->angle_y = options->angle_y;
}

void camera_iso_pan(camera_iso_t *camera, float dx, float dy)
{
    if (camera == NULL)
        return;
    camera->offset.x += dx;
    camera->offset.y += dy;
}

void camera_iso_add_zoom(camera_iso_t *camera, float delta,
    float min_zoom, float max_zoom)
{
    if (camera == NULL)
        return;
    camera->zoom += delta;
    if (camera->zoom < min_zoom)
        camera->zoom = min_zoom;
    if (camera->zoom > max_zoom)
        camera->zoom = max_zoom;
}

void camera_iso_add_rotation(camera_iso_t *camera, float delta_x,
    float delta_y)
{
    if (camera == NULL)
        return;
    camera->angle_x = wrap_angle(camera->angle_x + delta_x);
    camera->angle_y += delta_y;
    if (camera->angle_y < MIN_ANGLE_Y)
        camera->angle_y = MIN_ANGLE_Y;
    if (camera->angle_y > MAX_ANGLE_Y)
        camera->angle_y = MAX_ANGLE_Y;
}
