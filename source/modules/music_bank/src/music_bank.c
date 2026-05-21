/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Music dictionary implementation
*/

#include <stdlib.h>
#include "music_bank.h"

static int str_equals(const char *left, const char *right)
{
    size_t i;

    if (left == NULL || right == NULL)
        return 0;
    i = 0;
    while (left[i] != '\0' && right[i] != '\0') {
        if (left[i] != right[i])
            return 0;
        i++;
    }
    return left[i] == right[i];
}

static char *str_duplicate(const char *src)
{
    char *copy;
    size_t i;

    if (src == NULL)
        return NULL;
    i = 0;
    while (src[i] != '\0')
        i++;
    copy = malloc(sizeof(char) * (i + 1));
    if (copy == NULL)
        return NULL;
    i = 0;
    while (src[i] != '\0') {
        copy[i] = src[i];
        i++;
    }
    copy[i] = '\0';
    return copy;
}

static int ensure_capacity(music_bank_t *bank)
{
    music_bank_entry_t *new_entries;
    size_t i;
    size_t new_capacity;

    if (bank->count < bank->capacity)
        return 0;
    new_capacity = (bank->capacity == 0) ? 4 : bank->capacity * 2;
    new_entries = malloc(sizeof(music_bank_entry_t) * new_capacity);
    if (new_entries == NULL)
        return 84;
    i = 0;
    while (i < bank->count) {
        new_entries[i] = bank->entries[i];
        i++;
    }
    free(bank->entries);
    bank->entries = new_entries;
    bank->capacity = new_capacity;
    return 0;
}

static void entry_destroy(music_bank_entry_t *entry)
{
    free(entry->id);
    if (entry->music != NULL)
        sfMusic_destroy(entry->music);
}

int music_bank_init(music_bank_t *bank)
{
    if (bank == NULL)
        return 84;
    bank->entries = NULL;
    bank->count = 0;
    bank->capacity = 0;
    return 0;
}

void music_bank_destroy(music_bank_t *bank)
{
    size_t i;

    if (bank == NULL)
        return;
    i = 0;
    while (i < bank->count) {
        entry_destroy(&bank->entries[i]);
        i++;
    }
    free(bank->entries);
    bank->entries = NULL;
    bank->count = 0;
    bank->capacity = 0;
}

sfMusic *music_bank_get(const music_bank_t *bank, const char *id)
{
    size_t i;

    if (bank == NULL || id == NULL)
        return NULL;
    i = 0;
    while (i < bank->count) {
        if (str_equals(bank->entries[i].id, id))
            return bank->entries[i].music;
        i++;
    }
    return NULL;
}

static int push_entry(music_bank_t *bank, const char *id, sfMusic *music)
{
    music_bank_entry_t *entry;

    if (ensure_capacity(bank) == 84)
        return 84;
    entry = &bank->entries[bank->count];
    entry->id = str_duplicate(id);
    if (entry->id == NULL)
        return 84;
    entry->music = music;
    bank->count++;
    return 0;
}

int music_bank_load(music_bank_t *bank, const char *id, const char *path)
{
    sfMusic *music;

    if (bank == NULL || id == NULL || path == NULL)
        return 84;
    if (music_bank_get(bank, id) != NULL)
        return 84;
    music = sfMusic_createFromFile(path);
    if (music == NULL)
        return 84;
    if (push_entry(bank, id, music) == 84) {
        sfMusic_destroy(music);
        return 84;
    }
    return 0;
}
