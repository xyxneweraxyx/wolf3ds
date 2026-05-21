/*
** EPITECH PROJECT, 2025
** classhandler
** File description:
** Create and manage classes and entities with this simple lib.
*/

#include "./include/classhandler.h"

classhandler_t *classhandler_init(void)
{
    c_alloc_t *alloc = c_ini((uint16_t)50);
    classhandler_t *classhandler = NULL;

    if (!alloc)
        return NULL;
    classhandler = c_alloc(sizeof(classhandler_t), 1, alloc);
    if (!classhandler)
        return NULL;
    classhandler->alloc = alloc;
    classhandler->classes = linkedlist_ini();
    if (!classhandler->classes) {
        c_delete(alloc, true);
        return NULL;
    }
    return classhandler;
}

static void free_entities(classhandler_t *classhandler, class_t *class)
{
    entity_t *entity = NULL;

    for (node_t *node = class->entities->head; node; node = node->next) {
        if (!node->data)
            continue;
        entity = (entity_t *)node->data;
        if (entity->data)
            c_free(entity->data, classhandler->alloc);
        if (entity->name)
            c_free(entity->name, classhandler->alloc);
        c_free(entity, classhandler->alloc);
    }
}

static void free_classes(classhandler_t *classhandler, c_alloc_t *alloc)
{
    class_t *class = NULL;

    for (node_t *node = classhandler->classes->head;
        node;
        node = node->next) {
        if (!node->data)
            continue;
        class = (class_t *)node->data;
        if (class->entities) {
            free_entities(classhandler, class);
            linkedlist_destroy(class->entities, false);
        }
        if (class->name)
            c_free(class->name, alloc);
        c_free(class, classhandler->alloc);
    }
}

void classhandler_destroy(classhandler_t *classhandler)
{
    c_alloc_t *alloc = NULL;

    if (!classhandler)
        return;
    alloc = classhandler->alloc;
    if (!classhandler->classes) {
        c_delete(alloc, true);
        return;
    }
    free_classes(classhandler, alloc);
    linkedlist_destroy(classhandler->classes, false);
    c_free(classhandler, alloc);
    c_delete(alloc, false);
}
