/*
** EPITECH PROJECT, 2025
** player
** File description:
** The player class of wolf3d.
*/

#include "./../include/player.h"

uint16_t player_gethealth(entity_t *entity)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (uint16_t)-1;
    player = (player_t *)entity->data;
    return player->health;
}

uint16_t player_getdmg(entity_t *entity)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (uint16_t)-1;
    player = (player_t *)entity->data;
    return player->dmg;
}

player_state_t player_getstate(entity_t *entity)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (player_state_t)-1;
    player = (player_t *)entity->data;
    return player->state;
}

player_pos_t player_getpos(entity_t *entity)
{
    player_t *player = NULL;

    if (!entity ||
        !entity->class ||
        entity->class->byte_size != sizeof(player_t))
        return (player_pos_t){-1, -1, -1};
    player = (player_t *)entity->data;
    return (player_pos_t){player->pos.x, player->pos.y, player->pos.z};
}
