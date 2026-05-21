/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Private header between callback connexions.
*/

#define _GNU_SOURCE
#include "include/shoot.h"

static float weapon_anim_offset(wolf_t *wolf)
{
    struct timespec now;
    long elapsed_ns;
    float t;

    clock_gettime(CLOCK_MONOTONIC, &now);
    elapsed_ns = (now.tv_sec - wolf->weapon->start.tv_sec) * 1000000000LL
        + (now.tv_nsec - wolf->weapon->start.tv_nsec);
    if (elapsed_ns >= 250000000LL)
        return 0;
    t = elapsed_ns / 250000000.0f;
    return -30 * (t - 0.4) + 0.05;
}

static void project_bullet(wolf_t *wolf, bullet_t *bullet)
{
    sfVector2u win = sfRenderWindow_getSize(wolf->setfml->window);
    player_t *player = (player_t *)classhandler_fetchentityname(
        wolf->classhandler, CLASS_PLAYERS, NULL)->data;
    float cam = player->rotation * DEG_TO_RAD;
    float dx = bullet->pos_x - player->pos.x;
    float dz = bullet->pos_y - player->pos.z;
    float depth = dx * cosf(cam) + dz * sinf(cam);
    float side = -dx * sinf(cam) + dz * cosf(cam);
    float fov = wolf->raycast->render.degree * DEG_TO_RAD;
    float proj = (win.x / 2.0f) / tanf(fov / 2.0f);
    float scale = 0.7f / depth;
    float screen_x = (win.x / 2.0f) + (side / depth) * proj;
    float screen_y = win.y / 2.0f + (float)win.y * 0.4f / depth;

    if (depth <= 0.05f)
        return;
    if (screen_y > (float)win.y)
        screen_y = (float)win.y;
    sfSprite_setScale(bullet->sprite, (sfVector2f){scale, scale});
    sfSprite_setPosition(bullet->sprite, (sfVector2f){screen_x, screen_y});
}

void draw_bullet(wolf_t *wolf)
{
    for (bullet_t *tmp = wolf->weapon->bullet; tmp != NULL; tmp = tmp->next) {
        project_bullet(wolf, tmp);
        sfRenderWindow_drawSprite(wolf->setfml->window, tmp->sprite, NULL);
    }
}

static void set_ammo_string(char *ammo_str, int ammo)
{
    str_cpy("AMMO: ", ammo_str);
    if (ammo >= 10) {
        ammo_str[6] = ammo / 10 + '0';
        ammo_str[7] = ammo % 10 + '0';
        ammo_str[8] = '\0';
        return;
    }
    ammo_str[6] = ammo + '0';
    ammo_str[7] = '\0';
}

void draw_nb_bullet(wolf_t *wolf, setfml_t *setfml)
{
    char ammo_str[16];

    set_ammo_string(ammo_str, wolf->weapon->nb_bullet);
    sfText_setString(wolf->weapon->ammo_text, ammo_str);
    sfRenderWindow_drawText(setfml->window, wolf->weapon->ammo_text, NULL);
}

static unsigned int clamp_text(unsigned int value,
    unsigned int min, unsigned int max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

static void set_weapon_layout(wolf_t *wolf, setfml_t *setfml)
{
    sfVector2u win = sfRenderWindow_getSize(setfml->window);
    sfFloatRect rect = sfSprite_getLocalBounds(wolf->weapon->weapon_sprite);
    float scale = (float)win.y / 1080.0f;
    sfVector2f pos = {0, 0};

    if (scale < 0.45f)
        scale = 0.45f;
    if (scale > 1.20f)
        scale = 1.20f;
    pos.x = (float)win.x / 2.0f - rect.width * scale / 2.0f;
    pos.y = (float)win.y - rect.height * scale - 47.0f * scale;
    sfSprite_setScale(wolf->weapon->weapon_sprite,
        (sfVector2f){scale, scale});
    sfSprite_setPosition(wolf->weapon->weapon_sprite, pos);
    sfText_setCharacterSize(wolf->weapon->ammo_text,
        clamp_text(win.y / 60, 12, 22));
    sfText_setPosition(wolf->weapon->ammo_text,
        (sfVector2f){win.x * 0.02f, win.y - 55.0f});
}

size_t draw_weapon(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    sfVector2f base;

    (void)userdata;
    if (wolf->state == GAME_MENU)
        return 0;
    if (wolf->state == GAME_SETTINGS)
        return 0;
    if (wolf->state == GAME_CINEMATIC)
        return 0;
    set_weapon_layout(wolf, setfml);
    base = sfSprite_getPosition(wolf->weapon->weapon_sprite);
    sfSprite_setPosition(wolf->weapon->weapon_sprite,
        (sfVector2f){base.x, base.y + weapon_anim_offset(wolf)});
    sfRenderWindow_drawSprite(setfml->window,
        wolf->weapon->weapon_sprite, NULL);
    sfSprite_setPosition(wolf->weapon->weapon_sprite, base);
    draw_nb_bullet(wolf, setfml);
    draw_bullet(wolf);
    return 0;
}
