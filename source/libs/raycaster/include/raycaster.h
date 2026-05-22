/*
** EPITECH PROJECT, 2025
** raycaster
** File description:
** A raycasting lib for wolf3d.
*/

#ifndef RAYCASTER
    #define RAYCASTER

    #include <stdio.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdint.h>
    #include <stdbool.h>
    #include <limits.h>
    #include <float.h>
    #include <math.h>
    #include <SFML/Graphics.h>

    #include "./../../c_alloc/c_alloc.h"
    #include "./../../setfml/setfml.h"

    #define RAYCAST_SUCC 0
    #define RAYCAST_FAIL -1
    #define EMPTY_VERTEX -1
    #define RAYCAST_HEIGHT_UNIT 16
    #define PI 3.141592
    #define DEG_TO_RAD PI / 180

typedef struct ray_threed_s { float x; float y; float z; } ray_threed_t;
typedef struct ray_twod_s { float x; float y; } ray_twod_t;
typedef struct shape_s { ray_threed_t vertices[4]; float dist; } shape_t;
typedef struct ray_render_s {
    int16_t height; float wall_height; uint8_t degree; float distance;
} ray_render_t;
typedef struct ray_origin_s {
    char **map; ray_twod_t origin; float degree; const char *collisions;
} ray_origin_t;
typedef struct ray_calc_s { size_t max_dist; } ray_calc_t;

typedef struct col_data_s {
    setfml_t *setfml;
    struct raycast_s *raycast;
    float distance;
    float face_x;
    float screen_x;
    size_t column;
    float wall_height;
    sfVector2f position;
    char hit;
    int map_x;
    int map_y;
    uint8_t tile_bottom;
    uint8_t tile_top;
    float screen_y_top;
    float screen_y_bottom;
} col_data_t;

typedef void (*raycast_draw_t)(col_data_t *data);

typedef struct raycast_s {
    c_alloc_t *alloc;
    ray_render_t render;
    ray_origin_t origin;
    ray_calc_t calculations;
    float *depth_buffer;
    size_t depth_width;
    struct raycast_s *depth_source;
    raycast_draw_t on_draw;
    int flash_light;
    uint8_t **height_bottom;
    uint8_t **height_top;
    float eye_height;
    int *col_y_top;
    int *col_y_bot;
    size_t col_range_width;
    uint8_t *col_tile_top;
    float *col_depth2;
    uint8_t *col_top2;
    uint8_t *col_chain_bot;
} raycast_t;

raycast_t *raycast_create(char **map, ray_twod_t *origin);
void raycast_destroy(raycast_t *raycast);
size_t raycast_raycast(raycast_t *raycast, setfml_t *setfml);
bool raycast_is_collision(raycast_t *raycast, char cell);
void raycast_col_mark(raycast_t *raycast, size_t col, int top, int bot);

#endif
