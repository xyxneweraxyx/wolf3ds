/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Directory file listing helpers implementation
*/

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "file_list.h"

void file_list_init(file_list_t *list)
{
    if (list == NULL)
        return;
    list->entries = NULL;
    list->count = 0;
}

void file_list_clear(file_list_t *list)
{
    size_t i;

    if (list == NULL)
        return;
    i = 0;
    while (i < list->count) {
        free(list->entries[i]);
        i++;
    }
    free(list->entries);
    list->entries = NULL;
    list->count = 0;
}

static char *copy_string(const char *text)
{
    size_t len;
    char *copy;

    len = strlen(text);
    copy = malloc(len + 1);
    if (copy == NULL)
        return NULL;
    memcpy(copy, text, len + 1);
    return copy;
}

static int has_extension(const char *name, const char *extension)
{
    size_t name_len;
    size_t ext_len;

    if (extension == NULL || extension[0] == '\0')
        return 1;
    name_len = strlen(name);
    ext_len = strlen(extension);
    if (name_len < ext_len)
        return 0;
    return strcmp(name + name_len - ext_len, extension) == 0;
}

static int append_entry(file_list_t *list, const char *name)
{
    char **new_entries;
    char *copy;

    copy = copy_string(name);
    if (copy == NULL)
        return 84;
    new_entries = realloc(list->entries, sizeof(char *) * (list->count + 1));
    if (new_entries == NULL) {
        free(copy);
        return 84;
    }
    list->entries = new_entries;
    list->entries[list->count] = copy;
    list->count++;
    return 0;
}

static int compare_names(const void *left, const void *right)
{
    const char *left_name;
    const char *right_name;

    left_name = *(const char *const *)left;
    right_name = *(const char *const *)right;
    return strcmp(left_name, right_name);
}

static int scan_entries(file_list_t *list, DIR *dir, const char *extension)
{
    struct dirent *entry;

    entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_name[0] == '.' ||
            has_extension(entry->d_name, extension) == 0) {
            entry = readdir(dir);
            continue;
        }
        if (append_entry(list, entry->d_name) == 84)
            return 84;
        entry = readdir(dir);
    }
    return 0;
}

int file_list_scan(file_list_t *list, const char *directory,
    const char *extension)
{
    DIR *dir;

    if (list == NULL || directory == NULL)
        return 84;
    file_list_clear(list);
    dir = opendir(directory);
    if (dir == NULL)
        return 84;
    if (scan_entries(list, dir, extension) == 84) {
        closedir(dir);
        file_list_clear(list);
        return 84;
    }
    closedir(dir);
    if (list->count > 1)
        qsort(list->entries, list->count, sizeof(char *), compare_names);
    return 0;
}

const char *file_list_get(const file_list_t *list, size_t index)
{
    if (list == NULL || index >= list->count)
        return NULL;
    return list->entries[index];
}
