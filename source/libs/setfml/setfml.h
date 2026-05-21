/*
** EPITECH PROJECT, 2025
** setfml
** File description:
** Tired of initializing csfml? Up your workflow with this simple wrapper.
*/

#ifndef SET_FML
    #define SET_FML

    // Includes
    #include <time.h>
    #include <SFML/Audio.h>
    #include <SFML/Config.h>
    #include <SFML/Graphics.h>
    #include <SFML/System.h>
    #include <SFML/Window.h>
    #include "./../c_alloc/c_alloc.h" // MUST INCLUDE
    #include "./../linkedlist/linkedlist.h" // MUST INCLUDE
    #include "./../str/str.h" // MUST INCLUDE

    // Return codes
    #define SETFML_SUCC 0
    #define SETFML_FAIL -1
    #define SETFML_WINDOWCLOSING -2

    // Buffer lengths
    #define BUFF_FUNC_NAME 32
    #define BUFF_SPRITE_NAME 32
    #define BUFF_TEXT_NAME 32
    #define BUFF_TEXT_PATH 128

    // Time definitions
    #define SEC_TO_NANO 1000000000
    #define SEC_TO_MICRO 1000000
    #define SEC_TO_MILLI 1000
    #define MILLI_TO_MICRO 1000
    #define MILLI_TO_NANO 1000000
    #define MICRO_TO_NANO 1000

    // Hardcoded values
    #define C_ALLOC_BASE 512
    #define SETFML_EVT_AMT 24
    #define SETFML_LINKEDLIST_AMT 24 + 3 // All events plus data, render, draw

// Enums

typedef enum loop {
    LOOP_DATA = 24,
    LOOP_RENDER = 25,
    LOOP_DRAW = 26,
} loop_t;

// Typedefs

typedef struct window_params {
    const char *title;
    sfContextSettings *settings;
    uint16_t min_scr_res[2];
    uint16_t max_scr_res[2];
    sfUint32 style;
    uint8_t fps;
} window_params_t;

typedef struct time_params {
    size_t event; // In nanoseconds
    size_t data; // In nanoseconds
    size_t render; // In nanoseconds
    size_t draw; // In nanoseconds
} time_params_t;

typedef struct time_elapsed {
    struct timespec event;
    struct timespec data;
    struct timespec render;
    struct timespec draw;
} time_elapsed_t;

typedef struct loop_exec {
    bool event;
    bool data;
    bool render;
    bool draw;
} loop_exec_t;

typedef struct params {
    window_params_t window;
    time_params_t time;
} params_t;

typedef struct state {
    uint32_t frame;
    time_elapsed_t time_elapsed;
    time_elapsed_t last_exec;
    loop_exec_t to_exec;
    bool is_closing;
} state_t;

typedef struct texture {
    char name[BUFF_TEXT_NAME];
    char path[BUFF_TEXT_PATH];
    sfTexture *texture;
} texture_t;

typedef struct sprite {
    char name[BUFF_SPRITE_NAME];
    sfSprite *sprite;
    texture_t *texture;
    size_t original_win_x;
    size_t original_win_y;
} sprite_t;

typedef struct setfml {
    c_alloc_t *alloc;
    sfRenderWindow *window;
    linkedlist_t *sprites;
    linkedlist_t *textures;
    linkedlist_t *functions[SETFML_LINKEDLIST_AMT];
    sfEvent event;
    void *userdata; // par exemple pour moi ça serait mon main_t
    params_t params;
    state_t state;
} setfml_t;

typedef struct function {
    char name[BUFF_FUNC_NAME];
    size_t (*function)(setfml_t *setfml, void *userdata);
    void *userdata; // Any user data that should be added before the callback.
    size_t return_code;
    bool paused;
} function_t;

typedef struct setfml_func_comp {
    size_t (*f_before)(setfml_t *setfml, void *userdata);
    size_t (*callback)(setfml_t *setfml, void *userdata);
} setfml_func_comp_t;

// Functions

/// Initialization functions

/*
Initialize a setfml environment with given userdata.
Returns the new setfml environment, or NULL.
*/
setfml_t *setfml_ini(void *userdata);

/*
Destroys a setfml environment.
This will destroy the textures, sprites, linked lists, and allocations.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_destroy(setfml_t *setfml);

/*
Fills the parameters of a setfml environment with default parameters.
*/
void setfml_fillparams(setfml_t *setfml);

/// Window functions

/*
Creates a csfml window and stores it in the setfml environment.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_windowcreate(setfml_t *setfml);
size_t setfml_windowrecreate(setfml_t *setfml,
    const sfVideoMode *mode, sfUint32 style);

/*
Starts a setfml window by activating the loop of callback functions.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_windowstart(setfml_t *setfml);

/*
Closes a setfml window.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_windowclose(setfml_t *setfml);

/*
Deletes a setfml window.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_windowdestroy(setfml_t *setfml);

/*
Performs a single loop iteration over an open setfml window.
Returns SETFML_SUCC or SETFML_FAIL.
*/
void setfml_iteration(setfml_t *setfml);

/// Texture functions

/*
Creates and stores a csfml texture.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_textureadd(setfml_t *setfml, char name[BUFF_TEXT_NAME],
    char path[BUFF_TEXT_PATH]);

/*
Deletes a csfml texture.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_texturedel(setfml_t *setfml, char name[BUFF_TEXT_NAME]);

/*
Finds a texture based on its name.
Returns the texture's node/texture data, or NULL.
*/
void *setfml_texturefromname(setfml_t *setfml,
    char name[BUFF_TEXT_NAME], bool return_node);

/// Sprite functions

/*
Creates and stores a csfml sprite with it's corresponding texture.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_spriteadd(setfml_t *setfml, char name[BUFF_SPRITE_NAME],
    texture_t *texture);

/*
Deletes a csfml sprite.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_spritedel(setfml_t *setfml, char name[BUFF_SPRITE_NAME]);

/*
Finds a sprite based on its name.
Returns the sprite's node/sprite data, or NULL.
*/
void *setfml_spritefromname(setfml_t *setfml,
    char name[BUFF_SPRITE_NAME], bool return_node);

/*
Changes a sprite's texture based on the name.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_spritechangetexture(setfml_t *setfml, sprite_t *sprite,
    char texture_name[BUFF_TEXT_NAME]);

/// Callback functions

/*
Adds a callback function to a linked list of events.
Compatible with sfEventType, or any of the LOOP states (data, render, draw)
If functions->f_before is NULL, the new function will become the new head.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_add(setfml_t *setfml, setfml_func_comp_t *functions,
    char name[BUFF_FUNC_NAME], size_t event);

/*
Deletes a callback function based on its name.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_del(setfml_t *sketfml, char name[BUFF_FUNC_NAME]);

/*
Pauses a callback function based on its name.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_pause(setfml_t *setfml, char name[BUFF_FUNC_NAME]);

/*
Resumes a callback function based on its name.
Returns SETFML_SUCC or SETFML_FAIL.
*/
size_t setfml_resume(setfml_t *setfml, char name[BUFF_FUNC_NAME]);

/// Api functions

/*
Attempts to find a function in a given event based on its name.
Returns the function's node in the event's linked list, or NULL
*/
node_t *setfml_nodefromfunc(setfml_t *setfml,
    char name[BUFF_FUNC_NAME], size_t event);

#endif
