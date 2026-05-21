/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Isometric camera model
*/

#ifndef CAMERA_ISO_H_
    #define CAMERA_ISO_H_

    #include <SFML/Graphics.h>

typedef struct camera_iso_options_s {
    float offset_x;
    float offset_y;
    float zoom;
    float angle_x;
    float angle_y;
} camera_iso_options_t;

typedef struct camera_iso_s {
    sfVector2f offset;
    float zoom;
    float angle_x;
    float angle_y;
} camera_iso_t;

void camera_iso_init(camera_iso_t *camera,
    const camera_iso_options_t *options);
void camera_iso_pan(camera_iso_t *camera, float dx, float dy);
void camera_iso_add_zoom(camera_iso_t *camera, float delta,
    float min_zoom, float max_zoom);
void camera_iso_add_rotation(camera_iso_t *camera, float delta_x,
    float delta_y);

#endif /* !CAMERA_ISO_H_ */
