/*
** EPITECH PROJECT, 2025
** player
** File description:
** The player class of wolf3d.
*/

#include "./../include/player.h"

size_t player_addrotation(entity_t *entity, float value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    player = (player_t *)entity->data;
    player->rotation += value;
    return (size_t)0;
}

size_t player_addrotspeed(entity_t *entity, float value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    player = (player_t *)entity->data;
    player->rot_speed += value;
    return (size_t)0;
}

size_t player_addspeed(entity_t *entity, float value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    player = (player_t *)entity->data;
    player->move_speed += value;
    return (size_t)0;
}

size_t player_addjumppower(entity_t *entity, float value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    player = (player_t *)entity->data;
    player->jump_power += value;
    return (size_t)0;
}
