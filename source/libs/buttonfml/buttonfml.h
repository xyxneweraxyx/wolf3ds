/*
** EPITECH PROJECT, 2025
** buttonfml
** File description:
** Wrapper on the setfml library to create simple buttons easily.
*/

#ifndef BUTTONFML
    #define BUTTONFML

    // Includes
    #include "./../setfml/setfml.h"

    // Return codes
    #define BUTTONFML_SUCC 0
    #define BUTTONFML_FAIL -1

    // Buffer length
    #define BUTTON_NAME 32

// Enums

typedef enum btn_state_t {
    BUTTON_IDLE,
    BUTTON_HOVERED,
    BUTTON_CLICKED
} btn_state_t;

// Structures

typedef struct btn_text {
    char idle[BUFF_TEXT_PATH]; // The texture used without interactions.
    char hover[BUFF_TEXT_PATH]; // The texture used when button is hovered over.
    char click[BUFF_TEXT_PATH]; // The texture used when button is clicked.
} btn_text_t;

typedef struct btn_clbck {
    size_t (*frame)(setfml_t *setfml, void *userdata); // Triggered every frame
    size_t (*hover)(setfml_t *setfml, void *userdata); // Triggered on hover.
    size_t (*click)(setfml_t *setfml, void *userdata); // Triggered on click.
    size_t (*rel)(setfml_t *setfml, void *userdata); // Triggered on release.
} btn_clbck_t;

typedef struct btn_stats {
    size_t clicked; // Amount of times the button was clicked.
} btn_stats_t;

typedef struct buttonfml {
    c_alloc_t *alloc; // Allocations.
    setfml_t *setfml; // Setfml environment the buttonfml is created in.
    linkedlist_t *buttons; // Linked list of buttons.
} buttonfml_t;

typedef struct button {
    setfml_t *setfml; // The setfml environment the button is linked to.
    sprite_t *button; // The sprite of the button.
    btn_text_t *textures; // The textures used for the button.
    btn_clbck_t *callbacks; // The functions to be executed on events.
    buttonfml_t *buttonfml; // The buttonfml object the button is linked to.
    char name[BUTTON_NAME]; // The name of the button.
    btn_stats_t stats; // Stats of the button at runtime.
    btn_state_t state; // The current state of the button.
    bool is_visible; // Is this button drawn? (Changes hover + click)
    bool is_clickable; // Can this button be clicked? (Changes hover)
} button_t;

// Functions

/// Initializing functions

/*
Initializes a new buttonfml environment given a setfml environment.
Returns the buttonfml environment on success, or NULL on fail.
*/
buttonfml_t *buttonfml_ini(setfml_t *setfml);

/*
Destroys a buttonfml environment.
Returns BUTTONFML_SUCC on success, or BUTTONFML_FAIL on failure.
*/
void buttonfml_destroy(buttonfml_t *buttonfml);

// Button functions

/*
Creates a new button given a buttonfml environment, textures, callbacks, name.
Returns the button on success, or NULL on fail.
*/
button_t *buttonfml_buttoncreate(buttonfml_t *buttonfml, btn_text_t *textures,
    btn_clbck_t *callbacks, char name[BUTTON_NAME]);

/*
Destroys a button and removes its callbacks from its setfml environment.
Setting "destroy_data" to true will also destroy the sprites and textures.
Returns BUTTONFML_SUCC on success, or BUTTONFML_FAIL on failure.
*/
size_t buttonfml_buttondestroy(buttonfml_t *buttonfml, char name[BUTTON_NAME]);

size_t buttonfml_buttonshow(buttonfml_t *buttonfml, char name[BUTTON_NAME]);
size_t buttonfml_buttonhide(buttonfml_t *buttonfml, char name[BUTTON_NAME]);
button_t *buttonfml_buttonfromname(buttonfml_t *buttonfml,
    char name[BUTTON_NAME]);
size_t buttonfml_connectcallbacks(buttonfml_t *buttonfml);
size_t connect_3(buttonfml_t *buttonfml,
    node_t *node, function_t *function);
size_t connect_4(buttonfml_t *buttonfml,
    node_t *node, function_t *function);

#endif
