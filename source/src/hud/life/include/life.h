/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu system definitions for wolf3d
*/

#ifndef LIFE_H
    #define LIFE_H

    #include "wolf3d.h"

    #define FONT_LIFE "./assets/textures/life.png"
    #define LIFE_MAX 175
    #define LIFE_START 100
    #define DAMMAGE 20
    #define NUMBER_SPRITE LIFE_MAX / DAMMAGE
    #define LOSE -1

typedef struct life_sprite_s {
    sfSprite *sprite;
    int if_active;
    sfIntRect rect;
} life_sprite_t;

    #ifndef LIFE_T_DEFINED
        #define LIFE_T_DEFINED
typedef struct life_s life_t;
    #endif

struct life_s {
    struct life_sprite_s life_sprite[NUMBER_SPRITE];
    sfTexture *life_sp;
    int current_life;
    int max_life;
    int dammage;
    int nb_draw;
};

int init_life(wolf_t *wolf);
int connect_life(wolf_t *wolf);
int destroy_life(life_t *life);
size_t draw_life(setfml_t *setfml, void *userdata);
int update_life(int action, wolf_t *wolf, int dammage);

#endif
