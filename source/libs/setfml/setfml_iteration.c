/*
** EPITECH PROJECT, 2025
** setfml
** File description:
** Tired of initializing csfml? Up your workflow with this simple wrapper.
*/

#include "./setfml.h"

static void setfml_settime(setfml_t *setfml)
{
    clock_gettime(CLOCK_MONOTONIC, &(setfml->state.time_elapsed.data));
    clock_gettime(CLOCK_MONOTONIC, &(setfml->state.time_elapsed.draw));
    clock_gettime(CLOCK_MONOTONIC, &(setfml->state.time_elapsed.event));
    clock_gettime(CLOCK_MONOTONIC, &(setfml->state.time_elapsed.render));
    setfml->state.to_exec.data = false;
    setfml->state.to_exec.draw = false;
    setfml->state.to_exec.event = false;
    setfml->state.to_exec.render = false;
}

static void setfml_updateloop(setfml_t *setfml)
{
    if (setfml->state.to_exec.data)
        clock_gettime(CLOCK_MONOTONIC, &(setfml->state.last_exec.data));
    if (setfml->state.to_exec.draw)
        clock_gettime(CLOCK_MONOTONIC, &(setfml->state.last_exec.draw));
    if (setfml->state.to_exec.render)
        clock_gettime(CLOCK_MONOTONIC, &(setfml->state.last_exec.render));
    if (setfml->state.to_exec.event)
        clock_gettime(CLOCK_MONOTONIC, &(setfml->state.last_exec.event));
}

static void setfml_updatetime(setfml_t *setfml)
{
    time_elapsed_t t_el = setfml->state.time_elapsed;
    time_elapsed_t l_ex = setfml->state.last_exec;
    long time_elapsed_data = (t_el.data.tv_sec - l_ex.data.tv_sec) *
        SEC_TO_NANO + (t_el.data.tv_nsec - l_ex.data.tv_nsec);
    long time_elapsed_draw = (t_el.draw.tv_sec - l_ex.draw.tv_sec) *
        SEC_TO_NANO + (t_el.draw.tv_nsec - l_ex.draw.tv_nsec);
    long time_elapsed_event = (t_el.event.tv_sec - l_ex.event.tv_sec) *
        SEC_TO_NANO + (t_el.event.tv_nsec - l_ex.event.tv_nsec);
    long time_elapsed_render = (t_el.render.tv_sec - l_ex.render.tv_sec) *
        SEC_TO_NANO + (t_el.render.tv_nsec - l_ex.render.tv_nsec);

    if ((size_t)time_elapsed_data > setfml->params.time.data)
        setfml->state.to_exec.data = true;
    if ((size_t)time_elapsed_draw > setfml->params.time.draw)
        setfml->state.to_exec.draw = true;
    if ((size_t)time_elapsed_render > setfml->params.time.render)
        setfml->state.to_exec.render = true;
    if ((size_t)time_elapsed_event > setfml->params.time.event)
        setfml->state.to_exec.event = true;
}

static void setfml_execcallback(setfml_t *setfml, linkedlist_t *linkedlist)
{
    node_t *node = NULL;
    function_t *func = NULL;

    if (!linkedlist || !setfml || !linkedlist->head || setfml->state.is_closing)
        return;
    for (node = linkedlist->head; node; node = node->next) {
        func = (function_t *)node->data;
        if (func->paused)
            continue;
        func->return_code = func->function(setfml, func->userdata);
        if (func->return_code == (size_t)SETFML_WINDOWCLOSING)
            setfml->state.is_closing = true;
    }
}

static void setfml_execsingleevent(setfml_t *setfml, sfEventType event)
{
    if (setfml->event.type != event)
        return;
    setfml_execcallback(setfml, setfml->functions[event]);
}

static void setfml_execevents(setfml_t *setfml)
{
    while (sfRenderWindow_pollEvent(setfml->window, &(setfml->event))) {
        for (size_t i = 0; i < (size_t)SETFML_EVT_AMT; i++)
            setfml_execsingleevent(setfml, (sfEventType)i);
    }
}

static void setfml_handlecallbacks(setfml_t *setfml)
{
    if (setfml->state.to_exec.event) {
        setfml->state.to_exec.event = false;
        setfml_execevents(setfml);
    }
    if (setfml->state.to_exec.data) {
        setfml->state.to_exec.data = false;
        setfml_execcallback(setfml, setfml->functions[LOOP_DATA]);
    }
    if (setfml->state.to_exec.render) {
        setfml->state.to_exec.render = false;
        setfml_execcallback(setfml, setfml->functions[LOOP_RENDER]);
    }
    if (setfml->state.to_exec.draw) {
        setfml->state.to_exec.draw = false;
        sfRenderWindow_clear(setfml->window, sfBlack);
        setfml_execcallback(setfml, setfml->functions[LOOP_DRAW]);
    }
}

void setfml_iteration(setfml_t *setfml)
{
    setfml_settime(setfml);
    setfml_updatetime(setfml);
    setfml_updateloop(setfml);
    setfml_handlecallbacks(setfml);
    sfRenderWindow_display(setfml->window);
}
