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

static void *find_entity_by_name(class_t *class,
    const char *name, bool return_node)
{
    entity_t *entity = NULL;

    if (!class || !class->entities || !name)
        return NULL;
    for (node_t *node = class->entities->head; node; node = node->next) {
        entity = (entity_t *)node->data;
        if (!entity)
            continue;
        if (!str_cmp(name, entity->name))
            return (return_node) ? (void *)node : (void *)entity;
    }
    return NULL;
}

entity_t *init_entity(classhandler_t *classhandler,
    class_t *class, const char *entityname)
{
    entity_t *entity = NULL;

    if (!classhandler || !class || !entityname)
        return NULL;
    entity = c_alloc(sizeof(entity_t), 1, classhandler->alloc);
    if (!entity)
        return NULL;
    entity->name = c_alloc(sizeof(char),
        str_len(entityname) + 1, classhandler->alloc);
    if (!entity->name) {
        c_free(entity, classhandler->alloc);
        return NULL;
    }
    entity->data = c_alloc(sizeof(char),
        class->byte_size, classhandler->alloc);
    if (!entity->data) {
        c_free(entity->name, classhandler->alloc);
        c_free(entity, classhandler->alloc);
    }
    return entity;
}

entity_t *classhandler_entitycreate(classhandler_t *classhandler,
    const char *classname, const char *entityname)
{
    class_t *class = find_class_by_name(classhandler, classname, false);
    entity_t *entity = init_entity(classhandler, class, entityname);
    node_t *entity_node = NULL;

    if (!entity)
        return NULL;
    str_cpy(entityname, entity->name);
    entity->class = class;
    entity_node = linkedlist_newnode((void *)entity);
    if (!entity_node) {
        c_free(entity->data, classhandler->alloc);
        c_free(entity->name, classhandler->alloc);
        c_free(entity, classhandler->alloc);
        return NULL;
    }
    linkedlist_inserthead(class->entities, entity_node);
    return entity;
}

size_t classhandler_entitydestroy(classhandler_t *classhandler,
    const char *classname, const char *entityname)
{
    class_t *class = find_class_by_name(classhandler, classname, false);
    node_t *node = find_entity_by_name(class, entityname, true);
    entity_t *entity = NULL;

    if (!node)
        return CLASSHANDLER_FAIL;
    entity = (entity_t *)node->data;
    if (entity->data)
        c_free(entity->data, classhandler->alloc);
    if (entity->name)
        c_free(entity->name, classhandler->alloc);
    linkedlist_remove(class->entities, node, true);
    return CLASSHANDLER_SUCC;
}

size_t classhandler_entitywrite(classhandler_t *classhandler,
    const char *classname, const char *entityname, void *data)
{
    class_t *class = find_class_by_name(classhandler, classname, false);
    entity_t *entity = find_entity_by_name(class, entityname, false);

    if (!entity)
        return CLASSHANDLER_FAIL;
    for (size_t i = 0; i < class->byte_size; i++)
        entity->data[i] = ((char *)data)[i];
    return CLASSHANDLER_SUCC;
}
