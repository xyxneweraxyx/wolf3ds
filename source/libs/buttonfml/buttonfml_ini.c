/*
** EPITECH PROJECT, 2025
** buttonfml
** File description:
** Wrapper on the setfml library to create simple buttons easily.
*/

#include "./buttonfml.h"

buttonfml_t *buttonfml_ini(setfml_t *setfml)
{
    c_alloc_t *alloc = c_ini((uint16_t)100);
    buttonfml_t *buttonfml = NULL;

    if (!alloc)
        return NULL;
    buttonfml = c_alloc(sizeof(buttonfml_t), 1, alloc);
    if (!buttonfml)
        return NULL;
    buttonfml->alloc = alloc;
    buttonfml->buttons = linkedlist_ini();
    buttonfml->setfml = setfml;
    if (!buttonfml->buttons)
        return NULL;
    if (buttonfml_connectcallbacks(buttonfml) == (size_t)BUTTONFML_FAIL)
        return NULL;
    return buttonfml;
}

void buttonfml_destroy(buttonfml_t *buttonfml)
{
    button_t *data = NULL;
    char t_name[BUFF_TEXT_NAME] = {0};

    for (node_t *node = buttonfml->buttons->head; node; node = node->next) {
        data = (button_t *)node->data;
        str_cat(t_name, 2, data->name, "_idle");
        if (setfml_texturefromname(buttonfml->setfml, t_name, true))
            setfml_texturedel(buttonfml->setfml, t_name);
        str_cat(t_name, 2, data->name, "_hover");
        if (setfml_texturefromname(buttonfml->setfml, t_name, true))
            setfml_texturedel(buttonfml->setfml, t_name);
        str_cat(t_name, 2, data->name, "_click");
        if (setfml_texturefromname(buttonfml->setfml, t_name, true))
            setfml_texturedel(buttonfml->setfml, t_name);
        if (data->button)
            setfml_spritedel(buttonfml->setfml, data->button->name);
    }
    linkedlist_destroy(buttonfml->buttons, false);
    c_delete(buttonfml->alloc, true);
}
