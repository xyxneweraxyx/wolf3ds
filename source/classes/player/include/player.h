/*
** EPITECH PROJECT, 2025
** player
** File description:
** The player class of wolf3d.
*/

#ifndef PLAYER_CLASS
    #define PLAYER_CLASS

    // Standard includes
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    // Custom includes
    #include "./../../../libs/classhandler/include/classhandler.h"

    // Defines
    #define PLAYER_DEFAULT_HEALTH 100
    #define PLAYER_DEFAULT_DMG 100
    #define PLAYER_DEFAULT_MOVE_SPEED 1
    #define PLAYER_DEFAULT_JUMP_POWER 20
    #define PLAYER_DEFAULT_ROTATION 0
    #define PLAYER_DEFAULT_ROT_SPEED 30
    #define PLAYER_DEFAULT_GRAVITY 1

// Typedefs

typedef enum player_state_e {
    PLAYER_IDLE,
    PLAYER_MOVING,
    PLAYER_SPRINTING,
} player_state_t;

typedef struct player_pos_s {
    float x;
    float y;
    float z;
} player_pos_t;

typedef struct player_s {

    player_pos_t pos;
    player_state_t state;

    uint16_t health;
    uint16_t dmg;

    float jump_power;
    float move_speed;

    float rotation;
    float rot_speed;

    float jmp_strength;
    float gravity;
    float jmp_gravity;

} player_t;

// Methods

/// GET methods
float player_getspeed(entity_t *entity);
float player_getgravity(entity_t *entity);
float player_getjumppower(entity_t *entity);
float player_getrotation(entity_t *entity);
float player_getrotspeed(entity_t *entity);
uint16_t player_gethealth(entity_t *entity);
uint16_t player_getdmg(entity_t *entity);
player_state_t player_getstate(entity_t *entity);
player_pos_t player_getpos(entity_t *entity);

/// SET methods
size_t player_setdefault(entity_t *entity);
size_t player_setrotspeed(entity_t *entity, float value);
size_t player_setrotation(entity_t *entity, float value);
size_t player_setgravity(entity_t *entity, float value);
size_t player_setspeed(entity_t *entity, float value);
size_t player_setjumppower(entity_t *entity, float value);
size_t player_sethealth(entity_t *entity, uint16_t value);
size_t player_setdmg(entity_t *entity, uint16_t value);
size_t player_setstate(entity_t *entity, player_state_t state);
size_t player_setpos(entity_t *entity, player_pos_t *pos);

/// ADD methods
size_t player_addrotation(entity_t *entity, float value);
size_t player_addrotspeed(entity_t *entity, float value);
size_t player_addspeed(entity_t *entity, float value);
size_t player_addjumppower(entity_t *entity, float value);
size_t player_addhealth(entity_t *entity, uint16_t value);
size_t player_adddmg(entity_t *entity, uint16_t value);

/// GAMEPLAY methods
size_t player_gpmovex(entity_t *entity, float value);
size_t player_gpmovey(entity_t *entity, float value);
size_t player_gpmovez(entity_t *entity, float value);

// More complex gameplay methods can be added later.
size_t player_gpjump(entity_t *entity);
//size_t player_gpattack(const char *name);
//size_t player_gpdie(const char *name);

#endif
