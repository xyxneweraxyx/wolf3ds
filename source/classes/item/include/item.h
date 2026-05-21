/*
** EPITECH PROJECT, 2025
** item
** File description:
** The item class of wolf3d.
*/

#ifndef ITEM_CLASS
    #define ITEM_CLASS

    // Standard includes
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <math.h>
    #include <SFML/Graphics.h>

    // Custom includes
    #include "./../../../libs/classhandler/include/classhandler.h"

    // Defines
    #define ITEM_DEFAULT_MAX_ITEM 25
    #define ITEM_DEFAULT_DETECTION_RADIUS 0.75
    #define ITEM_DEFAULT_DETECTION_ANGLE 90
    #define ITEM_DEFAULT_SPAWN_DELAY 1.0f

// Typedefs

typedef enum item_state_e {
    ITEM_HIDDEN,
    ITEM_DRAWN,
} item_state_t;

typedef struct item_pos_s {
    float x;
    float y;
    float z;
} item_pos_t;

typedef struct item_s {

    sfSprite *sprite;
    sfTexture *texture;

    item_pos_t *positions;
    item_state_t state;

    float detection_radius;
    float detection_angle;

    size_t max_amount;
    size_t current_amount;
    float spawn_delay;
    float since_last_spawn;

    size_t closest_index;

} item_t;

// Tools

/// Math functions
float distance_2d(float pos1[2], float pos2[2]);
float distance_3d(float pos1[3], float pos2[3]);

// Methods

/// GET methods
sfSprite *item_getsprite(entity_t *entity);
sfTexture *item_gettexture(entity_t *entity);
item_state_t item_getstate(entity_t *entity);
float item_getdetectionradius(entity_t *entity);
float item_getdetectionangle(entity_t *entity);
size_t item_getmaxamount(entity_t *entity);
float item_getspawndelay(entity_t *entity);

/// SET methods
size_t item_setdefault(entity_t *entity);
size_t item_setsprite(entity_t *entity, sfSprite *sprite);
size_t item_settexture(entity_t *entity, sfTexture *texture);
size_t item_setstate(entity_t *entity, item_state_t state);
size_t item_setdetectionradius(entity_t *entity, float radius);
size_t item_setdetectionangle(entity_t *entity, float angle);
size_t item_setmaxamount(entity_t *entity, int amount);
size_t item_setspawndelay(entity_t *entity, float delay);

/// GAMEPLAY methods
size_t item_gpclosest(entity_t *entity, float pos[3]);
size_t item_gppickup(entity_t *entity, size_t index,
    float pos[3], float angle);
size_t item_gpspawndelete(entity_t *entity);

#endif
