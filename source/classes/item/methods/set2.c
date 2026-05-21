/*
** EPITECH PROJECT, 2025
** player
** File description:
** The item class of wolf3d.
*/

#include "./../include/item.h"

size_t item_setdetectionangle(entity_t *entity, float angle)
{
    item_t *item = NULL;

    if (!entity ||
        angle < 0 ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return (size_t)-1;
    item = (item_t *)entity->data;
    item->detection_angle = angle;
    return (size_t)0;
}

size_t item_setmaxamount(entity_t *entity, int amount)
{
    item_t *item = NULL;

    if (!entity ||
        amount < 0 ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return (size_t)-1;
    item = (item_t *)entity->data;
    item->max_amount = amount;
    return (size_t)0;
}

size_t item_setspawndelay(entity_t *entity, float delay)
{
    item_t *item = NULL;

    if (!entity ||
        delay < 0 ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return (size_t)-1;
    item = (item_t *)entity->data;
    item->spawn_delay = delay;
    return (size_t)0;
}
