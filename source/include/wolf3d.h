/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Main header file of wolf3d
*/

#ifndef WOLF3D_H
    #define WOLF3D_H

    #define _GNU_SOURCE

    // Standard includes
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <unistd.h>
    #include <math.h>
    #include <time.h>

    // Custom includes
    #include "./../libs/c_alloc/c_alloc.h"
    #include "./../libs/linkedlist/linkedlist.h"
    #include "./../libs/str/str.h"
    #include "./../libs/buttonfml/buttonfml.h"
    #include "./../libs/setfml/setfml.h"
    #include "./../libs/raycaster/include/raycaster.h"
    #include "./../libs/classhandler/include/classhandler.h"

    // Classes
    #include "./../classes/player/include/player.h"
    #include "./../classes/enemy/include/enemy.h"
    #include "./../classes/item/include/item.h"
    #include "./../classes/inventory/include/inventory.h"

    #include "../src/hud/shoot/include/shoot.h"
    #include "../src/sound/include/sound.h"

    /// Return codes
    #define WOLF_SUCC 0
    #define WOLF_FAIL 84

    /// Buffer lengths
    #define BUFF_LEN_NAME 32

    /// Default parameter values
    #define PARAMS_DEFAULT_NAME "Bowser"

    /// Classes definition
    #define CLASS_PLAYERS "player"
    #define CLASS_ENEMY "enemy"
    #define CLASS_INVENTORY "inventory"
    #define CLASS_ITEM "item"

    /// Item definitions
    #define ITEM_SCORE "score"

    /// Csfml assets
    #define SFML_TXT_WALL "./assets/textures/wall.png"
    #define SFML_TXT_MUSHROOM "./assets/textures/mushroom.png"
    #define SFML_TXT_GROUND "./assets/textures/ground.png"
    #define SFML_TXT_CEILING "./assets/textures/ceiling.png"

    #ifndef MENU_T_DEFINED
        #define MENU_T_DEFINED
typedef struct menu_s menu_t;
    #endif

    #ifndef LIFE_T_DEFINED
        #define LIFE_T_DEFINED
typedef struct life_s life_t;
    #endif

typedef struct map_2d_s map_2d_t;

// Typedefs

typedef struct object_hit_s {
    int map_x;
    int map_y;
    char type;
} object_hit_t;

struct object_sprite_s {
    sfVector2f center;
    float distance;
    float screen_x;
    char type;
};

typedef enum gamestate_e {
    GAME_MENU,
    GAME_SETTINGS,
    GAME_CINEMATIC,
    GAME_PLAY,
    GAME_PAUSED,
} gamestate_t;

typedef struct wolf_s {
    c_alloc_t *alloc;
    classhandler_t *classhandler;
    setfml_t *setfml;
    buttonfml_t *buttonfml;
    char **map;
    char **object_map;
    gamestate_t state;
    params_t params;
    raycast_t *raycast;
    raycast_t *object_raycast;
    menu_t *menu;
    life_t *life;
    map_2d_t *map_2d;
    object_hit_t *object_hits;
    size_t object_hit_count;
    size_t object_hit_capacity;
    struct object_sprite_s *object_sprites;
    size_t object_sprite_count;
    size_t object_sprite_capacity;
    struct weapon_s *weapon;
    sound_t *step_sound;
    sound_t *shoot_sound;
} wolf_t;

// Functions

int ini_csfml_env(wolf_t *wolf);
int connect_callbacks(wolf_t *wolf);
int destroy_return_int(int ret_code, wolf_t *wolf);
void *destroy_return_null(wolf_t *wolf);
int create_classes(wolf_t *wolf);
int menu_init(wolf_t *wolf);
void menu_destroy(menu_t *menu);
int init_life(wolf_t *wolf);
int connect_life(wolf_t *wolf);
int destroy_life(life_t *life);
int init_map_2d(wolf_t *wolf);
int connect_map_2d(wolf_t *wolf);
int connect_weapon(wolf_t *wolf);
int init_weapon(wolf_t *wolf);
int init_sound(wolf_t *wolf);
void destroy_sound(sound_t *sound);
int save_game(wolf_t *wolf);
int load_game(wolf_t *wolf);

#endif