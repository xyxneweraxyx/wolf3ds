/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Callback connexion entry point.
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

int connect_callbacks(wolf_t *wolf)
{
    if (connect_menu(wolf) == WOLF_FAIL ||
        connect_weapon(wolf) == WOLF_FAIL ||
        connect_life(wolf) == WOLF_FAIL ||
        connect_map_2d(wolf) == WOLF_FAIL ||
        connect_raycasts(wolf) == WOLF_FAIL ||
        connect_keyboard(wolf) == WOLF_FAIL)
        return WOLF_FAIL;
    return WOLF_SUCC;
}
