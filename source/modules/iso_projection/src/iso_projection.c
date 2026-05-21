/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Isometric projection helpers implementation
*/

#include <math.h>
#include "iso_projection.h"

static void rotate_on_ground(const sfVector3f *point,
    const camera_iso_t *camera, float *rotated_x, float *rotated_y)
{
    float yaw_cos;
    float yaw_sin;

    yaw_cos = cosf(camera->angle_x);
    yaw_sin = sinf(camera->angle_x);
    *rotated_x = point->x * yaw_cos - point->y * yaw_sin;
    *rotated_y = point->x * yaw_sin + point->y * yaw_cos;
}

sfVector2f iso_project_point(const sfVector3f *point,
    const camera_iso_t *camera)
{
    sfVector2f out;
    float rotated_x;
    float rotated_y;

    if (point == NULL || camera == NULL)
        return (sfVector2f){0.0f, 0.0f};
    rotate_on_ground(point, camera, &rotated_x, &rotated_y);
    out.x = (rotated_x - rotated_y) * 0.707106781f;
    out.y = (rotated_x + rotated_y) * sinf(camera->angle_y) - point->z;
    out.x = out.x * camera->zoom + camera->offset.x;
    out.y = out.y * camera->zoom + camera->offset.y;
    return out;
}
