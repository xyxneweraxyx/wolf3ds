/*
** EPITECH PROJECT, 2025
** setfml
** File description:
** Tired of initializing csfml? Up your workflow with this simple wrapper.
*/

#include "./setfml.h"

void *setfml_spritefromname(setfml_t *setfml,
    char name[BUFF_SPRITE_NAME], bool return_node)
{
    sprite_t *sprite = NULL;

    for (node_t *node = setfml->sprites->head; node; node = node->next) {
        sprite = (sprite_t *)node->data;
        if (!str_cmp(sprite->name, name) && return_node)
            return (void *)node;
        if (!str_cmp(sprite->name, name) && !return_node)
            return (void *)node->data;
    }
    return NULL;
}

size_t setfml_spriteadd(setfml_t *setfml, char name[BUFF_SPRITE_NAME],
    texture_t *texture)
{
    sprite_t *sprite = c_alloc(sizeof(sprite_t), 1, setfml->alloc);
    node_t *node = linkedlist_newnode(sprite);
    sfVector2u windowsize = sfRenderWindow_getSize(setfml->window);

    if (!sprite || !node)
        return (size_t)SETFML_FAIL;
    str_cpy(name, sprite->name);
    sprite->texture = texture;
    sprite->sprite = sfSprite_create();
    sprite->original_win_x = (size_t)windowsize.x;
    sprite->original_win_y = (size_t)windowsize.y;
    if (!sprite->sprite)
        return (size_t)SETFML_FAIL;
    setfml_spritechangetexture(setfml, sprite, texture->name);
    node->data = (void *)sprite;
    if (linkedlist_inserthead(setfml->sprites, node) == (size_t)SETFML_FAIL)
        return (size_t)SETFML_FAIL;
    return (size_t)SETFML_SUCC;
}

size_t setfml_spritedel(setfml_t *setfml, char name[BUFF_SPRITE_NAME])
{
    node_t *node = NULL;
    sprite_t *sprite = NULL;

    if (!setfml || !setfml->sprites)
        return (size_t)SETFML_FAIL;
    node = setfml_spritefromname(setfml, name, true);
    if (!node)
        return (size_t)SETFML_FAIL;
    sprite = (sprite_t *)node->data;
    if (sprite->sprite)
        sfSprite_destroy(sprite->sprite);
    linkedlist_remove(setfml->sprites, node, false);
    return (size_t)SETFML_SUCC;
}

size_t setfml_spritechangetexture(setfml_t *setfml, sprite_t *sprite,
    char texture_name[BUFF_TEXT_NAME])
{
    node_t *node = setfml_texturefromname(setfml, texture_name, true);
    texture_t *texture = NULL;

    if (!node)
        return (size_t)SETFML_FAIL;
    texture = (texture_t *)node->data;
    sprite->texture = texture;
    sfSprite_setTexture(sprite->sprite, texture->texture, sfTrue);
    return (size_t)SETFML_SUCC;
}
