/*
** EPITECH PROJECT, 2025
** player
** File description:
** The player class of wolf3d.
*/

#include "./../include/player.h"

size_t player_gpmovex(entity_t *entity, float value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return -1;
    player = (player_t *)entity->data;
    player->pos.x += value;
    return (size_t)0;
}

size_t player_gpmovey(entity_t *entity, float value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return -1;
    player = (player_t *)entity->data;
    player->pos.y += value;
    return (size_t)0;
}

size_t player_gpmovez(entity_t *entity, float value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return -1;
    player = (player_t *)entity->data;
    player->pos.z += value;
    return (size_t)0;
}

size_t player_gpjump(entity_t *entity)
{
    player_t *data = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    data = (player_t *)entity->data;
    data->jmp_strength = data->jump_power;
    data->jmp_gravity = 0;
    return (size_t)0;
}
