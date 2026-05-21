/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Reusable rectangle button helpers
*/

#ifndef UI_BUTTON_H_
    #define UI_BUTTON_H_

    #include <SFML/Graphics.h>

typedef enum ui_button_state_e {
    UI_BUTTON_IDLE = 0,
    UI_BUTTON_HOVER,
    UI_BUTTON_ACTIVE
} ui_button_state_t;

typedef struct ui_button_style_s {
    sfColor fill_idle;
    sfColor fill_hover;
    sfColor fill_active;
    sfColor border;
    sfColor text;
    unsigned int text_size;
} ui_button_style_t;

typedef struct ui_button_s {
    sfFloatRect bounds;
    const char *label;
    ui_button_state_t state;
} ui_button_t;

sfBool ui_button_contains(const ui_button_t *button, int x, int y);
ui_button_state_t ui_button_get_state(const ui_button_t *button,
    int x,
    int y,
    sfBool active);
void ui_button_draw(sfRenderWindow *window,
    sfText *text,
    const ui_button_t *button,
    const ui_button_style_t *style);

#endif /* !UI_BUTTON_H_ */
