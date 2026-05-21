/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Directory file listing helpers
*/

#ifndef FILE_LIST_H_
    #define FILE_LIST_H_

    #include <stddef.h>

typedef struct file_list_s {
    char **entries;
    size_t count;
} file_list_t;

void file_list_init(file_list_t *list);
void file_list_clear(file_list_t *list);
int file_list_scan(file_list_t *list,
    const char *directory,
    const char *extension);
const char *file_list_get(const file_list_t *list, size_t index);

#endif /* !FILE_LIST_H_ */
