/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Save system - read functions
*/

#include "./../../include/menu.h"
#include "include/save.h"
#include "life.h"

static int read_header(FILE *f)
{
    char magic[4] = {0};
    uint8_t version = 0;

    if (fread(magic, 1, 4, f) != 4)
        return WOLF_FAIL;
    if (magic[0] != 'W' || magic[1] != '3'
        || magic[2] != 'D' || magic[3] != 'S')
        return WOLF_FAIL;
    if (fread(&version, sizeof(uint8_t), 1, f) != 1)
        return WOLF_FAIL;
    if (version != SAVE_VERSION)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

static int read_player(FILE *f, player_t *player)
{
    if (fread(&player->pos.x, sizeof(float), 1, f) != 1)
        return WOLF_FAIL;
    if (fread(&player->pos.y, sizeof(float), 1, f) != 1)
        return WOLF_FAIL;
    if (fread(&player->pos.z, sizeof(float), 1, f) != 1)
        return WOLF_FAIL;
    if (fread(&player->rotation, sizeof(float), 1, f) != 1)
        return WOLF_FAIL;
    if (fread(&player->health, sizeof(uint16_t), 1, f) != 1)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

static int read_weapon(FILE *f, wolf_t *wolf)
{
    if (fread(&wolf->weapon->nb_bullet, sizeof(int), 1, f) != 1)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

static int read_settings(FILE *f, wolf_t *wolf)
{
    int fullscreen = 0;

    if (fread(&wolf->menu->volume, sizeof(int), 1, f) != 1)
        return WOLF_FAIL;
    if (fread(&wolf->menu->resolution_index, sizeof(int), 1, f) != 1)
        return WOLF_FAIL;
    if (fread(&fullscreen, sizeof(int), 1, f) != 1)
        return WOLF_FAIL;
    wolf->menu->fullscreen = (bool)fullscreen;
    return WOLF_SUCC;
}

static int read_life(FILE *f, wolf_t *wolf)
{
    if (fread(&wolf->life->current_life, sizeof(int), 1, f) != 1)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

static int read_all(FILE *f, wolf_t *wolf, player_t *player)
{
    if (read_header(f) == WOLF_FAIL)
        return WOLF_FAIL;
    if (read_player(f, player) == WOLF_FAIL)
        return WOLF_FAIL;
    if (read_weapon(f, wolf) == WOLF_FAIL)
        return WOLF_FAIL;
    if (read_settings(f, wolf) == WOLF_FAIL)
        return WOLF_FAIL;
    if (read_life(f, wolf) == WOLF_FAIL)
        return WOLF_FAIL;
    return WOLF_SUCC;
}

static void apply_loaded(wolf_t *wolf)
{
    float ratio = (float)wolf->menu->volume / 100.0f;

    if (wolf->step_sound && wolf->step_sound->sound)
        sfSound_setVolume(wolf->step_sound->sound,
            SFML_STEP_VOLUME * ratio);
    if (wolf->shoot_sound && wolf->shoot_sound->sound)
        sfSound_setVolume(wolf->shoot_sound->sound,
            SFML_SHOOT_VOLUME * ratio);
    update_life(0, wolf, 0);
}

int load_game(wolf_t *wolf)
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
    f = fopen(SAVE_PATH, "rb");
    if (!f)
        return WOLF_FAIL;
    player = (player_t *)entity->data;
    result = read_all(f, wolf, player);
    fclose(f);
    if (result == WOLF_SUCC)
        apply_loaded(wolf);
    return result;
}
