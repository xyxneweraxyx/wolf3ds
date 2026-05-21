/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Sound system initialization for wolf3d
*/

#include "./../../include/wolf3d.h"
#include "./include/sound.h"

void destroy_sound(sound_t *sound)
{
    if (!sound)
        return;
    if (sound->sound)
        sfSound_destroy(sound->sound);
    if (sound->buffer)
        sfSoundBuffer_destroy(sound->buffer);
    free(sound);
}

void init_step_sound(wolf_t *wolf)
{
    wolf->step_sound = malloc(sizeof(sound_t));
    if (!wolf->step_sound)
        return;
    wolf->step_sound->buffer = sfSoundBuffer_createFromFile(SFML_SND_STEP);
    wolf->step_sound->sound = sfSound_create();
    sfSound_setBuffer(wolf->step_sound->sound, wolf->step_sound->buffer);
    sfSound_setVolume(wolf->step_sound->sound, SFML_STEP_VOLUME);
}

void init_shoot_sound(wolf_t *wolf)
{
    wolf->shoot_sound = malloc(sizeof(sound_t));
    if (!wolf->shoot_sound)
        return;
    wolf->shoot_sound->buffer = sfSoundBuffer_createFromFile(SFML_SND_SHOOT);
    wolf->shoot_sound->sound = sfSound_create();
    sfSound_setBuffer(wolf->shoot_sound->sound, wolf->shoot_sound->buffer);
    sfSound_setVolume(wolf->shoot_sound->sound, SFML_SHOOT_VOLUME);
}

int init_sound(wolf_t *wolf)
{
    init_step_sound(wolf);
    init_shoot_sound(wolf);
    return WOLF_SUCC;
}
