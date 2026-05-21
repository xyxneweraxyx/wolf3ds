/*
** EPITECH PROJECT, 2025
** player
** File description:
** The item class of wolf3d.
*/

#include "./../include/item.h"

size_t item_getmaxamount(entity_t *entity)
{
    item_t *item = NULL;

    if (!entity ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return -1;
    item = (item_t *)entity->data;
    return item->max_amount;
}

float item_getspawndelay(entity_t *entity)
{
    item_t *item = NULL;

    if (!entity ||
        !entity->data ||
        sizeof(entity->class->byte_size) != sizeof(entity->data))
        return -1;
    item = (item_t *)entity->data;
    return item->spawn_delay;
}
