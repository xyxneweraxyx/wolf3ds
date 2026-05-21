/*
** EPITECH PROJECT, 2025
** classhandler
** File description:
** Create and manage classes and entities with this simple lib.
*/

#include "./include/classhandler.h"

static void *find_class_by_name(classhandler_t *classhandler,
    const char *name, bool return_node)
{
    class_t *class = NULL;

    if (!classhandler || !classhandler->classes || !name)
        return NULL;
    for (node_t *node = classhandler->classes->head; node; node = node->next) {
        class = (class_t *)node->data;
        if (!class)
            continue;
        if (!str_cmp(name, class->name))
            return (return_node) ? (void *)node : (void *)class;
    }
    return NULL;
}

static class_t *ini_class(classhandler_t *classhandler,
    size_t byte_size, const char *name)
{
    class_t *class = c_alloc(sizeof(class_t), 1, classhandler->alloc);

    if (!class)
        return NULL;
    class->name = c_alloc(sizeof(char),
        str_len(name) + 1, classhandler->alloc);
    if (!class->name) {
        c_free(class, classhandler->alloc);
        return NULL;
    }
    str_cpy(name, class->name);
    class->entities = linkedlist_ini();
    if (!class->entities) {
        c_free(class->name, classhandler->alloc);
        c_free(class, classhandler->alloc);
        return NULL;
    }
    class->byte_size = byte_size;
    classhandler->class_amount += 1;
    return class;
}

class_t *classhandler_classcreate(classhandler_t *classhandler,
    size_t byte_size, const char *name)
{
    class_t *class = NULL;
    node_t *node = NULL;

    if (!classhandler || byte_size <= 0 || !name)
        return NULL;
    class = ini_class(classhandler, byte_size, name);
    if (!class)
        return NULL;
    node = linkedlist_newnode((void *)class);
    if (!node) {
        linkedlist_destroy(class->entities, false);
        c_free(class->name, classhandler->alloc);
        c_free(class, classhandler->alloc);
        return NULL;
    }
    linkedlist_inserthead(classhandler->classes, node);
    return class;
}

static void free_entities(classhandler_t *classhandler, class_t *class)
{
    entity_t *entity = NULL;

    for (node_t *node = class->entities->head; node; node = node->next) {
        if (!node->data)
            continue;
        entity = (entity_t *)node->data;
        c_free(entity->data, classhandler->alloc);
        c_free(entity->name, classhandler->alloc);
        c_free(entity, classhandler->alloc);
    }
}

size_t classhandler_classdestroy(classhandler_t *classhandler,
    const char *name)
{
    node_t *node = find_class_by_name(classhandler, name, true);
    class_t *class = NULL;

    if (!node || !node->data)
        return CLASSHANDLER_FAIL;
    class = (class_t *)node->data;
    c_free(class->name, classhandler->alloc);
    linkedlist_remove(classhandler->classes, node, false);
    if (!class->entities) {
        c_free(class, classhandler->alloc);
        return CLASSHANDLER_SUCC;
    }
    free_entities(classhandler, class);
    linkedlist_destroy(class->entities, false);
    c_free(class, classhandler->alloc);
    return CLASSHANDLER_SUCC;
}
