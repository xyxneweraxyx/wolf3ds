/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Private header between callback connexions.
*/

#include "./../../include/wolf3d.h"
#include "./../hud/shoot/include/shoot.h"

#ifndef CALLBACKS_H
    #define CALLBACKS_H

typedef struct ground_draw_s {
    raycast_t *raycast;
    setfml_t *setfml;
    sfVertexArray *vertices;
    texture_t *texture;
    sfVector2u win_size;
    sfVector2u tex_size;
    sfVector2f direction;
    sfVector2f plane;
    float center_y;
    float camera_height;
    int map_h;
    int map_w;
} ground_draw_t;

typedef struct ground_row_s {
    sfVector2f floor_left;
    sfVector2f floor_right;
    float distance;
    int y;
} ground_row_t;

// Functions
int connect_menu(wolf_t *wolf);
int connect_raycasts(wolf_t *wolf);
int connect_keyboard(wolf_t *wolf);
void draw_wall(col_data_t *data);
void draw_ground(raycast_t *raycast, setfml_t *setfml);
void draw_ceiling(raycast_t *raycast, setfml_t *setfml);
void draw_wall_tops(raycast_t *raycast, setfml_t *setfml);
void set_ground_row(ground_draw_t *draw, ground_row_t *row);
void tops_fill_height(ground_draw_t *draw, uint8_t h);
void draw_object(col_data_t *data);
bool project_object_hit(wolf_t *wolf, object_hit_t *hit,
    struct object_sprite_s *sprite);
void project_object_hits(wolf_t *wolf);
void draw_object_sprites(wolf_t *wolf, setfml_t *setfml);
void shader2(col_data_t *data);
void play_step_sound(wolf_t *wolf);
#endif