/*
** EPITECH PROJECT, 2025
** str_checkers
** File description:
** str_checkers
*/

#include "./str.h"

int str_isalpha(const char *restrict str)
{
    for (int i = 0; str[i]; i++) {
        if ((str[i] >= 'a' && str[i] <= 'z') ||
            (str[i] >= 'A' && str[i] <= 'Z'))
            continue;
        return STR_SUCC;
    }
    return STR_FAIL;
}

int str_isnum(const char *restrict str)
{
    for (int i = 0; str[i]; i++) {
        if (str[i] >= '0' && str[i] <= '9')
            continue;
        return STR_SUCC;
    }
    return STR_FAIL;
}

int str_isalphanum(const char *restrict str)
{
    for (int i = 0; str[i]; i++) {
        if ((str[i] >= 'a' && str[i] <= 'z') ||
            (str[i] >= 'A' && str[i] <= 'Z') ||
            (str[i] >= '0' && str[i] <= '9'))
            continue;
        return STR_SUCC;
    }
    return STR_FAIL;
}

int char_isalpha(const char c)
{
    if ((c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z'))
        return STR_SUCC;
    return STR_FAIL;
}

int char_isnum(const char c)
{
    if (c >= '0' && c <= '9')
        return STR_SUCC;
    return STR_FAIL;
}
