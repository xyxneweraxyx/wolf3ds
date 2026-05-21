/*
** EPITECH PROJECT, 2025
** str
** File description:
** A simple library with string functions.
*/

#ifndef STRING_LIB
    #define STRING_LIB

    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #define STR_SUCC 0
    #define STR_FAIL -1

// Functions

int str_len(const char *restrict len);
void str_cpy(const char *restrict src, char *restrict dest);
int str_cmp(const char *restrict str1, const char *restrict str2);
int str_ncmp(const char *restrict str1,
    const char *restrict str2, const int n);
void str_cat(char *restrict dest, int n, ...);
void str_ncpy(const char *restrict src, char *restrict dest, const int n);
void str_low(char *restrict __str);
void str_upp(char *restrict __str);
char *str_dup(const char *restrict __str);
int str_isalpha(const char *restrict str);
int str_isnum(const char *restrict str);
int str_isalphanum(const char *restrict str);
int char_isalpha(const char c);
int char_isnum(const char c);
int str_to_int(const char *restrict str, int *res);

#endif
