/*
** EPITECH PROJECT, 2025
** setfml
** File description:
** Tired of initializing csfml? Up your workflow with this simple wrapper.
*/

#include "./setfml.h"

static bool insert_func(node_t *toinsert, node_t *comp)
{
    function_t *n_before = NULL;
    function_t *n_insert = NULL;

    if (!toinsert || !comp)
        return true;
    n_insert = (function_t *)toinsert->data;
    n_before = (function_t *)comp->data;
    if (n_insert->function == n_before->function)
        return true;
    return false;
}

node_t *setfml_nodefromfunc(setfml_t *setfml,
    char name[BUFF_FUNC_NAME], size_t event)
{
    function_t *func = NULL;

    for (node_t *n = setfml->functions[event]->head; n; n = n->next) {
        func = (function_t *)n->data;
        if (!str_cmp(func->name, name))
            return n;
    }
    return NULL;
}

static loop_t func_from_name(setfml_t *setfml, char name[BUFF_FUNC_NAME])
{
    node_t *node = NULL;

    for (loop_t event = 0; event < SETFML_LINKEDLIST_AMT; event++) {
        node = setfml_nodefromfunc(setfml, name, event);
        if (node)
            return event;
    }
    return (loop_t)SETFML_FAIL;
}

size_t setfml_add(setfml_t *setfml, setfml_func_comp_t *functions,
    char name[BUFF_FUNC_NAME], size_t event)
{
    function_t *function = c_alloc(sizeof(function_t), 1, setfml->alloc);
    node_t *node = linkedlist_newnode(function);

    if (!node || !function || event < 0 || event > SETFML_LINKEDLIST_AMT ||
        !functions || event >= SETFML_LINKEDLIST_AMT)
        return (size_t)SETFML_FAIL;
    function->function = functions->f_before;
    str_cpy(name, function->name);
    node->data = (void *)function;
    if (!functions->f_before) {
        if (linkedlist_inserthead(setfml->functions[event], node)
            == (size_t)SETFML_FAIL)
            return (size_t)SETFML_FAIL;
    } else {
        if (linkedlist_insert(setfml->functions[event], node, &insert_func)
            == (size_t)SETFML_FAIL)
            return (size_t)SETFML_FAIL;
    }
    function->function = (void *)functions->callback;
    return (size_t)SETFML_SUCC;
}

size_t setfml_del(setfml_t *setfml, char name[BUFF_FUNC_NAME])
{
    loop_t event = func_from_name(setfml, name);
    node_t *node = NULL;

    if (event == (loop_t)SETFML_FAIL)
        return (size_t)SETFML_FAIL;
    node = setfml_nodefromfunc(setfml, name, event);
    if (!node)
        return (size_t)SETFML_FAIL;
    linkedlist_remove(setfml->functions[event], node, false);
    return (size_t)SETFML_SUCC;
}

size_t setfml_pause(setfml_t *setfml, char name[BUFF_FUNC_NAME])
{
    loop_t event = func_from_name(setfml, name);
    node_t *node = NULL;
    function_t *function = NULL;

    if (event == (loop_t)SETFML_FAIL)
        return (size_t)SETFML_FAIL;
    node = setfml_nodefromfunc(setfml, name, event);
    if (!node)
        return (size_t)SETFML_FAIL;
    function = (function_t *)node->data;
    if (!function)
        return (size_t)SETFML_FAIL;
    function->paused = true;
    return (size_t)SETFML_SUCC;
}

size_t setfml_resume(setfml_t *setfml, char name[BUFF_FUNC_NAME])
{
    loop_t event = func_from_name(setfml, name);
    node_t *node = NULL;
    function_t *function = NULL;

    if (event == (loop_t)SETFML_FAIL)
        return (size_t)SETFML_FAIL;
    node = setfml_nodefromfunc(setfml, name, event);
    if (!node)
        return (size_t)SETFML_FAIL;
    function = (function_t *)node->data;
    if (!function)
        return (size_t)SETFML_FAIL;
    function->paused = false;
    return (size_t)SETFML_SUCC;
}
