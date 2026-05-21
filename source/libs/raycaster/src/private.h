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

    size_t map_x; // X size of the currently stored map.
    size_t map_y; // Y size of the currently stored map.

    float degree_begin; // The beginning point of the degrees for the raycast.
    float degree_end;   // The end point of the degrees for the raycast.
    float degree_modulo; // The degrees with a modulo 360 applied.
    float degree_step;  // Degrees per pixel column.

    int8_t dx; // Step direction on x axis (-1 or 1).
    int8_t dy; // Step direction on y axis (-1 or 1).

    /*
    Distance along the ray between two consecutive vertical grid lines.
    */
    float delta_dist_x;

    /*
    Distance along the ray between two consecutive horizontal grid lines.
    */
    float delta_dist_y;

    float dist_from_x;  // Distance to the next vertical grid crossing.
    float dist_from_y;  // Distance to the next horizontal grid crossing.

    int map_cur_x; // X index of the cell currently being traversed.
    int map_cur_y; // Y index of the cell currently being traversed.

    float min_dist; // Distance to the wall that was hit.
    bool x;         // If true, the wall was hit on an X side (for shading).
    char hit;       // Map character hit by the raycast.

    float screen_width;  // Width of the render window in pixels.
    float screen_height; // Height of the render window in pixels.

} ray_exec_t;

// Functions
size_t number_in_range_f(float number, float base, float range);
size_t number_in_range_i(int number, int base, int range);
bool raycast_resize_depth_buffer(raycast_t *raycast, size_t width);
void raycast_reset_depth_buffer(raycast_t *raycast);
void raycast_store_depth(raycast_t *raycast, col_data_t *data);
bool raycast_is_occluded(raycast_t *raycast, col_data_t *data);
void ini_dda(raycast_t *raycast, ray_exec_t *data);
bool dda_check_for_collision(ray_exec_t *data);

#endif
