/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu system definitions for wolf3d
*/


#include "wolf3d.h"

#ifndef MAP_2D_T_DEFINED
    #define MAP_2D_T_DEFINED

    #define FONT_MAP "./assets/textures/texture_map.png"

struct map_2d_s {
    int len_x;
    int len_y;
    sfVector2f **map_2d;
    sfVector2i pos;
    sfCircleShape *point;
    sfTexture *texture;
    sfConvexShape *shape;
};

size_t draw_2d_map(setfml_t *setfml, void *userdata);
int connect_map_2d(wolf_t *wolf);
sfVector2f **create_2d_map(wolf_t *wolf, map_2d_t *map_2d);
sfVertexArray *create_line(sfVector2f *point1, sfVector2f *point2);
size_t update_map_2d_with_player(setfml_t *setfml, void *userdata);
void free_mini_map(map_2d_t *map_2d);

#endif
