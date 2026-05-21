/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu system definitions for wolf3d
*/

#include "private.h"

#ifndef SHOOT_H
    #define SHOOT_H

    #include "./wolf3d.h"
    #define SHOOT_SPRITE "./assets/textures/weapon.png"
    #define BULLET_SPRITE "./assets/textures/bullet.png"
    #define AMMO_FONT "./assets/fonts/PressStart2P-Regular.ttf"
    #define BULLET_FRAME_DURATION 0.2
    #define BULLET_FRAME_COUNT 3
    #define BULLET_FIRE_RATE_NS 200000000L
    #define BULLET_UPDATE_RATE_NS 200000000L
    #define WEAPON_MAX_AMMO 30


typedef struct bullet_s {
    float pos_x;
    float pos_y;
    float angle;
    float speed;
    int time;
    sfSprite *sprite;
    sfVector2f scale;
    struct bullet_s *next;
} bullet_t;

typedef struct weapon_s {
    sfSprite *weapon_sprite;
    sfTexture *weapon_texture;
    sfTexture *bullet_texture;
    struct bullet_s *bullet;
    struct timespec start;
    int nb_bullet;
    sfText *ammo_text;
    sfFont *ammo_font;
} weapon_t;

size_t draw_weapon(setfml_t *setfml, void *userdata);
size_t shoot(setfml_t *setfml, void *userdata);
size_t update_bullets(setfml_t *setfml, void *userdata);

#endif
