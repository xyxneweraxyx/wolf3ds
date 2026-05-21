/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** life_init
*/

#include "include/life.h"

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

int connect_life(wolf_t *wolf)
{
    if (!wolf || !wolf->setfml)
        return WOLF_FAIL;
    if (add_callback(wolf, &draw_life, "life_draw", LOOP_DRAW))
        return WOLF_FAIL;
    return EXIT_SUCCESS;
}
