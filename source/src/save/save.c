/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Save system - write functions
*/

#include "./../../include/menu.h"
#include "include/save.h"
#include "./../hud/life/include/life.h"

static int write_header(FILE *f)
{
    const char magic[4] = {'W', '3', 'D', 'S'};
    uint8_t version = SAVE_VERSION;

    if (fwrite(magic, 1, 4, f) != 4)
        return WOLF_FAIL;
    if (fwrite(&version, sizeof(uint8_t), 1, f) != 1)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

static int write_player(FILE *f, player_t *player)
{
    if (fwrite(&player->pos.x, sizeof(float), 1, f) != 1)
        return WOLF_FAIL;
    if (fwrite(&player->pos.y, sizeof(float), 1, f) != 1)
        return WOLF_FAIL;
    if (fwrite(&player->pos.z, sizeof(float), 1, f) != 1)
        return WOLF_FAIL;
    if (fwrite(&player->rotation, sizeof(float), 1, f) != 1)
        return WOLF_FAIL;
    if (fwrite(&player->health, sizeof(uint16_t), 1, f) != 1)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

static int write_weapon(FILE *f, wolf_t *wolf)
{
    if (fwrite(&wolf->weapon->nb_bullet, sizeof(int), 1, f) != 1)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

static int write_settings(FILE *f, wolf_t *wolf)
{
    int fullscreen = (int)wolf->menu->fullscreen;

    if (fwrite(&wolf->menu->volume, sizeof(int), 1, f) != 1)
        return WOLF_FAIL;
    if (fwrite(&wolf->menu->resolution_index, sizeof(int), 1, f) != 1)
        return WOLF_FAIL;
    if (fwrite(&fullscreen, sizeof(int), 1, f) != 1)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

static int write_life(FILE *f, wolf_t *wolf)
{
    if (fwrite(&wolf->life->current_life, sizeof(int), 1, f) != 1)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

static int write_all(FILE *f, wolf_t *wolf, player_t *player)
{
    if (write_header(f) == WOLF_FAIL)
        return WOLF_FAIL;
    if (write_player(f, player) == WOLF_FAIL)
        return WOLF_FAIL;
    if (write_weapon(f, wolf) == WOLF_FAIL)
        return WOLF_FAIL;
    if (write_settings(f, wolf) == WOLF_FAIL)
        return WOLF_FAIL;
    if (write_life(f, wolf) == WOLF_FAIL)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

int save_game(wolf_t *wolf)
{
    entity_t *entity = NULL;
    player_t *player = NULL;
    FILE *f = NULL;
    int result = WOLF_SUCC;

    if (!wolf || !wolf->menu || !wolf->life || !wolf->weapon)
        return WOLF_FAIL;
    entity = classhandler_fetchentityname(wolf->classhandler,
        CLASS_PLAYERS, NULL);
    if (!entity || !entity->data)
        return WOLF_FAIL;
    f = fopen(SAVE_PATH, "wb");
    if (!f)
        return WOLF_FAIL;
    player = (player_t *)entity->data;
    result = write_all(f, wolf, player);
    fclose(f);
    return result;
}