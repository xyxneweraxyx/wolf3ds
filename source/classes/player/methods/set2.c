/*
** EPITECH PROJECT, 2025
** player
** File description:
** The player class of wolf3d.
*/

#include "./../include/player.h"

size_t player_sethealth(entity_t *entity, uint16_t value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    player = (player_t *)entity->data;
    player->health = value;
    return (size_t)0;
}

size_t player_setdmg(entity_t *entity, uint16_t value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    player = (player_t *)entity->data;
    player->dmg = value;
    return (size_t)0;
}

size_t player_setgravity(entity_t *entity, float value)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    player = (player_t *)entity->data;
    player->gravity = value;
    return (size_t)0;
}

size_t player_setstate(entity_t *entity, player_state_t state)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    player = (player_t *)entity->data;
    player->state = state;
    return (size_t)0;
}

size_t player_setpos(entity_t *entity, player_pos_t *pos)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (size_t)-1;
    player = (player_t *)entity->data;
    player->pos.x = pos->x;
    player->pos.y = pos->y;
    player->pos.z = pos->z;
    return (size_t)0;
}
