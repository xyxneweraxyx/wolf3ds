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

static size_t count_entities_comp(classhandler_t *classhandler,
    const char *classname, bool (*comp)(entity_t *entity))
{
    class_t *class = find_class_by_name(classhandler, classname, false);
    entity_t *entity = NULL;
    size_t total = 0;

    if (!class || !comp)
        return (size_t)-1;
    for (node_t *node = class->entities->head; node; node = node->next) {
        if (!node->data)
            continue;
        entity = (entity_t *)node->data;
        if (comp(entity))
            total += 1;
    }
    return total;
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

entity_t *classhandler_fetchentityname(classhandler_t *classhandler,
    const char *classname, const char *entityname)
{
    class_t *class = find_class_by_name(classhandler, classname, false);
    entity_t *entity = find_entity_by_name(class, entityname, false);

    if (!entityname && class->entities->head)
        return (entity_t *)class->entities->head->data;
    return entity;
}

entity_t *classhandler_fetchentitycomp(classhandler_t *classhandler,
    const char *classname, bool (*comp)(entity_t *entity))
{
    class_t *class = find_class_by_name(classhandler, classname, false);
    entity_t *entity = NULL;

    if (!class || !comp)
        return NULL;
    for (node_t *node = class->entities->head; node; node = node->next) {
        if (!node->data)
            continue;
        entity = (entity_t *)node->data;
        if (comp(entity))
            return entity;
    }
    return NULL;
}

static void write_entities(entity_t **result,
    class_t *class, bool (*comp)(entity_t *entity))
{
    entity_t *entity = NULL;
    size_t written = 0;

    for (node_t *node = class->entities->head; node; node = node->next) {
        if (!node->data)
            continue;
        entity = (entity_t *)node->data;
        if (comp(entity)) {
            result[written] = entity;
            written += 1;
        }
    }
}

entity_t **classhandler_fetchentitiescomp(classhandler_t *classhandler,
    const char *classname, bool (*comp)(entity_t *entity))
{
    class_t *class = find_class_by_name(classhandler, classname, false);
    size_t total = 0;
    entity_t **result = NULL;

    if (!class || !comp)
        return NULL;
    total = count_entities_comp(classhandler, classname, comp);
    if (total == (size_t)-1)
        return NULL;
    result = malloc(sizeof(entity_t) * total + 1);
    if (!result)
        return NULL;
    result[total] = NULL;
    write_entities(result, class, comp);
    return result;
}

entity_t *classhandler_fetchentityscore(classhandler_t *classhandler,
    const char *classname, float (*score)(entity_t *entity))
{
    class_t *class = find_class_by_name(classhandler, classname, false);
    float highest = 0;
    float current = 0;
    entity_t *best_entity = 0;
    entity_t *entity = 0;

    if (!class)
        return NULL;
    for (node_t *node = class->entities->head; node; node = node->next) {
        if (!node->data)
            continue;
        entity = (entity_t *)node->data;
        current = score(entity);
        if (current > highest) {
            highest = current;
            best_entity = entity;
        }
    }
    return best_entity;
}
