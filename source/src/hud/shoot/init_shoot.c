/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Private header between callback connexions.
*/

#include "include/shoot.h"

int init_weapon(wolf_t *wolf)
{
    wolf->weapon = c_alloc(sizeof(weapon_t), 1, wolf->alloc);
    wolf->weapon->weapon_sprite = sfSprite_create();
    wolf->weapon->weapon_texture = sfTexture_createFromFile(SHOOT_SPRITE, NULL);
    wolf->weapon->bullet = NULL;
    wolf->weapon->bullet_texture = sfTexture_createFromFile(BULLET_SPRITE,
        NULL);
    wolf->weapon->nb_bullet = WEAPON_MAX_AMMO;
    wolf->weapon->ammo_font = sfFont_createFromFile(AMMO_FONT);
    wolf->weapon->ammo_text = sfText_create();
    sfText_setFont(wolf->weapon->ammo_text, wolf->weapon->ammo_font);
    sfText_setCharacterSize(wolf->weapon->ammo_text, 18);
    sfText_setFillColor(wolf->weapon->ammo_text, sfWhite);
    sfText_setPosition(wolf->weapon->ammo_text, (sfVector2f){20, 680});
    sfSprite_setPosition(wolf->weapon->weapon_sprite, (sfVector2f){600, 600});
    sfSprite_setScale(wolf->weapon->weapon_sprite, (sfVector2f){1, 1});
    sfSprite_setTexture(wolf->weapon->weapon_sprite,
        wolf->weapon->weapon_texture, sfTrue);
    return 0;
}
