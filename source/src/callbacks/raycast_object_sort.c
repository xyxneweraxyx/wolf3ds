/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Raycast object projection sorting.
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

static void swap_object_sprites(struct object_sprite_s *a,
    struct object_sprite_s *b)
{
    struct object_sprite_s tmp = *a;

    *a = *b;
    *b = tmp;
}

static size_t find_farthest_sprite(wolf_t *wolf, size_t start)
{
    size_t farthest = start;

    for (size_t i = start + 1; i < wolf->object_sprite_count; i++) {
        if (wolf->object_sprites[i].distance >
            wolf->object_sprites[farthest].distance)
            farthest = i;
    }
    return farthest;
}

static void sort_object_sprites(wolf_t *wolf)
{
    size_t farthest = 0;

    for (size_t i = 0; i < wolf->object_sprite_count; i++) {
        farthest = find_farthest_sprite(wolf, i);
        if (farthest != i)
            swap_object_sprites(&wolf->object_sprites[i],
                &wolf->object_sprites[farthest]);
    }
}

static void append_object_sprite(wolf_t *wolf, object_hit_t *hit)
{
    struct object_sprite_s sprite = {0};

    if (wolf->object_sprite_count >= wolf->object_sprite_capacity)
        return;
    if (!project_object_hit(wolf, hit, &sprite))
        return;
    wolf->object_sprites[wolf->object_sprite_count] = sprite;
    wolf->object_sprite_count += 1;
}

static bool is_object_cell(char cell)
{
    return cell >= '2' && cell <= '4';
}

static void append_map_object(wolf_t *wolf, size_t x, size_t y)
{
    object_hit_t hit = {
        .map_x = (int)x,
        .map_y = (int)y,
        .type = wolf->object_map[y][x]
    };

    append_object_sprite(wolf, &hit);
}

static void project_object_row(wolf_t *wolf, size_t y)
{
    for (size_t x = 0; wolf->object_map[y][x]; x++) {
        if (!is_object_cell(wolf->object_map[y][x]))
            continue;
        append_map_object(wolf, x, y);
    }
}

void project_object_hits(wolf_t *wolf)
{
    if (!wolf || !wolf->object_sprites || !wolf->object_map)
        return;
    wolf->object_sprite_count = 0;
    for (size_t y = 0; wolf->object_map[y]; y++)
        project_object_row(wolf, y);
    sort_object_sprites(wolf);
}
