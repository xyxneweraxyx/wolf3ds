/*
** EPITECH PROJECT, 2025
** raycaster
** File description:
** Raycaster depth buffer helpers.
*/

#include "./../include/raycaster.h"
#include "./private.h"

bool raycast_resize_depth_buffer(raycast_t *raycast, size_t width)
{
    c_realloc_t sizes = {raycast->depth_width, width};
    float *buffer = NULL;

    if (raycast->depth_buffer && raycast->depth_width == width)
        return true;
    if (!raycast->depth_buffer)
        buffer = c_alloc(sizeof(float), width, raycast->alloc);
    else
        buffer = c_realloc(raycast->alloc, raycast->depth_buffer,
            &sizes, sizeof(float));
    if (!buffer)
        return false;
    raycast->depth_buffer = buffer;
    raycast->depth_width = width;
    return true;
}

void raycast_reset_depth_buffer(raycast_t *raycast)
{
    for (size_t i = 0; i < raycast->depth_width; i++)
        raycast->depth_buffer[i] = FLT_MAX;
}

void raycast_store_depth(raycast_t *raycast, col_data_t *data)
{
    if (!raycast->depth_buffer || data->column >= raycast->depth_width)
        return;
    if (data->distance < raycast->depth_buffer[data->column])
        raycast->depth_buffer[data->column] = data->distance;
}

bool raycast_is_occluded(raycast_t *raycast, col_data_t *data)
{
    raycast_t *source = raycast->depth_source;

    if (!source || !source->depth_buffer ||
        data->column >= source->depth_width)
        return false;
    return data->distance > source->depth_buffer[data->column];
}
