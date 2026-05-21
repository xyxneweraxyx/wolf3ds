/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Music dictionary
*/

#ifndef MUSIC_BANK_H_
    #define MUSIC_BANK_H_

    #include <SFML/Audio.h>
    #include <stddef.h>

typedef struct music_bank_entry_s {
    char *id;
    sfMusic *music;
} music_bank_entry_t;

typedef struct music_bank_s {
    music_bank_entry_t *entries;
    size_t count;
    size_t capacity;
} music_bank_t;

int music_bank_init(music_bank_t *bank);
void music_bank_destroy(music_bank_t *bank);
int music_bank_load(music_bank_t *bank, const char *id, const char *path);
sfMusic *music_bank_get(const music_bank_t *bank, const char *id);

#endif /* !MUSIC_BANK_H_ */
