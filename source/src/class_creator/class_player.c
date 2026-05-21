/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Class creator at initialization.
*/

#include "./../../include/wolf3d.h"
#include "./class_creator.h"

size_t create_players(wolf_t *wolf)
{
    class_t *class = classhandler_classcreate(wolf->classhandler,
        sizeof(player_t), CLASS_PLAYERS);
    entity_t *player = NULL;

    if (!class)
        return WOLF_FAIL;
    player = classhandler_entitycreate(wolf->classhandler,
        CLASS_PLAYERS, PARAMS_DEFAULT_NAME);
    if (!player)
        return WOLF_FAIL;
    if (player_setdefault(player))
        return WOLF_FAIL;
    return WOLF_SUCC;
}
