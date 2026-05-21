/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** Callback connexion entry point.
*/

#include "./../../include/wolf3d.h"
#include "./callbacks.h"

static void attempt_move(wolf_t *wolf, entity_t *plr, float x, float z)
{
    player_t *data = (player_t *)plr->data;
    float margin = 0.1f;
    float next_x = data->pos.x + x;
    float next_z = data->pos.z + z;
    float check_x = next_x + (x >= 0 ? margin : -margin);
    float check_z = next_z + (z >= 0 ? margin : -margin);

    if (!raycast_is_collision(wolf->raycast,
            wolf->map[(int)data->pos.z][(int)check_x]))
        player_gpmovex(plr, x);
    if (!raycast_is_collision(wolf->raycast,
            wolf->map[(int)check_z][(int)data->pos.x]))
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

static void flash_light_sound(wolf_t *wolf, entity_t *plr)
{
    if (sfKeyboard_isKeyPressed(sfKeyW))
        change_shader(wolf);
    if (!((player_t *)plr->data)->jmp_strength)
        play_step_sound(wolf);
    else
        sfSound_stop(wolf->step_sound->sound);
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

static void rotate_player(entity_t *plr, float rot_speed)
{
    if (sfKeyboard_isKeyPressed(sfKeyQ) ||
        sfJoystick_getAxisPosition(0, sfJoystickX) < -50)
        player_addrotation(plr, rot_speed * -1);
    if (sfKeyboard_isKeyPressed(sfKeyD) ||
        sfJoystick_getAxisPosition(0, sfJoystickX) > 50)
        player_addrotation(plr, rot_speed);
}

static void movement(wolf_t *wolf, entity_t *plr)
{
    player_t *data = (player_t *)plr->data;
    float speed = data->move_speed / 100;
    float rot_speed = data->rot_speed / 100;

    translate_player(wolf, plr, data, speed);
    rotate_player(plr, rot_speed);
    flash_light_sound(wolf, plr);
}

static void jump(wolf_t *wolf, entity_t *entity)
{
    player_t *data = (player_t *)entity->data;

    if (!data->jmp_strength) {
        if (sfKeyboard_isKeyPressed(sfKeySpace) ||
            sfJoystick_isButtonPressed(0, 1)) {
            sfSound_stop(wolf->step_sound->sound);
            player_gpjump(entity);
        }
    }
    data->jmp_gravity += data->gravity / 100;
    data->jmp_strength -= data->jmp_gravity;
    if (data->pos.y + data->jmp_strength < 0) {
        data->pos.y = 0;
        data->jmp_strength = 0;
        data->jmp_gravity = 0;
        return;
    }
    data->pos.y += data->jmp_strength;
}

static entity_t *get_player_entity(wolf_t *wolf)
{
    return classhandler_fetchentityname(wolf->classhandler,
        CLASS_PLAYERS, NULL);
}

static size_t keyboard(setfml_t *setfml, void *userdata)
{
    wolf_t *wolf = (wolf_t *)setfml->userdata;
    entity_t *entity = NULL;

    (void)userdata;
    if (wolf->state != GAME_PLAY)
        return (size_t)SETFML_SUCC;
    entity = get_player_entity(wolf);
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
