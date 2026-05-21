/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Terrain height grid container implementation
*/

#include <stdlib.h>
#include "terrain_grid.h"

static size_t grid_index(const terrain_grid_t *grid, size_t x, size_t y)
{
    return y * grid->width + x;
}

int terrain_grid_create(terrain_grid_t *grid, size_t width, size_t height)
{
    size_t count;

    if (grid == NULL || width < 2 || height < 2)
        return 84;
    count = width * height;
    grid->values = malloc(sizeof(float) * count);
    if (grid->values == NULL)
        return 84;
    grid->width = width;
    grid->height = height;
    terrain_grid_reset(grid, 0.0f);
    return 0;
}

void terrain_grid_destroy(terrain_grid_t *grid)
{
    if (grid == NULL)
        return;
    free(grid->values);
    grid->values = NULL;
    grid->width = 0;
    grid->height = 0;
}

void terrain_grid_reset(terrain_grid_t *grid, float value)
{
    size_t i;
    size_t count;

    if (grid == NULL || grid->values == NULL)
        return;
    count = grid->width * grid->height;
    i = 0;
    while (i < count) {
        grid->values[i] = value;
        i++;
    }
}

float terrain_grid_get(const terrain_grid_t *grid, size_t x, size_t y)
{
    if (grid == NULL || grid->values == NULL)
        return 0.0f;
    if (x >= grid->width || y >= grid->height)
        return 0.0f;
    return grid->values[grid_index(grid, x, y)];
}

void terrain_grid_set(terrain_grid_t *grid, size_t x, size_t y, float value)
{
    if (grid == NULL || grid->values == NULL)
        return;
    if (x >= grid->width || y >= grid->height)
        return;
    grid->values[grid_index(grid, x, y)] = value;
}
