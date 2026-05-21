/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Helper functions to destroy objects.
*/

#include "./../include/wolf3d.h"

static void free_map(char **map)
{
    for (int i = 0; map[i]; i++)
        free(map[i]);
    free(map);
}

static void destroy_visuals(wolf_t *wolf)
{
    if (wolf->menu)
        menu_destroy(wolf->menu);
    if (wolf->life)
        destroy_life(wolf->life);
    if (wolf->buttonfml)
        buttonfml_destroy(wolf->buttonfml);
    if (wolf->raycast)
        raycast_destroy(wolf->raycast);
    if (wolf->object_raycast)
        raycast_destroy(wolf->object_raycast);
    if (wolf->setfml)
        setfml_destroy(wolf->setfml);
    if (wolf->step_sound)
        destroy_sound(wolf->step_sound);
    if (wolf->shoot_sound)
        destroy_sound(wolf->shoot_sound);
}

static void destroy_data(wolf_t *wolf)
{
    if (wolf->map)
        free_map(wolf->map);
    if (wolf->object_map)
        free_map(wolf->object_map);
    if (wolf->classhandler)
        classhandler_destroy(wolf->classhandler);
    if (wolf->alloc)
        c_delete(wolf->alloc, true);
}

static void destroy_wolf(wolf_t *wolf)
{
    destroy_visuals(wolf);
    destroy_data(wolf);
}

int destroy_return_int(int ret_code, wolf_t *wolf)
{
    if (!wolf)
        return ret_code;
    destroy_wolf(wolf);
    return ret_code;
}

void *destroy_return_null(wolf_t *wolf)
{
    if (!wolf)
        return NULL;
    destroy_wolf(wolf);
    return NULL;
}
