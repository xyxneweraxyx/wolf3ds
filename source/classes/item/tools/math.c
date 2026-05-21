/*
** EPITECH PROJECT, 2025
** player
** File description:
** The item class of wolf3d.
*/

#include "./../include/item.h"

float distance_3d(float pos1[3], float pos2[3])
{
    float xsquared = (pos2[0] - pos1[0]) * (pos2[0] - pos1[0]);
    float ysquared = (pos2[1] - pos1[1]) * (pos2[1] - pos1[0]);
    float zsquared = (pos2[2] - pos1[2]) * (pos2[2] - pos1[2]);

    return sqrtf(xsquared + ysquared + zsquared);
}

float distance_2d(float pos1[2], float pos2[2])
{
    float xsquared = (pos2[0] - pos1[0]) * (pos2[0] - pos1[0]);
    float ysquared = (pos2[1] - pos1[1]) * (pos2[1] - pos1[2]);

    return sqrtf(xsquared + ysquared);
}
