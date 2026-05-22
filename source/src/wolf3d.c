/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Main entry point of wolf3d.
*/

#include "./../include/wolf3d.h"
#include "./hud/shoot/include/shoot.h"

static const char *const DEFAULT_MAP[] = {
    "111111111111111111111111111111",
    "100000000000000000000000000001",
    "100000000000000000000000000001",
    "100011110000000001111000000001",
    "100010000000000000001000000001",
    "100010001111111100001000000001",
    "100010001000000100001000000001",
    "100010001000000100001111110001",
    "100000001000000100000000000001",
    "100000001000000100000000000001",
    "100011111000000111110000000001",
    "100000000000000000000000000001",
    "100000000000000000000000000001",
    "100000000111000000000111000001",
    "100000000010000000000010000001",
    "100000000010000000000010000001",
    "100111100010001110000010000001",
    "100000000010000010000010000001",
    "100000000000000010000000000001",
    "100000000000000010000000000001",
    "100000001111111110000111111001",
    "100000000000000000000000000101",
    "100000000000000000000000000101",
    "101111000000111111000000001101",
    "100001000000100001000000001001",
    "100001000000100001000000000001",
    "100001111111100001111111000001",
    "100000000000000000000000000001",
    "100000000000000000000000000001",
    "111111111111111111111111111111",
    NULL
};

static const char *const DEFAULT_OBJECT_MAP[] = {
    "000000000000000000000000000000",
    "000000200000000000000000000000",
    "000000000000003000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000004000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    NULL
};

static int count_map_rows(const char *const src_map[])
{
    int row_count = 0;

    for (; src_map[row_count]; row_count++);
    return row_count;
}

static void free_map_rows(char **map, int count)
{
    for (int i = 0; i < count; i++)
        free(map[i]);
}

static bool fill_map_rows(char **map, const char *const src_map[],
    int row_count)
{
    for (int i = 0; i < row_count; i++) {
        map[i] = malloc(sizeof(char) * (str_len(src_map[i]) + 1));
        if (!map[i]) {
            free_map_rows(map, i);
            return false;
        }
        str_cpy(src_map[i], map[i]);
    }
    map[row_count] = NULL;
    return true;
}

static char **create_map(const char *const src_map[])
{
    int row_count = count_map_rows(src_map);
    char **map = malloc(sizeof(char *) * (row_count + 1));

    if (!map)
        return NULL;
    if (!fill_map_rows(map, src_map, row_count)) {
        free(map);
        return NULL;
    }
    return map;
}

static uint8_t *create_height_row(char *map_row, uint8_t top_val)
{
    int cols = str_len(map_row);
    uint8_t *row = malloc(sizeof(uint8_t) * cols);

    if (!row)
        return NULL;
    for (int x = 0; x < cols; x++)
        row[x] = (map_row[x] != '0') ? top_val : 0;
    return row;
}

static bool ini_height_maps(wolf_t *wolf)
{
    int rows = 0;

    for (; wolf->map[rows]; rows++);
    wolf->height_top = calloc(rows + 1, sizeof(uint8_t *));
    wolf->height_bottom = calloc(rows + 1, sizeof(uint8_t *));
    if (!wolf->height_top || !wolf->height_bottom)
        return false;
    for (int y = 0; y < rows; y++) {
        wolf->height_top[y] = create_height_row(wolf->map[y],
            (uint8_t)RAYCAST_HEIGHT_UNIT);
        wolf->height_bottom[y] = create_height_row(wolf->map[y], 0);
        if (!wolf->height_top[y] || !wolf->height_bottom[y])
            return false;
    }
    wolf->height_top[3][4] = 8;
    wolf->height_top[3][6] = 32;
    wolf->height_bottom[3][7] = 20;
    wolf->height_top[3][7] = 24;
    wolf->height_top[5][9] = 8;
    return true;
}

static bool ini_object_hits(wolf_t *wolf)
{
    size_t height = 0;
    size_t area = 0;

    if (!wolf->object_map || !wolf->object_map[0])
        return false;
    for (; wolf->object_map[height]; height++);
    area = (size_t)str_len(wolf->object_map[0]) * height;
    wolf->object_hit_count = 0;
    wolf->object_hit_capacity = area;
    wolf->object_hits = c_alloc(sizeof(object_hit_t), area, wolf->alloc);
    wolf->object_sprite_count = 0;
    wolf->object_sprite_capacity = area;
    wolf->object_sprites = c_alloc(sizeof(struct object_sprite_s),
        area, wolf->alloc);
    return wolf->object_hits != NULL && wolf->object_sprites != NULL;
}

static wolf_t *ini_main(c_alloc_t *alloc)
{
    wolf_t *wolf = NULL;

    if (!alloc)
        return NULL;
    wolf = c_alloc(sizeof(wolf_t), 1, alloc);
    if (!wolf)
        return destroy_return_null(wolf);
    wolf->alloc = alloc;
    wolf->state = GAME_MENU;
    wolf->map = create_map(DEFAULT_MAP);
    wolf->object_map = create_map(DEFAULT_OBJECT_MAP);
    if (!wolf->map || !wolf->object_map)
        return destroy_return_null(wolf);
    if (!ini_height_maps(wolf))
        return destroy_return_null(wolf);
    if (!ini_object_hits(wolf))
        return destroy_return_null(wolf);
    return wolf;
}

int main(void)
{
    c_alloc_t *alloc = c_ini((uint16_t)100);
    wolf_t *wolf = ini_main(alloc);

    if (!wolf)
        return WOLF_FAIL;
    if (ini_csfml_env(wolf) == WOLF_FAIL ||
        menu_init(wolf) == WOLF_FAIL ||
        create_classes(wolf) == WOLF_FAIL ||
        connect_callbacks(wolf) == WOLF_FAIL ||
        init_life(wolf) == WOLF_FAIL ||
        init_map_2d(wolf) == WOLF_FAIL ||
        init_weapon(wolf) == WOLF_FAIL)
        return destroy_return_int(WOLF_FAIL, wolf);
    init_sound(wolf);
    if (setfml_windowcreate(wolf->setfml) == (size_t)SETFML_FAIL)
        return destroy_return_int(WOLF_FAIL, wolf);
    setfml_windowstart(wolf->setfml);
    return destroy_return_int(WOLF_SUCC, wolf);
}
