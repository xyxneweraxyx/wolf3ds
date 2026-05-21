/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Reusable textbox helpers
*/

#ifndef UI_TEXTBOX_H_
    #define UI_TEXTBOX_H_

    #include <stddef.h>
    #include <SFML/Graphics.h>

typedef struct ui_textbox_s {
    sfFloatRect bounds;
    char *buffer;
    size_t capacity;
    size_t max_length;
    sfBool active;
} ui_textbox_t;

typedef struct ui_textbox_options_s {
    sfFloatRect bounds;
    char *buffer;
    size_t capacity;
    size_t max_length;
} ui_textbox_options_t;

typedef struct ui_textbox_style_s {
    sfColor fill_idle;
    sfColor fill_active;
    sfColor border;
    sfColor text;
    unsigned int text_size;
} ui_textbox_style_t;

void ui_textbox_init(ui_textbox_t *textbox,
    const ui_textbox_options_t *options);
sfBool ui_textbox_contains(const ui_textbox_t *textbox, int x, int y);
void ui_textbox_handle_digit_input(ui_textbox_t *textbox, sfUint32 unicode);
void ui_textbox_draw(sfRenderWindow *window,
    sfText *text,
    const ui_textbox_t *textbox,
    const ui_textbox_style_t *style);

#endif /* !UI_TEXTBOX_H_ */
