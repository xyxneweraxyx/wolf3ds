/*
** EPITECH PROJECT, 2025
** player
** File description:
** The item class of wolf3d.
*/

#include "./../include/item.h"

sfSprite *item_getsprite(entity_t *entity)
{
    item_t *item = NULL;

    if (!entity ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return NULL;
    item = (item_t *)entity->data;
    return item->sprite;
}

sfTexture *item_gettexture(entity_t *entity)
{
    item_t *item = NULL;

    if (!entity ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return NULL;
    item = (item_t *)entity->data;
    return item->texture;
}

item_state_t item_getstate(entity_t *entity)
{
    item_t *item = NULL;

    if (!entity ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return -1;
    item = (item_t *)entity->data;
    return item->state;
}

float item_getdetectionradius(entity_t *entity)
{
    item_t *item = NULL;

    if (!entity ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return -1;
    item = (item_t *)entity->data;
    return item->detection_radius;
}

float item_getdetectionangle(entity_t *entity)
{
    item_t *item = NULL;

    if (!entity ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return -1;
    item = (item_t *)entity->data;
    return item->detection_angle;
}
