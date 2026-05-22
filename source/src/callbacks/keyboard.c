/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Keyboard callbacks for wolf3d.
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

static bool tile_blocks_move(wolf_t *wolf, player_t *data, int tx, int tz)
{
    uint8_t t_bottom = 0;
    uint8_t t_top = 0;
    float p_top = data->pos.y + (float)RAYCAST_HEIGHT_UNIT;

    if (tz < 0 || tx < 0 || !wolf->map[tz])
        return true;
    if (!raycast_is_collision(wolf->raycast, wolf->map[tz][tx]))
        return false;
    if (!wolf->height_bottom || !wolf->height_top)
        return true;
    t_bottom = wolf->height_bottom[tz][tx];
    t_top = wolf->height_top[tz][tx];
    return (float)t_top > data->pos.y && (float)t_bottom < p_top;
}

static void attempt_move(wolf_t *wolf, entity_t *plr, float x, float z)
{
    player_t *data = (player_t *)plr->data;
    float margin = 0.1f;
    float check_x = data->pos.x + x + (x >= 0 ? margin : -margin);
    float check_z = data->pos.z + z + (z >= 0 ? margin : -margin);

    if (!tile_blocks_move(wolf, data, (int)check_x, (int)data->pos.z))
        player_gpmovex(plr, x);
    if (!tile_blocks_move(wolf, data, (int)data->pos.x, (int)check_z))
        player_gpmovez(plr, z);
}

int change_shader(wolf_t *wolf)
{
    if (wolf->raycast->flash_light == 1) {
        wolf->raycast->on_draw = &shader2;
        wolf->raycast->flash_light = 0;
    } else {
        wolf->raycast->on_draw = &draw_wall;
        wolf->raycast->flash_light = 1;
    }
    return 0;
}

static void translate_player(wolf_t *wolf, entity_t *plr,
    player_t *data, float speed)
{
    if (sfKeyboard_isKeyPressed(sfKeyZ) ||
        sfJoystick_getAxisPosition(0, sfJoystickY) < -50)
        attempt_move(wolf, plr,
            cosf(data->rotation * DEG_TO_RAD) * speed,
            sinf(data->rotation * DEG_TO_RAD) * speed);
    if (sfKeyboard_isKeyPressed(sfKeyS) ||
        sfJoystick_getAxisPosition(0, sfJoystickY) > 50)
        attempt_move(wolf, plr,
            -1 * cosf(data->rotation * DEG_TO_RAD) * speed,
            -1 * sinf(data->rotation * DEG_TO_RAD) * speed);
}

static void movement(wolf_t *wolf, entity_t *plr)
{
    player_t *data = (player_t *)plr->data;
    float speed = data->move_speed / 100;
    float rot_speed = data->rot_speed / 100;

    translate_player(wolf, plr, data, speed);
    if (sfKeyboard_isKeyPressed(sfKeyQ) ||
        sfJoystick_getAxisPosition(0, sfJoystickX) < -50)
        player_addrotation(plr, rot_speed * -1);
    if (sfKeyboard_isKeyPressed(sfKeyD) ||
        sfJoystick_getAxisPosition(0, sfJoystickX) > 50)
        player_addrotation(plr, rot_speed);
    if (sfKeyboard_isKeyPressed(sfKeyW))
        change_shader(wolf);
    if (!data->jmp_strength)
        play_step_sound(wolf);
    else
        sfSound_stop(wolf->step_sound->sound);
}

static void get_vertical_bounds(wolf_t *wolf, player_t *data,
    float *floor_h, float *ceil_h)
{
    int tx = (int)data->pos.x;
    int tz = (int)data->pos.z;
    float hb = 0.0f;
    float ht = 0.0f;

    *floor_h = 0.0f;
    *ceil_h = (float)UINT8_MAX;
    if (!wolf->height_top || !wolf->height_bottom || !wolf->map[tz])
        return;
    if (!raycast_is_collision(wolf->raycast, wolf->map[tz][tx]))
        return;
    hb = (float)wolf->height_bottom[tz][tx];
    ht = (float)wolf->height_top[tz][tx];
    if (data->pos.y + (float)RAYCAST_HEIGHT_UNIT <= hb)
        *ceil_h = hb;
    else
        *floor_h = ht;
}

static void try_jump(wolf_t *wolf, entity_t *entity, player_t *data,
    float floor_h)
{
    if (data->jmp_strength || data->pos.y > floor_h)
        return;
    if (!sfKeyboard_isKeyPressed(sfKeySpace) &&
        !sfJoystick_isButtonPressed(0, 1))
        return;
    sfSound_stop(wolf->step_sound->sound);
    player_gpjump(entity);
}

static void jump(wolf_t *wolf, entity_t *entity)
{
    player_t *data = (player_t *)entity->data;
    float floor_h = 0.0f;
    float ceil_h = 0.0f;

    get_vertical_bounds(wolf, data, &floor_h, &ceil_h);
    try_jump(wolf, entity, data, floor_h);
    data->jmp_gravity += data->gravity / 100;
    data->jmp_strength -= data->jmp_gravity;
    if (data->pos.y + (float)RAYCAST_HEIGHT_UNIT + data->jmp_strength >= ceil_h)
        data->jmp_strength = ceil_h - (float)RAYCAST_HEIGHT_UNIT - data->pos.y;
    if (data->pos.y + data->jmp_strength <= floor_h) {
        data->pos.y = floor_h;
        data->jmp_strength = 0;
        data->jmp_gravity = 0;
        return;
    }
    data->pos.y += data->jmp_strength;
}

static size_t keyboard(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    entity_t *entity = classhandler_fetchentityname(wolf->classhandler,
        CLASS_PLAYERS, NULL);

    (void)userdata;
    if (wolf->state != GAME_PLAY)
        return (size_t)SETFML_SUCC;
    if (!entity || !entity->data)
        return (size_t)SETFML_FAIL;
    movement(wolf, entity);
    jump(wolf, entity);
    return (size_t)SETFML_SUCC;
}

int connect_keyboard(wolf_t *wolf)
{
    setfml_add(wolf->setfml, &(setfml_func_comp_t){NULL, &keyboard},
        "kb_directions", LOOP_DATA);
    return WOLF_SUCC;
}