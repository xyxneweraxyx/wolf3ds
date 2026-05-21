/*
** EPITECH PROJECT, 2025
** raycaster
** File description:
** A raycasting lib for wolf3d.
*/

#ifndef RAYCASTER
    #define RAYCASTER

    // Includes default libs
    #include <stdio.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdint.h>
    #include <stdbool.h>
    #include <limits.h>
    #include <float.h>
    #include <math.h>
    #include <SFML/Graphics.h>

    // Includes private libs
    #include "./../../c_alloc/c_alloc.h"
    #include "./../../setfml/setfml.h"

    // Defines
    #define RAYCAST_SUCC 0
    #define RAYCAST_FAIL -1
    #define EMPTY_VERTEX -1

    // Hardcoded values
    #define PI 3.141592
    #define DEG_TO_RAD PI / 180

// Typedefs

typedef struct ray_threed_s {
    float x; // Component X.
    float y; // Component Y.
    float z; // Component Z.
} ray_threed_t;

typedef struct ray_twod_s {
    float x; // Component X.
    float y; // Component Y.
} ray_twod_t;

typedef struct shape_s {

    /*
    The four vertices that compose the shape.
    Stored in a clockwise order, from the top-left corner.
    */
    ray_threed_t vertices[4];

    /*
    The distance from the raycast's origin to the shape's hitpoint.
    Allows for painter's algorithm on the results.
    */
    float dist;

} shape_t;

typedef struct ray_render_s {

    /*
    Height of the render, in pixels.
    Defaults to 0, the middle of the screen will be used.
    */
    int16_t height;

    /*
    Factor over the height of the walls.
    Defaults to 1.
    */
    float wall_height;

    /*
    The degrees visible in the raycast.
    Defaults to 70.
    */
    uint8_t degree;

    /*
    Distance factor applied to the height of the raycast draw.
    Defaults to 0.
    */
    float distance;

} ray_render_t;

typedef struct ray_origin_s {

    /*
    Map used in the raycast. The map should contain the "walls" character.
    */
    char **map;

    /*
    The origin point of the raycast. This point should be expressed in map size.
    For example, {3.5, 4.5} will be considered as an origin in map[4][3].
    */
    ray_twod_t origin;

    /*
    Degree of the direct line of sight of the raycast.
    "degrees" in ray_result_t will base itself on this value.
    */
    float degree;

    /*
    The characters in "map" to be considered as collisions.
    */
    const char *collisions;

} ray_origin_t;

typedef struct ray_calc_s {

    /*
    The maximum distance in cells that a ray from a raycast will traverse.
    If a ray surpasses this distance, it will stop executing and assume void.
    Defaults to 100.
    */
    size_t max_dist;

} ray_calc_t;

typedef struct col_data_s {

    /*
    The setfml environment in which the column is being drawn.
    */
    setfml_t *setfml;

    /*
    The raycaster that was used to render the column.
    */
    struct raycast_s *raycast;

    /*
    The distance from the column to the raycast's origin.
    */
    float distance;

    /*
    The position (between 0 and 1) where the face was hit.
    */
    float face_x;

    /*
    The screen X-position (between 0 and 1) where the face was hit.
    0 is the screen's left, 1 is the screen's right.
    */
    float screen_x;

    /*
    The screen column where the ray hit.
    */
    size_t column;

    /*
    The height of the projected wall column.
    */
    float wall_height;

    /*
    The top-left position where the hit should be drawn.
    */
    sfVector2f position;

    /*
    The map character hit by the ray.
    */
    char hit;

    /*
    The map position hit by the ray.
    */
    int map_x;
    int map_y;

} col_data_t;

typedef void (*raycast_draw_t)(col_data_t *data);

typedef struct raycast_s {

    /*
    The c_alloc object used for allocations during the raycast's existence.
    */
    c_alloc_t *alloc;

    /*
    Rendering options of the raycast.
    */
    ray_render_t render;

    /*
    Origin data of the raycast.
    */
    ray_origin_t origin;

    /*
    Calculation data of the raycast.
    */
    ray_calc_t calculations;

    /*
    Distance for each screen column during the latest render.
    */
    float *depth_buffer;
    size_t depth_width;

    /*
    Optional depth reference used to hide hits behind another raycast.
    */
    struct raycast_s *depth_source;

    /*
    Function called when a ray hits a collision cell.
    */
    raycast_draw_t on_draw;

    int flash_light;

} raycast_t;

// Functions

/*
Creates a raycast_t object filled with default parameters.
The map must be null-terminated.
Returns the object on success, or NULL on fail.
*/
raycast_t *raycast_create(char **map, ray_twod_t *origin);

/*
Destroys a raycast_t object.
*/
void raycast_destroy(raycast_t *raycast);

/*
Performs a raycast based on the parameters of raycast.
On success, stores the results in raycast->result and returns RAYCAST_SUCC.
On fail, returns RAYCAST_FAIL.
*/
size_t raycast_raycast(raycast_t *raycast, setfml_t *setfml);

/*
Checks if a map cell is part of the raycast collision characters.
*/
bool raycast_is_collision(raycast_t *raycast, char cell);

#endif
