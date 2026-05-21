/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Sound system definitions for wolf3d
*/

#ifndef SOUND_H
    #define SOUND_H

    #include <SFML/Audio.h>

    #define SFML_SND_STEP "./assets/audio/step.mp3"
    #define SFML_SND_SHOOT "./assets/audio/shoot_sound.mp3"
    #define SFML_STEP_VOLUME 40.0f
    #define SFML_SHOOT_VOLUME 10.0f

typedef struct sound_s {
    sfSoundBuffer *buffer;
    sfSound *sound;
} sound_t;

#endif
