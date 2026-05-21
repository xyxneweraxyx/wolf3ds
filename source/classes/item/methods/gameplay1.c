/*
** EPITECH PROJECT, 2025
** item
** File description:
** The item class of wolf3d.
*/

#include <float.h>
#include "./../include/item.h"

static const float ITEM_PI = 3.14159265359f;
static const float ITEM_DEG_TO_RAD = ITEM_PI / 180.0f;

static void copy_position(item_pos_t *position, float pos[3])
{
    pos[0] = position->x;
    pos[1] = position->y;
    pos[2] = position->z;
}

static void get_closest(item_t *item,
    float *closest, size_t *closest_id, float pos[3])
{
    float current = 0;
    float item_pos[3] = {0};

    for (size_t id = 0; id < item->current_amount; id++) {
        copy_position(&item->positions[id], item_pos);
        current = distance_3d(item_pos, pos);
        if (current < *closest) {
            *closest = current;
            *closest_id = id;
        }
    }
}

size_t item_gpclosest(entity_t *entity, float pos[3])
{
    float closest = __FLT_MAX__;
    size_t closest_id = 0;
    item_t *item = NULL;

    if (!entity || !entity->data || !pos)
        return (size_t)-1;
    item = (item_t *)entity->data;
    if (!item->positions || item->current_amount == 0)
        return (size_t)-1;
    get_closest(item, &closest, &closest_id, pos);
    item->closest_index = closest_id;
    return (size_t)0;
}

static bool is_facing(item_t *item, float item_pos[3],
    float pos[3], float angle)
{
    float target = atan2f(item_pos[2] - pos[2], item_pos[0] - pos[0]);
    float delta = target - angle * ITEM_DEG_TO_RAD;

    while (delta > ITEM_PI)
        delta -= 2.0f * ITEM_PI;
    while (delta < -ITEM_PI)
        delta += 2.0f * ITEM_PI;
    return fabsf(delta) <= item->detection_angle * ITEM_DEG_TO_RAD / 2.0f;
}

size_t item_gppickup(entity_t *entity, size_t index,
    float pos[3], float angle)
{
    item_t *item = NULL;
    float item_pos[3] = {0};

    if (!entity || !entity->data)
        return (size_t)0;
    item = (item_t *)entity->data;
    if (!item->positions || index >= item->current_amount)
        return (size_t)-1;
    if (index != item->closest_index)
        return (size_t)-1;
    copy_position(&item->positions[index], item_pos);
    if (distance_3d(item_pos, pos) > item->detection_radius)
        return (size_t)-1;
    if (!is_facing(item, item_pos, pos, angle))
        return (size_t)-1;
    item->positions[index] = item->positions[item->current_amount - 1];
    item->current_amount -= 1;
    return (size_t)0;
}

size_t item_gpspawndelete(entity_t *entity)
{
    item_t *item = NULL;

    if (!entity || !entity->data)
        return (size_t)-1;
    item = (item_t *)entity->data;
    if (item->current_amount > item->max_amount)
        item->current_amount = item->max_amount;
    return (size_t)0;
}
