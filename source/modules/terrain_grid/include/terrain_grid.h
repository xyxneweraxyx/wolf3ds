/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Terrain height grid container
*/

#ifndef TERRAIN_GRID_H_
    #define TERRAIN_GRID_H_

    #include <stddef.h>

typedef struct terrain_grid_s {
    size_t width;
    size_t height;
    float *values;
} terrain_grid_t;

int terrain_grid_create(terrain_grid_t *grid, size_t width, size_t height);
void terrain_grid_destroy(terrain_grid_t *grid);
void terrain_grid_reset(terrain_grid_t *grid, float value);
float terrain_grid_get(const terrain_grid_t *grid, size_t x, size_t y);
void terrain_grid_set(terrain_grid_t *grid, size_t x, size_t y, float value);

#endif /* !TERRAIN_GRID_H_ */
