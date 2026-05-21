/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Tile picking helpers
*/

#ifndef TILE_PICKER_H_
    #define TILE_PICKER_H_

    #include <SFML/Graphics.h>
    #include "camera_iso.h"
    #include "terrain_grid.h"

typedef struct tile_picker_request_s {
    int mouse_x;
    int mouse_y;
    float tile_size;
} tile_picker_request_t;

typedef struct tile_picker_result_s {
    size_t x;
    size_t y;
} tile_picker_result_t;

typedef struct tile_picker_state_s {
    const terrain_grid_t *grid;
    const camera_iso_t *camera;
    const tile_picker_request_t *request;
    float best;
    size_t best_x;
    size_t best_y;
} tile_picker_state_t;

sfBool tile_picker_pick_cell(const terrain_grid_t *grid,
    const camera_iso_t *camera,
    const tile_picker_request_t *request,
    tile_picker_result_t *result);

#endif /* !TILE_PICKER_H_ */
