/*
** EPITECH PROJECT, 2025
** raycaster
** File description:
** A raycasting lib for wolf3d.
*/

#ifndef RAYCAST_PRIVATE
    #define RAYCAST_PRIVATE

    #include "./../include/raycaster.h"

typedef struct ray_exec_s {

    size_t map_x;
    size_t map_y;

    float degree_begin;
    float degree_end;
    float degree_modulo;
    float degree_step;

    int8_t dx;
    int8_t dy;

    float delta_dist_x;
    float delta_dist_y;

    float dist_from_x;
    float dist_from_y;

    int map_cur_x;
    int map_cur_y;

    float min_dist;
    bool x;
    char hit;

    float screen_width;
    float screen_height;

    uint8_t tile_h_bottom;
    uint8_t tile_h_top;

} ray_exec_t;

// Depth buffer
bool raycast_resize_depth_buffer(raycast_t *raycast, size_t width);
void raycast_reset_depth_buffer(raycast_t *raycast);
void raycast_store_depth(raycast_t *raycast, col_data_t *data);
bool raycast_is_occluded(raycast_t *raycast, col_data_t *data);

// Column coverage tracking
bool raycast_resize_col_range(raycast_t *raycast, size_t width);
void raycast_reset_col_range(raycast_t *raycast);
bool raycast_col_is_done(raycast_t *raycast, size_t col, int screen_h);
void raycast_col_mark(raycast_t *raycast, size_t col, int top, int bot);
bool raycast_col_is_occluded(raycast_t *raycast, size_t col, int top, int bot);

// Math
size_t number_in_range_f(float number, float base, float range);
size_t number_in_range_i(int number, int base, int range);

#endif