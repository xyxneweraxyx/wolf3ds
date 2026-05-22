/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Raycast callbacks to setfml.
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

static void modify_raycast_data(raycast_t *ray, player_t *player)
{
    ray->origin.degree = player->rotation;
    ray->origin.origin.x = player->pos.x;
    ray->origin.origin.y = player->pos.z;
    ray->render.height = player->pos.y;
    ray->eye_height = (float)RAYCAST_HEIGHT_UNIT / 2.0f + player->pos.y;
}

static entity_t *get_player_entity(wolf_t *wolf)
{
    return classhandler_fetchentityname(wolf->classhandler,
        CLASS_PLAYERS, NULL);
}

static bool prepare_raycasts(wolf_t *wolf)
{
    entity_t *entity = get_player_entity(wolf);
    player_t *player = NULL;

    if (!entity || !entity->data ||
        !wolf->raycast || !wolf->object_raycast)
        return false;
    player = (player_t *)entity->data;
    modify_raycast_data(wolf->raycast, player);
    modify_raycast_data(wolf->object_raycast, player);
    return true;
}

static size_t raycast_function(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;

    (void)userdata;
    if (wolf->state != GAME_PLAY && wolf->state != GAME_PAUSED)
        return (size_t)SETFML_SUCC;
    if (!prepare_raycasts(wolf))
        return (size_t)SETFML_FAIL;
    draw_ceiling(wolf->raycast, setfml);
    draw_ground(wolf->raycast, setfml);
    raycast_raycast(wolf->raycast, setfml);
    draw_wall_tops(wolf->raycast, setfml);
    wolf->object_hit_count = 0;
    raycast_raycast(wolf->object_raycast, setfml);
    project_object_hits(wolf);
    draw_object_sprites(wolf, setfml);
    return (size_t)SETFML_SUCC;
}

static void set_wall_texture(sfRectangleShape *col, col_data_t *data)
{
    int shade = (255 - data->distance * 30);
    sfIntRect rect = {0};
    texture_t *texture = NULL;
    sfVector2u tex_size = {0};

    shade = (shade < 0) ? 0 : shade;
    texture = setfml_texturefromname(data->setfml, "wall", false);
    sfRectangleShape_setFillColor(col, (sfColor){shade, shade, shade, 255});
    if (!texture)
        return;
    tex_size = sfTexture_getSize(texture->texture);
    rect.left = (int)(data->face_x * tex_size.x);
    rect.top = 0;
    rect.width = 1;
    rect.height = (int)tex_size.y;
    sfRectangleShape_setTexture(col, texture->texture, sfFalse);
    sfRectangleShape_setTextureRect(col, rect);
}

void draw_wall(col_data_t *data)
{
    sfRectangleShape *col = sfRectangleShape_create();

    if (!col)
        return;
    sfRectangleShape_setSize(col, (sfVector2f){1.0f, data->wall_height});
    sfRectangleShape_setPosition(col, data->position);
    set_wall_texture(col, data);
    sfRenderWindow_drawRectangleShape(data->setfml->window, col, NULL);
    sfRectangleShape_destroy(col);
}

static raycast_t *create_wall_raycast(wolf_t *wolf, player_t *plr)
{
    raycast_t *raycast = raycast_create(wolf->map,
        &(ray_twod_t){plr->pos.x, plr->pos.z});

    if (!raycast)
        return NULL;
    raycast->on_draw = draw_wall;
    raycast->height_bottom = wolf->height_bottom;
    raycast->height_top = wolf->height_top;
    return raycast;
}

static raycast_t *create_object_raycast(wolf_t *wolf, player_t *plr)
{
    raycast_t *raycast = raycast_create(wolf->object_map,
        &(ray_twod_t){plr->pos.x, plr->pos.z});

    if (!raycast)
        return NULL;
    raycast->origin.collisions = "234";
    raycast->depth_source = wolf->raycast;
    raycast->on_draw = draw_object;
    return raycast;
}

int connect_raycasts(wolf_t *wolf)
{
    entity_t *entity = classhandler_fetchentityname(wolf->classhandler,
        CLASS_PLAYERS, NULL);
    player_t *plr = NULL;

    if (!entity || !entity->data)
        return WOLF_FAIL;
    plr = (player_t *)entity->data;
    wolf->raycast = create_wall_raycast(wolf, plr);
    wolf->object_raycast = create_object_raycast(wolf, plr);
    if (!wolf->raycast || !wolf->object_raycast)
        return WOLF_FAIL;
    wolf->raycast->flash_light = 1;
    setfml_add(wolf->setfml, &(setfml_func_comp_t){NULL, &raycast_function},
        "raycast", LOOP_DRAW);
    return WOLF_SUCC;
}
