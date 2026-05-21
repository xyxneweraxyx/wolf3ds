/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Class creator at initialization.
*/

#include "./../../include/wolf3d.h"
#include "./class_creator.h"

int create_classes(wolf_t *wolf)
{
    classhandler_t *classhandler = classhandler_init();

    if (!classhandler)
        return WOLF_FAIL;
    wolf->classhandler = classhandler;
    if (!classhandler ||
        create_players(wolf) == WOLF_FAIL ||
        create_items(wolf) == WOLF_FAIL)
        return WOLF_FAIL;
    return WOLF_SUCC;
}
