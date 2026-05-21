/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** life_init
*/

#include "include/mini_map.h"

static int add_callback(wolf_t *wolf,
    size_t (*callback)(setfml_t *setfml, void *userdata),
    char name[BUFF_FUNC_NAME], size_t event)
{
    if (setfml_add(wolf->setfml,
            &(setfml_func_comp_t){NULL, callback},
            name, event) == (size_t)SETFML_FAIL)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

int connect_map_2d(wolf_t *wolf)
{
    if (!wolf || !wolf->setfml)
        return WOLF_FAIL;
    if (add_callback(wolf, &update_map_2d_with_player, "map_update", LOOP_DRAW))
        return WOLF_FAIL;
    if (add_callback(wolf, &draw_2d_map, "draw_map", LOOP_DRAW))
        return WOLF_FAIL;
    return EXIT_SUCCESS;
}
