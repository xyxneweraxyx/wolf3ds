/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Tile picking helpers implementation
*/

#include "tile_picker.h"
#include "iso_projection.h"

static float squared_distance(float x1, float y1, float x2, float y2)
{
    float dx;
    float dy;

    dx = x1 - x2;
    dy = y1 - y2;
    return dx * dx + dy * dy;
}

static float get_threshold(const camera_iso_t *camera)
{
    float threshold;

    threshold = 28.0f * camera->zoom;
    if (threshold < 12.0f)
        threshold = 12.0f;
    return threshold;
}

static float get_center_height(const terrain_grid_t *grid, size_t x, size_t y)
{
    float h00;
    float h10;
    float h01;
    float h11;

    h00 = terrain_grid_get(grid, x, y);
    h10 = terrain_grid_get(grid, x + 1, y);
    h01 = terrain_grid_get(grid, x, y + 1);
    h11 = terrain_grid_get(grid, x + 1, y + 1);
    return (h00 + h10 + h01 + h11) / 4.0f;
}

static sfVector2f get_grid_center(const tile_picker_state_t *state)
{
    sfVector2f center;

    center.x = ((float)state->grid->width - 1.0f) * state->request->tile_size;
    center.y = ((float)state->grid->height - 1.0f) * state->request->tile_size;
    center.x *= 0.5f;
    center.y *= 0.5f;
    return center;
}

static sfVector3f get_cell_center(size_t x, size_t y,
    const tile_picker_state_t *state, const sfVector2f *center)
{
    sfVector3f world_center;

    world_center.x = ((float)x + 0.5f) * state->request->tile_size - center->x;
    world_center.y = ((float)y + 0.5f) * state->request->tile_size - center->y;
    world_center.z = get_center_height(state->grid, x, y);
    return world_center;
}

static void test_cell(size_t x, size_t y, tile_picker_state_t *state)
{
    sfVector3f center;
    sfVector2f projected;
    sfVector2f grid_center;
    float dist;

    grid_center = get_grid_center(state);
    center = get_cell_center(x, y, state, &grid_center);
    projected = iso_project_point(&center, state->camera);
    dist = squared_distance(projected.x, projected.y,
        (float)state->request->mouse_x,
        (float)state->request->mouse_y);
    if (dist < state->best) {
        state->best = dist;
        state->best_x = x;
        state->best_y = y;
    }
}

static void scan_cells(tile_picker_state_t *state)
{
    size_t x;
    size_t y;

    y = 0;
    while (y + 1 < state->grid->height) {
        x = 0;
        while (x + 1 < state->grid->width) {
            test_cell(x, y, state);
            x++;
        }
        y++;
    }
}

static sfBool check_request(const terrain_grid_t *grid,
    const camera_iso_t *camera,
    const tile_picker_request_t *request,
    tile_picker_result_t *result)
{
    if (grid == NULL || camera == NULL || request == NULL || result == NULL)
        return sfFalse;
    if (grid->width < 2 || grid->height < 2)
        return sfFalse;
    if (request->tile_size <= 0.0f)
        return sfFalse;
    return sfTrue;
}

sfBool tile_picker_pick_cell(const terrain_grid_t *grid,
    const camera_iso_t *camera,
    const tile_picker_request_t *request,
    tile_picker_result_t *result)
{
    tile_picker_state_t state;
    float threshold;

    if (check_request(grid, camera, request, result) == sfFalse)
        return sfFalse;
    threshold = get_threshold(camera);
    state.grid = grid;
    state.camera = camera;
    state.request = request;
    state.best = 1000000000.0f;
    state.best_x = 0;
    state.best_y = 0;
    scan_cells(&state);
    if (state.best > threshold * threshold)
        return sfFalse;
    result->x = state.best_x;
    result->y = state.best_y;
    return sfTrue;
}
