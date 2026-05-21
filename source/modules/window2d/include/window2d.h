/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Generic 2D window helper
*/

#ifndef WINDOW2D_H_
    #define WINDOW2D_H_

    #include <SFML/Graphics.h>
    #include <SFML/Window.h>

typedef struct window2d_options_s {
    unsigned int width;
    unsigned int height;
    const char *title;
    sfUint32 style;
    unsigned int framerate;
} window2d_options_t;

typedef struct window2d_s {
    sfRenderWindow *handle;
    unsigned int width;
    unsigned int height;
} window2d_t;

int window2d_create(window2d_t *window, const window2d_options_t *options);
void window2d_destroy(window2d_t *window);
void window2d_clear(window2d_t *window, const sfColor *color);
void window2d_display(window2d_t *window);
sfBool window2d_poll_event(window2d_t *window, sfEvent *event);

#endif /* !WINDOW2D_H_ */
