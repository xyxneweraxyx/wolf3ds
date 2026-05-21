/*
** EPITECH PROJECT, 2025
** classhandler
** File description:
** Create and manage classes and entities with this simple lib.
*/

#ifndef CLASSHANDLER_H
    #define CLASSHANDLER_H

    // Standard includes
    #include <stdio.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <stdbool.h>

    // Library includes
    #include "./../../c_alloc/c_alloc.h"
    #include "./../../linkedlist/linkedlist.h"
    #include "./../../str/str.h"

    // Return codes
    #define CLASSHANDLER_SUCC 0
    #define CLASSHANDLER_FAIL 84

// Type definitions

typedef struct class_s {

    /*
    Name of the class. Must be unique.
    */
    char *name;

    /*
    Linked list of entity_t for the class.
    */
    linkedlist_t *entities;

    /*
    Size (in bytes) of a single entity's data.
    */
    size_t byte_size;

} class_t;

typedef struct entity_s {

    /*
    Name of the entity. Must be unique.
    */
    char *name;

    /*
    Data of the entity. Must cast to the correct structure when using.
    */
    char *data;

    /*
    The class object the entity is a part of. Assigned automatically.
    */
    class_t *class;

} entity_t;

typedef struct classhandler_s {

    /*
    The c_alloc_t object used for the class handler. Managed automatically.
    */
    c_alloc_t *alloc;

    /*
    A linked list of the classes. Managed automatically.
    */
    linkedlist_t *classes;

    /*
    The current amount of classes. Managed automatically.
    */
    size_t class_amount;

    /*
    The current amount of entities. Managed automatically.
    */
    size_t entity_amount;

} classhandler_t;

// Functions

/// Class handler functions

/*
Initializes the class handler. Must be called before any other function.
Returns the class handler on success, or NULL.
*/
classhandler_t *classhandler_init(void);

/*
Destroys the class handler and all of its components.
*/
void classhandler_destroy(classhandler_t *classhandler);


/// Class functions

/*
Creates a new class based on it's name and the size of it's data structure.
Returns the created class, or NULL.
*/
class_t *classhandler_classcreate(classhandler_t *classhandler,
    size_t byte_size, const char *name);

/*
Destroys a class based on it's name.
Returns CLASSHANDLER_SUCC or CLASSHANDLER_FAIL.
*/
size_t classhandler_classdestroy(classhandler_t *classhandler,
    const char *name);


/// Entity functions

/*
Creates and storess a new class entity.
Returns the entity created, or NULL.
*/
entity_t *classhandler_entitycreate(classhandler_t *classhandler,
    const char *classname, const char *entityname);

/*
Destroys and deletes the data of a class entity.
Returns CLASSHANDLER_SUCC or CLASSHANDLER_FAIL.
*/
size_t classhandler_entitydestroy(classhandler_t *classhandler,
    const char *classname, const char *entityname);


/// API functions

/*
Fetches an entity based on its class and name.
If entityname is NULL, will return the first entity of the class, if any.
Returns a pointer to the entity, or NULL.
*/
entity_t *classhandler_fetchentityname(classhandler_t *classhandler,
    const char *classname, const char *entityname);

/*
Fetches an entity based on a comparison function.
Returns a pointer to the entity, or NULL.
*/
entity_t *classhandler_fetchentitycomp(classhandler_t *classhandler,
    const char *classname, bool (*comp)(entity_t *entity));

/*
Fetches all entities that satisfy a comparison function.
Returns a NULL-terminated array of pointers to the entities, or NULL.
*/
entity_t **classhandler_fetchentitiescomp(classhandler_t *classhandler,
    const char *classname, bool (*comp)(entity_t *entity));

/*
Fetches an entity based on a score function.
Only the entity with the highest score in the class will return.
Returns a pointer to the entity, or NULL.
*/
entity_t *classhandler_fetchentityscore(classhandler_t *classhandler,
    const char *classname, float (*score)(entity_t *entity));

#endif
