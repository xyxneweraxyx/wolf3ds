/*
** EPITECH PROJECT, 2025
** player
** File description:
** The player class of wolf3d.
*/

#include "./../include/player.h"

size_t player_addhealth(entity_t *entity, uint16_t value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    player = (player_t *)entity->data;
    player->health += value;
    return (size_t)0;
}

size_t player_adddmg(entity_t *entity, uint16_t value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    player = (player_t *)entity->data;
    player->dmg += value;
    return (size_t)0;
}
