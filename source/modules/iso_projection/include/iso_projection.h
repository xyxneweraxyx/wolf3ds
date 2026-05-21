/*
** EPITECH PROJECT, 2025
** My_World
** File description:
** Isometric projection helpers
*/

#ifndef ISO_PROJECTION_H_
    #define ISO_PROJECTION_H_

    #include <SFML/Graphics.h>
    #include "camera_iso.h"

sfVector2f iso_project_point(const sfVector3f *point,
    const camera_iso_t *camera);

#endif /* !ISO_PROJECTION_H_ */
