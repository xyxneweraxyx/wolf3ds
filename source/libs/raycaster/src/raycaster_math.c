/*
** EPITECH PROJECT, 2025
** raycaster
** File description:
** A raycasting lib for wolf3d.
*/

#include "./../include/raycaster.h"
#include "./private.h"

size_t number_in_range_f(float number, float base, float range)
{
    return (number <= base + range && number >= base - range);
}

size_t number_in_range_i(int number, int base, int range)
{
    return (number <= base + range && number >= base - range);
}
