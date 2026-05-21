/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Raycast object sprites.
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

struct sprite_draw_s {
    sfFloatRect bounds;
    sfVector2u tex_size;
    setfml_t *setfml;
    texture_t *texture;
};

static bool is_sprite_column_visible(wolf_t *wolf, size_t column,
    float distance)
{
    raycast_t *raycast = wolf->raycast;

    if (!raycast || !raycast->depth_buffer || column >= raycast->depth_width)
        return true;
    return distance <= raycast->depth_buffer[column];
}

static float clamp_sprite_height(float height)
{
    if (height < 8.0f)
        return 8.0f;
    if (height > 80.0f)
        return 80.0f;
    return height;
}

static sfVector2f get_sprite_size(struct object_sprite_s *sprite,
    texture_t *texture)
{
    sfVector2u tex_size = sfTexture_getSize(texture->texture);
    float height = 160.0f / sprite->distance;
    float ratio = (float)tex_size.x / (float)tex_size.y;

    if (sprite->distance <= 0.01f)
        height = 24.0f;
    height = clamp_sprite_height(height);
    return (sfVector2f){height * ratio, height};
}

static void clamp_sprite_columns(int *left, int *right, unsigned int width)
{
    if (*left < 0)
        *left = 0;
    if (*right >= (int)width)
        *right = (int)width - 1;
}

static int get_texture_column(struct sprite_draw_s *draw, int column)
{
    float offset = (float)column - draw->bounds.left;
    int texture_column = 0;

    texture_column = (int)(offset / draw->bounds.width *
        (float)draw->tex_size.x);
    if (texture_column < 0)
        return 0;
    if (texture_column >= (int)draw->tex_size.x)
        return (int)draw->tex_size.x - 1;
    return texture_column;
}

static void set_sprite_column(sfRectangleShape *rect,
    struct sprite_draw_s *draw, int column)
{
    sfIntRect tex_rect = {get_texture_column(draw, column), 0, 1,
        (int)draw->tex_size.y};

    sfRectangleShape_setSize(rect, (sfVector2f){1.0f, draw->bounds.height});
    sfRectangleShape_setPosition(rect,
        (sfVector2f){column, draw->bounds.top});
    sfRectangleShape_setTextureRect(rect, tex_rect);
}

static void draw_visible_columns(wolf_t *wolf, sfRectangleShape *rect,
    struct object_sprite_s *sprite, struct sprite_draw_s *draw)
{
    int left = (int)draw->bounds.left;
    int right = (int)(draw->bounds.left + draw->bounds.width);
    unsigned int width = sfRenderWindow_getSize(draw->setfml->window).x;

    clamp_sprite_columns(&left, &right, width);
    for (int column = left; column <= right; column++) {
        if (!is_sprite_column_visible(wolf, (size_t)column, sprite->distance))
            continue;
        set_sprite_column(rect, draw, column);
        sfRenderWindow_drawRectangleShape(draw->setfml->window, rect, NULL);
    }
}

static void set_draw_bounds(wolf_t *wolf, setfml_t *setfml,
    struct object_sprite_s *sprite, struct sprite_draw_s *draw)
{
    sfVector2u win_size = sfRenderWindow_getSize(setfml->window);
    sfVector2f size = get_sprite_size(sprite, draw->texture);
    raycast_t *raycast = wolf->object_raycast;
    float fov = raycast->render.degree * DEG_TO_RAD;
    float proj_dist = ((float)win_size.y / 2.0f) / tanf(fov / 2.0f);
    float wall_height = proj_dist / sprite->distance;
    float floor_y = 0;

    wall_height *= raycast->render.wall_height;
    floor_y = ((float)win_size.y + wall_height +
        (raycast->render.height / sprite->distance)) / 2.0f;
    draw->bounds.left = sprite->screen_x - size.x / 2.0f;
    draw->bounds.top = floor_y - size.y;
    draw->bounds.width = size.x;
    draw->bounds.height = size.y;
}

static void draw_sprite(wolf_t *wolf, setfml_t *setfml,
    texture_t *texture, struct object_sprite_s *sprite)
{
    sfRectangleShape *rect = sfRectangleShape_create();
    struct sprite_draw_s draw = {0};

    if (!rect)
        return;
    draw.setfml = setfml;
    draw.texture = texture;
    draw.tex_size = sfTexture_getSize(texture->texture);
    set_draw_bounds(wolf, setfml, sprite, &draw);
    sfRectangleShape_setTexture(rect, texture->texture, sfTrue);
    sfRectangleShape_setFillColor(rect, sfWhite);
    draw_visible_columns(wolf, rect, sprite, &draw);
    sfRectangleShape_destroy(rect);
}

void draw_object_sprites(wolf_t *wolf, setfml_t *setfml)
{
    texture_t *texture = NULL;

    if (!wolf || !setfml || !wolf->object_sprites)
        return;
    texture = setfml_texturefromname(setfml, "mushroom", false);
    if (!texture || !texture->texture)
        return;
    for (size_t i = 0; i < wolf->object_sprite_count; i++)
        draw_sprite(wolf, setfml, texture, &wolf->object_sprites[i]);
}
