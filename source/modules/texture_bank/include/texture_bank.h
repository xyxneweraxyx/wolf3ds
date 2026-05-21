/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Texture dictionary
*/

#ifndef TEXTURE_BANK_H_
    #define TEXTURE_BANK_H_

    #include <SFML/Graphics.h>
    #include <stddef.h>

typedef struct texture_bank_entry_s {
    char *id;
    sfTexture *texture;
} texture_bank_entry_t;

typedef struct texture_bank_s {
    texture_bank_entry_t *entries;
    size_t count;
    size_t capacity;
} texture_bank_t;

int texture_bank_init(texture_bank_t *bank);
void texture_bank_destroy(texture_bank_t *bank);
int texture_bank_load(texture_bank_t *bank, const char *id, const char *path);
sfTexture *texture_bank_get(const texture_bank_t *bank, const char *id);

#endif /* !TEXTURE_BANK_H_ */
