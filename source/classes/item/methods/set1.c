/*
** EPITECH PROJECT, 2025
** player
** File description:
** The item class of wolf3d.
*/

#include "./../include/item.h"

size_t item_setdefault(entity_t *entity)
{
    item_t *item = NULL;

    if (!entity ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return (size_t)-1;
    item = (item_t *)entity->data;
    item->detection_angle = ITEM_DEFAULT_DETECTION_ANGLE;
    item->detection_radius = ITEM_DEFAULT_DETECTION_RADIUS;
    item->max_amount = ITEM_DEFAULT_MAX_ITEM;
    item->spawn_delay = ITEM_DEFAULT_SPAWN_DELAY;
    return (size_t)0;
}

size_t item_setsprite(entity_t *entity, sfSprite *sprite)
{
    item_t *item = NULL;

    if (!entity ||
        !sprite ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return (size_t)-1;
    item = (item_t *)entity->data;
    item->sprite = sprite;
    return (size_t)0;
}

size_t item_settexture(entity_t *entity, sfTexture *texture)
{
    item_t *item = NULL;

    if (!entity ||
        !texture ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return (size_t)-1;
    item = (item_t *)entity->data;
    item->texture = texture;
    return (size_t)0;
}

size_t item_setstate(entity_t *entity, item_state_t state)
{
    item_t *item = NULL;

    if (!entity ||
        state > ITEM_DRAWN ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return (size_t)-1;
    item = (item_t *)entity->data;
    item->state = state;
    return (size_t)0;
}

size_t item_setdetectionradius(entity_t *entity, float radius)
{
    item_t *item = NULL;

    if (!entity ||
        radius < 0 ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return (size_t)-1;
    item = (item_t *)entity->data;
    item->detection_radius = radius;
    return (size_t)0;
}
