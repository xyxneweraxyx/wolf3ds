/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Terrain brush tools implementation
*/

#include <math.h>
#include <stdlib.h>
#include "terrain_brush.h"

static float brush_distance(float x1, float y1, float x2, float y2)
{
    float dx;
    float dy;

    dx = x1 - x2;
    dy = y1 - y2;
    return sqrtf(dx * dx + dy * dy);
}

static float compute_factor(const terrain_brush_t *brush, float dist)
{
    float factor;

    factor = dist / (brush->radius + 0.001f);
    return 1.0f - factor;
}

static void apply_add_on_cell(terrain_grid_t *grid,
    const terrain_brush_t *brush, size_t x, size_t y)
{
    float dist;
    float factor;
    float value;

    dist = brush_distance((float)x, (float)y, brush->center_x, brush->center_y);
    if (dist > brush->radius)
        return;
    factor = compute_factor(brush, dist);
    value = terrain_grid_get(grid, x, y) + brush->amount * factor;
    terrain_grid_set(grid, x, y, value);
}

void terrain_brush_add(terrain_grid_t *grid, const terrain_brush_t *brush)
{
    size_t x;
    size_t y;

    if (grid == NULL || brush == NULL || grid->values == NULL)
        return;
    y = 0;
    while (y < grid->height) {
        x = 0;
        while (x < grid->width) {
            apply_add_on_cell(grid, brush, x, y);
            x++;
        }
        y++;
    }
}

static float *buffer_create(const terrain_grid_t *grid)
{
    size_t count;

    count = grid->width * grid->height;
    return malloc(sizeof(float) * count);
}

static void buffer_copy_grid(const terrain_grid_t *grid, float *buffer)
{
    size_t x;
    size_t y;

    y = 0;
    while (y < grid->height) {
        x = 0;
        while (x < grid->width) {
            buffer[y * grid->width + x] = terrain_grid_get(grid, x, y);
            x++;
        }
        y++;
    }
}

static float average_neighbors(const terrain_grid_t *grid, size_t x, size_t y)
{
    size_t min_x;
    size_t max_x;
    size_t min_y;
    size_t max_y;
    size_t count;
    float sum;

    min_x = (x == 0) ? 0 : x - 1;
    max_x = (x + 1 >= grid->width) ? grid->width - 1 : x + 1;
    min_y = (y == 0) ? 0 : y - 1;
    max_y = (y + 1 >= grid->height) ? grid->height - 1 : y + 1;
    count = (max_x - min_x + 1) * (max_y - min_y + 1);
    sum = 0.0f;
    for (size_t iy = min_y; iy <= max_y; iy++) {
        for (size_t ix = min_x; ix <= max_x; ix++)
            sum += terrain_grid_get(grid, ix, iy);
    }
    return sum / (float)count;
}

static void apply_smooth_on_cell(const terrain_grid_t *grid,
    const terrain_brush_t *brush, float *buffer, size_t x)
{
    size_t y;
    float dist;

    y = 0;
    while (y < grid->height) {
        dist = brush_distance((float)x, (float)y, brush->center_x,
            brush->center_y);
        if (dist <= brush->radius)
            buffer[y * grid->width + x] = average_neighbors(grid, x, y);
        y++;
    }
}

static void buffer_apply_grid(terrain_grid_t *grid, const float *buffer)
{
    size_t x;
    size_t y;

    y = 0;
    while (y < grid->height) {
        x = 0;
        while (x < grid->width) {
            terrain_grid_set(grid, x, y, buffer[y * grid->width + x]);
            x++;
        }
        y++;
    }
}

void terrain_brush_smooth(terrain_grid_t *grid, const terrain_brush_t *brush)
{
    float *buffer;
    size_t x;

    if (grid == NULL || brush == NULL || grid->values == NULL)
        return;
    buffer = buffer_create(grid);
    if (buffer == NULL)
        return;
    buffer_copy_grid(grid, buffer);
    x = 0;
    while (x < grid->width) {
        apply_smooth_on_cell(grid, brush, buffer, x);
        x++;
    }
    buffer_apply_grid(grid, buffer);
    free(buffer);
}
