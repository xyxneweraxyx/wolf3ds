/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** life_init
*/

#include "include/mini_map.h"

static int test_x(wolf_t *wolf, int row, int column, int distance)
{
    sfVertexArray *vert;

    if (column <= wolf->map_2d->len_y && wolf->map[column][row] != 48 &&
        distance < 10) {
        vert = create_line(&wolf->map_2d->map_2d[row][column],
            &wolf->map_2d->map_2d[row][column + 1]);
        sfRenderWindow_drawVertexArray(wolf->setfml->window, vert, 0);
        sfVertexArray_destroy(vert);
        return 1;
    }
    return 0;
}

static void test_x_bis(wolf_t *wolf, int row, int column, int distance)
{
    sfVertexArray *vert;

    if (row <= wolf->map_2d->len_x && wolf->map[column][row] != 48 &&
        distance < 10) {
        vert = create_line(&wolf->map_2d->map_2d[row + 1][column],
            &wolf->map_2d->map_2d[row + 1][column + 1]);
        sfConvexShape_setPoint(wolf->map_2d->shape, 0,
            wolf->map_2d->map_2d[row][column]);
        sfConvexShape_setPoint(wolf->map_2d->shape, 1,
            wolf->map_2d->map_2d[row + 1][column]);
        sfConvexShape_setPoint(wolf->map_2d->shape, 2,
            wolf->map_2d->map_2d[row][column + 1]);
        sfConvexShape_setPoint(wolf->map_2d->shape, 3,
            wolf->map_2d->map_2d[row + 1][column + 1]);
        sfRenderWindow_drawConvexShape(wolf->setfml->window,
            wolf->map_2d->shape, 0);
        sfRenderWindow_drawVertexArray(wolf->setfml->window, vert, 0);
        sfVertexArray_destroy(vert);
    }
}

void handle_line_x(wolf_t *wolf, int row, int column, player_t *player)
{
    float distance_x = (row - player->pos.x);
    float distance_z = (column - player->pos.z);
    float distance = sqrt(distance_x * distance_x + distance_z * distance_z);

    test_x(wolf, row, column, distance);
    test_x_bis(wolf, row, column, distance);
    return;
}

static int test_y(wolf_t *wolf, int row, int column, int distance)
{
    sfVertexArray *vert;

    if (row <= wolf->map_2d->len_x && wolf->map[column][row] != 48 &&
        distance < 10) {
        vert = create_line(&wolf->map_2d->map_2d[row][column],
            &wolf->map_2d->map_2d[row + 1][column]);
        sfRenderWindow_drawVertexArray(wolf->setfml->window, vert, 0);
        sfVertexArray_destroy(vert);
        return 1;
    }
    return 0;
}

static void test_y_bis(wolf_t *wolf, int row, int column, int distance)
{
    sfVertexArray *vert;

    if (row <= wolf->map_2d->len_x && wolf->map[column][row] != 48 &&
        distance < 10) {
        vert = create_line(&wolf->map_2d->map_2d[row][column + 1],
            &wolf->map_2d->map_2d[row + 1][column + 1]);
        sfConvexShape_setPoint(wolf->map_2d->shape, 0,
            wolf->map_2d->map_2d[row][column]);
        sfConvexShape_setPoint(wolf->map_2d->shape, 2,
            wolf->map_2d->map_2d[row + 1][column]);
        sfConvexShape_setPoint(wolf->map_2d->shape, 1,
            wolf->map_2d->map_2d[row][column + 1]);
        sfConvexShape_setPoint(wolf->map_2d->shape, 3,
            wolf->map_2d->map_2d[row + 1][column + 1]);
        sfRenderWindow_drawConvexShape(wolf->setfml->window,
            wolf->map_2d->shape, 0);
        sfRenderWindow_drawVertexArray(wolf->setfml->window, vert, 0);
        sfVertexArray_destroy(vert);
    }
}

void handle_line_y(wolf_t *wolf, int row, int column, player_t *player)
{
    float distance_x = (row - player->pos.x);
    float distance_z = (column - player->pos.z);
    float distance = sqrt(distance_x * distance_x + distance_z * distance_z);

    test_y(wolf, row, column, distance);
    test_y_bis(wolf, row, column, distance);
}

void handle_line(wolf_t *wolf, int row, int column, player_t *player)
{
    handle_line_x(wolf, row, column, player);
    handle_line_y(wolf, row, column, player);
}

size_t draw_2d_map(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    entity_t *entity = NULL;

    (void)userdata;
    if (wolf->state == GAME_MENU)
        return 0;
    if (wolf->state == GAME_SETTINGS)
        return 0;
    if (wolf->state == GAME_CINEMATIC)
        return 0;
    entity = classhandler_fetchentityname(wolf->classhandler,
        CLASS_PLAYERS, NULL);
    for (int row = 0; row < wolf->map_2d->len_x; row++) {
        for (int column = 0; column < wolf->map_2d->len_y; column++) {
            handle_line(wolf, row, column, (player_t *)entity->data);
        }
        sfRenderWindow_drawCircleShape(wolf->setfml->window,
            wolf->map_2d->point, NULL);
    }
    return 0;
}
