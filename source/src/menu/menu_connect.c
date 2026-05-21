/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu callback connections for wolf3d
*/

#include "./../../include/menu.h"

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

int connect_menu(wolf_t *wolf)
{
    if (!wolf || !wolf->setfml)
        return WOLF_FAIL;
    if (add_callback(wolf, &menu_on_draw, "menu_draw", LOOP_DRAW))
        return WOLF_FAIL;
    if (add_callback(wolf, &menu_on_render, "menu_render", LOOP_RENDER))
        return WOLF_FAIL;
    if (add_callback(wolf, &menu_on_keypress, "menu_keypress",
            sfEvtKeyPressed))
        return WOLF_FAIL;
    if (add_callback(wolf, &menu_on_mousemove, "menu_mousemove",
            sfEvtMouseMoved))
        return WOLF_FAIL;
    if (add_callback(wolf, &menu_on_mousepress, "menu_mousepress",
            sfEvtMouseButtonPressed))
        return WOLF_FAIL;
    if (add_callback(wolf, &menu_on_mouserelease, "menu_mouserelease",
            sfEvtMouseButtonReleased))
        return WOLF_FAIL;
    return WOLF_SUCC;
}
