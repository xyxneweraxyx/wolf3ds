/*
** EPITECH PROJECT, 2025
** setfml
** File description:
** Tired of initializing csfml? Up your workflow with this simple wrapper.
*/

#include "./setfml.h"

void *setfml_texturefromname(setfml_t *setfml,
    char name[BUFF_TEXT_NAME], bool return_node)
{
    texture_t *texture = NULL;

    for (node_t *node = setfml->textures->head; node; node = node->next) {
        texture = (texture_t *)node->data;
        if (!str_cmp(texture->name, name) && return_node)
            return (void *)node;
        if (!str_cmp(texture->name, name) && !return_node)
            return (void *)node->data;
    }
    return NULL;
}

size_t setfml_textureadd(setfml_t *setfml, char name[BUFF_TEXT_NAME],
    char path[BUFF_TEXT_PATH])
{
    texture_t *texture = c_alloc(sizeof(texture_t), 1, setfml->alloc);
    node_t *node = linkedlist_newnode(texture);

    if (!texture || !node)
        return (size_t)SETFML_FAIL;
    str_cpy(name, texture->name);
    str_cpy(path, texture->path);
    texture->texture = sfTexture_createFromFile(path, NULL);
    if (!texture->texture)
        return (size_t)SETFML_FAIL;
    node->data = (void *)texture;
    if (linkedlist_inserthead(setfml->textures, node) == (size_t)SETFML_FAIL)
        return (size_t)SETFML_FAIL;
    return (size_t)SETFML_SUCC;
}

size_t setfml_texturedel(setfml_t *setfml, char name[BUFF_TEXT_NAME])
{
    node_t *node = NULL;
    texture_t *texture = NULL;

    if (!setfml || !setfml->textures)
        return (size_t)SETFML_FAIL;
    node = setfml_texturefromname(setfml, name, true);
    if (!node)
        return (size_t)SETFML_FAIL;
    texture = (texture_t *)node->data;
    if (texture->texture)
        sfTexture_destroy(texture->texture);
    linkedlist_remove(setfml->textures, node, false);
    return (size_t)SETFML_SUCC;
}
