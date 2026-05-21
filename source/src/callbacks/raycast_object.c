/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Raycast object drawing callback.
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

static bool is_same_object(object_hit_t *hit, col_data_t *data)
{
    return hit->map_x == data->map_x && hit->map_y == data->map_y;
}

static bool has_object_hit(wolf_t *wolf, col_data_t *data)
{
    for (size_t i = 0; i < wolf->object_hit_count; i++) {
        if (is_same_object(&wolf->object_hits[i], data))
            return true;
    }
    return false;
}

static void append_object_hit(wolf_t *wolf, col_data_t *data)
{
    object_hit_t *hit = NULL;

    if (wolf->object_hit_count >= wolf->object_hit_capacity)
        return;
    hit = &wolf->object_hits[wolf->object_hit_count];
    hit->map_x = data->map_x;
    hit->map_y = data->map_y;
    hit->type = data->hit;
    wolf->object_hit_count += 1;
}

void draw_object(col_data_t *data)
{
    wolf_t *wolf = NULL;

    if (!data || !data->setfml)
        return;
    wolf = (wolf_t *)data->setfml->userdata;
    if (!wolf || !wolf->object_hits || has_object_hit(wolf, data))
        return;
    append_object_hit(wolf, data);
}

static void set_camera_direction(raycast_t *raycast, sfVector2f *direction)
{
    float angle = raycast->origin.degree * DEG_TO_RAD;

    direction->x = cosf(angle);
    direction->y = sinf(angle);
}

static void set_camera_plane(raycast_t *raycast,
    sfVector2f *direction, sfVector2f *plane)
{
    float fov = raycast->render.degree * DEG_TO_RAD;
    float plane_size = tanf(fov / 2.0f);

    plane->x = -direction->y * plane_size;
    plane->y = direction->x * plane_size;
}

static bool set_object_transform(raycast_t *raycast,
    struct object_sprite_s *sprite, float *side)
{
    sfVector2f direction = {0};
    sfVector2f plane = {0};
    sfVector2f delta = {sprite->center.x - raycast->origin.origin.x,
        sprite->center.y - raycast->origin.origin.y};
    float det = 0;
    float inv_det = 0;

    set_camera_direction(raycast, &direction);
    set_camera_plane(raycast, &direction, &plane);
    det = plane.x * direction.y - direction.x * plane.y;
    if (fabsf(det) <= 0.0001f)
        return false;
    inv_det = 1.0f / det;
    *side = inv_det * (direction.y * delta.x - direction.x * delta.y);
    sprite->distance = inv_det * (-plane.y * delta.x + plane.x * delta.y);
    return true;
}

static bool is_sprite_near_screen(struct object_sprite_s *sprite,
    sfVector2u *win_size)
{
    if (sprite->distance <= 0.01f)
        return false;
    return sprite->screen_x > -(float)win_size->x &&
        sprite->screen_x < (float)win_size->x * 2.0f;
}

bool project_object_hit(wolf_t *wolf, object_hit_t *hit,
    struct object_sprite_s *sprite)
{
    raycast_t *raycast = NULL;
    sfVector2u win_size = {0};
    float side = 0;

    if (!wolf || !hit || !sprite || !wolf->object_raycast ||
        !wolf->setfml || !wolf->setfml->window)
        return false;
    raycast = wolf->object_raycast;
    win_size = sfRenderWindow_getSize(wolf->setfml->window);
    sprite->center = (sfVector2f){hit->map_x + 0.5f, hit->map_y + 0.5f};
    sprite->type = hit->type;
    if (!set_object_transform(raycast, sprite, &side))
        return false;
    sprite->screen_x = ((float)win_size.x / 2.0f) *
        (1.0f + side / sprite->distance);
    return is_sprite_near_screen(sprite, &win_size);
}
