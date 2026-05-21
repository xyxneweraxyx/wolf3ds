/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Callback connexion entry point.
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

static void init_rect(sfIntRect *rect,
    col_data_t *data, sfVector2u *tex_size)
{
    rect->left = data->face_x * tex_size->x;
    rect->top = 0;
    rect->width = 1;
    rect->height = tex_size->y;
}

void play_step_sound(wolf_t *wolf)
{
    bool moving = sfKeyboard_isKeyPressed(sfKeyZ) ||
        sfKeyboard_isKeyPressed(sfKeyS);

    if (!wolf->step_sound)
        return;
    if (moving && sfSound_getStatus(wolf->step_sound->sound) == sfStopped) {
        sfSound_play(wolf->step_sound->sound);
    }
    if (!moving)
        sfSound_stop(wolf->step_sound->sound);
}

void shader2(col_data_t *data)
{
    sfRectangleShape *col = sfRectangleShape_create();
    sfIntRect rect;
    texture_t *texture = NULL;
    sfVector2u tex_size;

    if (!col)
        return;
    sfRectangleShape_setSize(col, (sfVector2f){1.0f, data->wall_height});
    sfRectangleShape_setPosition(col, data->position);
    texture = setfml_texturefromname(data->setfml, "wall", false);
    sfRectangleShape_setFillColor(col, (sfColor){255, 255, 255, 255});
    if (texture) {
        tex_size = sfTexture_getSize(texture->texture);
        init_rect(&rect, data, &tex_size);
        sfRectangleShape_setTexture(col, texture->texture, sfFalse);
        sfRectangleShape_setTextureRect(col, rect);
    }
    sfRenderWindow_drawRectangleShape(data->setfml->window, col, NULL);
    sfRectangleShape_destroy(col);
}
