/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Texture dictionary implementation
*/

#include <stdlib.h>
#include "texture_bank.h"

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

static int ensure_capacity(texture_bank_t *bank)
{
    texture_bank_entry_t *new_entries;
    size_t i;
    size_t new_capacity;

    if (bank->count < bank->capacity)
        return 0;
    new_capacity = (bank->capacity == 0) ? 4 : bank->capacity * 2;
    new_entries = malloc(sizeof(texture_bank_entry_t) * new_capacity);
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

static void entry_destroy(texture_bank_entry_t *entry)
{
    free(entry->id);
    if (entry->texture != NULL)
        sfTexture_destroy(entry->texture);
}

int texture_bank_init(texture_bank_t *bank)
{
    if (bank == NULL)
        return 84;
    bank->entries = NULL;
    bank->count = 0;
    bank->capacity = 0;
    return 0;
}

void texture_bank_destroy(texture_bank_t *bank)
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

sfTexture *texture_bank_get(const texture_bank_t *bank, const char *id)
{
    size_t i;

    if (bank == NULL || id == NULL)
        return NULL;
    i = 0;
    while (i < bank->count) {
        if (str_equals(bank->entries[i].id, id))
            return bank->entries[i].texture;
        i++;
    }
    return NULL;
}

static int push_entry(texture_bank_t *bank, const char *id, sfTexture *texture)
{
    texture_bank_entry_t *entry;

    if (ensure_capacity(bank) == 84)
        return 84;
    entry = &bank->entries[bank->count];
    entry->id = str_duplicate(id);
    if (entry->id == NULL)
        return 84;
    entry->texture = texture;
    bank->count++;
    return 0;
}

int texture_bank_load(texture_bank_t *bank, const char *id, const char *path)
{
    sfTexture *texture;

    if (bank == NULL || id == NULL || path == NULL)
        return 84;
    if (texture_bank_get(bank, id) != NULL)
        return 84;
    texture = sfTexture_createFromFile(path, NULL);
    if (texture == NULL)
        return 84;
    if (push_entry(bank, id, texture) == 84) {
        sfTexture_destroy(texture);
        return 84;
    }
    return 0;
}
