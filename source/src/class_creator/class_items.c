/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Class creator at initialization.
*/

#include "./../../include/wolf3d.h"
#include "./class_creator.h"

static size_t create_score_item(wolf_t *wolf)
{
    entity_t *score_item = classhandler_entitycreate(wolf->classhandler,
        CLASS_ITEM, ITEM_SCORE);

    if (!score_item)
        return WOLF_FAIL;
    if (item_setdefault(score_item) == WOLF_FAIL)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

size_t create_items(wolf_t *wolf)
{
    class_t *class = classhandler_classcreate(wolf->classhandler,
        sizeof(item_t), CLASS_ITEM);

    if (!class)
        return WOLF_FAIL;
    if (create_score_item(wolf) == WOLF_FAIL)
        return WOLF_FAIL;
    return WOLF_SUCC;
}
