/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Ground, ceiling and wall top rendering.
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

static void set_ground_camera(ground_draw_t *draw)
{
    float angle = draw->raycast->origin.degree * DEG_TO_RAD;
    float fov = draw->raycast->render.degree * DEG_TO_RAD;
    float plane_size = tanf(fov / 2.0f);

    draw->direction.x = cosf(angle);
    draw->direction.y = sinf(angle);
    draw->plane.x = -draw->direction.y * plane_size;
    draw->plane.y = draw->direction.x * plane_size;
}

static void append_ground_vertex(ground_draw_t *draw, sfVector2f *position,
    sfVector2f *floor_pos, sfColor *color)
{
    sfVertex vertex = {0};

    vertex.position = *position;
    vertex.texCoords.x = floor_pos->x * draw->tex_size.x;
    vertex.texCoords.y = floor_pos->y * draw->tex_size.y;
    vertex.color = *color;
    sfVertexArray_append(draw->vertices, vertex);
}

static void append_ground_row(ground_draw_t *draw, ground_row_t *row)
{
    int shade = 255 - (int)(row->distance * 12.0f);
    sfColor color = {0};
    sfVector2f pos = {0};

    if (shade < 45) shade = 45;
    if (shade > 255) shade = 255;
    color = (sfColor){shade, shade, shade, 255};
    pos = (sfVector2f){0.0f, (float)row->y};
    append_ground_vertex(draw, &pos, &row->floor_left, &color);
    pos = (sfVector2f){draw->win_size.x, (float)row->y};
    append_ground_vertex(draw, &pos, &row->floor_right, &color);
    pos = (sfVector2f){draw->win_size.x, (float)(row->y + 1)};
    append_ground_vertex(draw, &pos, &row->floor_right, &color);
    pos = (sfVector2f){0.0f, (float)(row->y + 1)};
    append_ground_vertex(draw, &pos, &row->floor_left, &color);
}

void set_ground_row(ground_draw_t *draw, ground_row_t *row)
{
    row->distance = draw->camera_height / (row->y + 0.5f - draw->center_y);
    row->floor_left.x = draw->raycast->origin.origin.x +
        row->distance * (draw->direction.x - draw->plane.x);
    row->floor_left.y = draw->raycast->origin.origin.y +
        row->distance * (draw->direction.y - draw->plane.y);
    row->floor_right.x = draw->raycast->origin.origin.x +
        row->distance * (draw->direction.x + draw->plane.x);
    row->floor_right.y = draw->raycast->origin.origin.y +
        row->distance * (draw->direction.y + draw->plane.y);
}

static void init_ground_draw(ground_draw_t *draw)
{
    float fov = draw->raycast->render.degree * DEG_TO_RAD;
    float proj_dist = 0;

    draw->win_size = sfRenderWindow_getSize(draw->setfml->window);
    draw->tex_size = sfTexture_getSize(draw->texture->texture);
    proj_dist = ((float)draw->win_size.x / 2.0f) / tanf(fov / 2.0f);
    draw->center_y = (float)draw->win_size.y / 2.0f;
    draw->camera_height = proj_dist * draw->raycast->eye_height
        / (float)RAYCAST_HEIGHT_UNIT;
    sfTexture_setRepeated(draw->texture->texture, sfTrue);
    sfVertexArray_setPrimitiveType(draw->vertices, sfQuads);
    set_ground_camera(draw);
    draw->map_h = 0;
    while (draw->raycast->origin.map[draw->map_h])
        draw->map_h++;
    draw->map_w = 0;
    if (draw->map_h > 0)
        while (draw->raycast->origin.map[0][draw->map_w])
            draw->map_w++;
}

void draw_ground(raycast_t *raycast, setfml_t *setfml)
{
    ground_draw_t draw = {0};
    ground_row_t row = {0};
    sfRenderStates states = {sfBlendAlpha, sfTransform_Identity, NULL, NULL};

    if (!raycast || !setfml || !setfml->window)
        return;
    draw.raycast = raycast;
    draw.setfml = setfml;
    draw.texture = setfml_texturefromname(setfml, "ground", false);
    if (!draw.texture || !draw.texture->texture)
        return;
    draw.vertices = sfVertexArray_create();
    if (!draw.vertices)
        return;
    init_ground_draw(&draw);
    for (row.y = (int)draw.center_y; row.y < (int)draw.win_size.y; row.y++) {
        set_ground_row(&draw, &row);
        append_ground_row(&draw, &row);
    }
    states.texture = draw.texture->texture;
    sfRenderWindow_drawVertexArray(draw.setfml->window, draw.vertices, &states);
    sfVertexArray_destroy(draw.vertices);
}

void draw_ceiling(raycast_t *raycast, setfml_t *setfml)
{
    sfRectangleShape *rect = NULL;
    sfVector2u win = {0};

    if (!raycast || !setfml || !setfml->window)
        return;
    win = sfRenderWindow_getSize(setfml->window);
    rect = sfRectangleShape_create();
    if (!rect)
        return;
    sfRectangleShape_setPosition(rect, (sfVector2f){0.0f, 0.0f});
    sfRectangleShape_setSize(rect,
        (sfVector2f){(float)win.x, (float)win.y / 2.0f});
    sfRectangleShape_setFillColor(rect, (sfColor){20, 20, 40, 255});
    sfRenderWindow_drawRectangleShape(setfml->window, rect, NULL);
    sfRectangleShape_destroy(rect);
}

void draw_wall_tops(raycast_t *raycast, setfml_t *setfml)
{
    ground_draw_t draw = {0};
    sfRenderStates states = {sfBlendAlpha, sfTransform_Identity, NULL, NULL};

    if (!raycast || !setfml || !setfml->window || !raycast->height_top)
        return;
    draw.raycast = raycast;
    draw.setfml = setfml;
    draw.texture = setfml_texturefromname(setfml, "ground", false);
    if (!draw.texture || !draw.texture->texture)
        return;
    draw.vertices = sfVertexArray_create();
    if (!draw.vertices)
        return;
    init_ground_draw(&draw);
    for (uint8_t h = 1; (float)h < raycast->eye_height; h++)
        tops_fill_height(&draw, h);
    states.texture = draw.texture->texture;
    sfRenderWindow_drawVertexArray(draw.setfml->window, draw.vertices, &states);
    sfVertexArray_destroy(draw.vertices);
}