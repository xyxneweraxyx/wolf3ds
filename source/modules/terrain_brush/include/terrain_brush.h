/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Terrain brush tools
*/

#ifndef TERRAIN_BRUSH_H_
    #define TERRAIN_BRUSH_H_

    #include "terrain_grid.h"

typedef struct terrain_brush_s {
    float center_x;
    float center_y;
    float radius;
    float amount;
} terrain_brush_t;

void terrain_brush_add(terrain_grid_t *grid, const terrain_brush_t *brush);
void terrain_brush_smooth(terrain_grid_t *grid, const terrain_brush_t *brush);

#endif /* !TERRAIN_BRUSH_H_ */
