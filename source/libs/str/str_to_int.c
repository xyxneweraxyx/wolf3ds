/*
** EPITECH PROJECT, 2025
** str_ncpy
** File description:
** str_ncpy
*/

#include <stdio.h>

int str_to_int(const char *restrict str, int *res)
{
    int total = 0;
    int i = 0;

    if (str[i] < '0' || str[i] > '9') {
        (*res) = 0;
        return 0;
    }
    (*res) = 1;
    for (; str[i] >= '0' && str[i] <= '9'; i++)
        total = total * 10 + (str[i] - '0');
    return total;
}
