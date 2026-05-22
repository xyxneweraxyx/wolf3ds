/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Menu system definitions for wolf3d
*/

#ifndef MENU_H
    #define MENU_H

    #include "./wolf3d.h"

    #define MENU_BUTTON_COUNT 13
    #define MENU_ACTION_DELAY 0.18f
    #define MENU_BUTTON_FRAME_WIDTH 1536
    #define MENU_BUTTON_FRAME_HEIGHT 341
    #define MENU_BUTTON_BODY_X 373
    #define MENU_BUTTON_BODY_WIDTH 790
    #define MENU_BUTTON_BODY_HEIGHT 187
    #define MENU_BUTTON_IDLE_TOP 154
    #define MENU_BUTTON_HOVERED_TOP 80
    #define MENU_BUTTON_CLICKED_TOP 13
    #define MENU_TXT_BACKGROUND "./assets/textures/background.png"
    #define MENU_TXT_BUTTONS "./assets/textures/buttons.png"
    #define MENU_FONT_MAIN "./assets/fonts/PressStart2P-Regular.ttf"

typedef enum menu_action_e {
    MENU_ACTION_NONE,
    MENU_ACTION_START,
    MENU_ACTION_SETTINGS,
    MENU_ACTION_VOLUME_DOWN,
    MENU_ACTION_VOLUME_UP,
    MENU_ACTION_RES_DOWN,
    MENU_ACTION_RES_UP,
    MENU_ACTION_FULLSCREEN,
    MENU_ACTION_BACK,
    MENU_ACTION_QUIT,
    MENU_ACTION_SAVE,
    MENU_ACTION_LOAD,
} menu_action_t;

typedef enum menu_button_state_e {
    MENU_BUTTON_IDLE,
    MENU_BUTTON_HOVERED,
    MENU_BUTTON_CLICKED,
} menu_button_state_t;

typedef struct menu_button_s {
    sfSprite *sprite;
    sfText *text;
    menu_action_t action;
    menu_button_state_t state;
    sfVector2f position;
    sfVector2f scale;
    bool visible;
} menu_button_t;

    #ifndef MENU_T_DEFINED
        #define MENU_T_DEFINED
typedef struct menu_s menu_t;
    #endif

struct menu_s {
    sfTexture *background_texture;
    sfTexture *button_texture;
    sfFont *font;
    sfSprite *background_sprite;
    sfRectangleShape *overlay;
    sfText *title;
    sfText *subtitle;
    sfText *settings_text;
    sfText *cinematic_title;
    sfText *cinematic_text;
    sfText *cinematic_hint;
    menu_button_t buttons[MENU_BUTTON_COUNT];
    sfVector2u windowed_size;
    bool fullscreen;
    int volume;
    int resolution_index;
    int pressed_button;
    int pending_button;
    menu_action_t pending_action;
    sfClock *action_clock;
    sfClock *cinematic_clock;
};

int connect_menu(wolf_t *wolf);
void menu_apply_action(wolf_t *wolf, menu_action_t action);
void menu_refresh(menu_t *menu, const sfVector2u *size, gamestate_t state);
void menu_set_settings_layout(menu_t *menu,
    const sfVector2u *size, float button_width);
void menu_reset(menu_t *menu, gamestate_t state);
void menu_set_button_state(menu_button_t *button, menu_button_state_t state);
sfFloatRect menu_get_button_hitbox(const menu_button_t *button);
void menu_center_text(sfText *text, const sfFloatRect *rect);
void menu_center_full_width(sfText *text,
    const sfVector2u *size, float top, float height);
size_t menu_on_draw(setfml_t *setfml, void *userdata);
size_t menu_on_keypress(setfml_t *setfml, void *userdata);
size_t menu_on_render(setfml_t *setfml, void *userdata);
size_t menu_on_mousemove(setfml_t *setfml, void *userdata);
size_t menu_on_mousepress(setfml_t *setfml, void *userdata);
size_t menu_on_mouserelease(setfml_t *setfml, void *userdata);

#endif
