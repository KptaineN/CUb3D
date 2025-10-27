/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_keys.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:53:52 by adi-marc          #+#    #+#             */
/*   Updated: 2025/10/21 12:19:29 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* Debug flag for alien key debug prints; disable in production */
#ifndef DEBUG_ALIEN
# define DEBUG_ALIEN 0
#endif

static void perform_half_turn(t_cub *cub)
{
    double target_angle;

    if (!cub || !cub->player)
        return ;
    /* calculer l'angle cible (180° plus loin) */
    target_angle = cub->player->angle + M_PI;
    target_angle = fmod(target_angle, 2.0 * M_PI);
    if (target_angle < 0.0)
        target_angle += 2.0 * M_PI;
    /* déclencher l'animation de rotation rapide */
    cub->player->is_turning = true;
    cub->player->turn_target_angle = target_angle;
}


/* restart the game state when on game over */
static void restart_game(t_cub *cub)
{
    if (!cub || !cub->player)
        return ;
    /* restore basic player state */
    if (cub->player->max_health <= 0)
        cub->player->max_health = 3;
    cub->player->health = cub->player->max_health;
    cub->player->invincibility_timer = 0.0;
    cub->player->is_invincible = 0;
    cub->player->damage_flash_timer = 0.0;
    cub->player->damage_level = 0;
    cub->player->is_dead = 0;
    cub->player->death_timer = 0.0;
    /* restore player to initial spawn position & angle if available */
    if (cub->player_spawn_saved)
    {
        cub->player->position->x = cub->player_spawn_x;
        cub->player->position->y = cub->player_spawn_y;
        cub->player->angle = cub->player_spawn_angle;
        update_player_trig(cub);
    }
    /* reset movement keys */
    cub->player->key_up = false;
    cub->player->key_down = false;
    cub->player->key_left = false;
    cub->player->key_right = false;
    cub->player->left_rotate = false;
    cub->player->right_rotate = false;
    cub->player->is_running = false;
    cub->player->is_crouching = false;
    /* reload HUD textures (ensure HUD images are present) */
    load_hud_textures(cub);
    /* reinitialize or reset alien state */
    if (cub->alien)
    {
        alien_free(cub);
        alien_init(cub);
    }
    else
        alien_init(cub);
    /* hide minimap and caches */
    cub->show_minimap = 0;
    cub->minimap_cache_open = 0;
    /* clear possible visual flash */
    clear_image(cub);
}

int key_press(int keycode, t_cub *cub)
{
    if (cub && cub->player)
    {
        if (keycode == W || keycode == UP || keycode == Z)
            cub->player->key_up = true;
        else if (keycode == S || keycode == DOWN)
            cub->player->key_down = true;
        else if (keycode == Q || keycode == A)
            cub->player->key_left = true;
        else if (keycode == D)
            cub->player->key_right = true;
        else if (keycode == LEFT)
            cub->player->left_rotate = true;
        else if (keycode == RIGHT)
            cub->player->right_rotate = true;
        else if (keycode == 65505) // Shift gauche
            cub->player->is_running = true;
        else if (keycode == 65507) // Ctrl gauche
            cub->player->is_crouching = true;
        else if (keycode == 112) /* 'p' */
        {
            if (!cub->player)
            {
                return (0);
            }
            if (!cub->alien)
                {
                    if (!alien_init(cub))
                    {
                        return (0);
                    }
                }
                /* defensive: ensure pos pointer present before writing */
                if (!cub->alien || !cub->alien->pos)
                {
                    return (0);
                }
                /* place alien two blocks in front of player for debug */
                cub->alien->pos->x = cub->player->position->x + cub->player->direction->x * BLOCK * 2.0;
                cub->alien->pos->y = cub->player->position->y + cub->player->direction->y * BLOCK * 2.0;
                cub->alien->state = ALIEN_CHASE;
        }
        else if (keycode == 111) /* 'o' */
            alien_trigger_pounce(cub);
        else if (keycode == 98) /* 'b' */
            alien_toggle_replace_bg(cub);
        else if (keycode == 110) /* 'n' */
            alien_toggle_replace_bg(cub);
        else if (keycode == 117) /* 'u' */
            alien_dump_corners(cub);
        else if (keycode == 116) /* 't' */
            alien_toggle_force_transparent(cub);
        else if (keycode == KEY_COMMA || keycode == KEY_LESS
            || keycode == KEY_PERIOD || keycode == KEY_GREATER)
            perform_half_turn(cub);
        else if (keycode == 109) /* 'm' */
        {
            cub->show_minimap = !cub->show_minimap;
            if (cub->show_minimap)
            {
                cub->minimap_cache_open = 1;
                if (cub->player->position && cub->player->direction)
                {
                    cub->minimap_cached_px = cub->player->position->x / (double)BLOCK;
                    cub->minimap_cached_py = cub->player->position->y / (double)BLOCK;
                    cub->minimap_cached_dirx = cub->player->cos_angle;
                    cub->minimap_cached_diry = cub->player->sin_angle;
                }
            }
            else
                cub->minimap_cache_open = 0;
        }
        /* Tab press ignored - minimap only controlled by 'm' (hold) */
        else if (keycode == ESC_KEY)
        {
            cleanup_all(cub);
            exit(0);
        }
        else if (keycode == 65293) /* Enter / Return */
        {
            if (cub->player && cub->player->is_dead)
                restart_game(cub);
        }
        else if (keycode == 32) /* Space - attack/fire */
        {
            t_player *player = cub->player;
            int weapon = player->current_weapon;
            if (weapon == WEAPON_HAMMER && player->has_hammer)
            {
                weapon_attack_hammer(player);
                door_try_activate(cub);
                weapon_break_forward_soft_wall(cub);
            }
            else if (weapon == WEAPON_MP && player->has_mp)
            {
                int shot_fired;

                player->is_firing = 1;
                shot_fired = weapon_fire_mp(player);
                if (!shot_fired)
                    player->is_firing = 0;
                weapon_process_shot_result(cub, shot_fired);
            }
            else if (weapon == WEAPON_PISTOL && player->has_pistol)
            {
                int shot_fired;

                player->is_firing = 0;
                shot_fired = weapon_fire_pistol(player);
                weapon_process_shot_result(cub, shot_fired);
            }
            else if (weapon == WEAPON_FISTS)
            {
                player->is_firing = 0;
                weapon_punch(cub);
            }
        }
        else if (keycode == 101) /* 'e' - switch weapon */
        {
            weapon_switch(cub->player);
        }
        else if (keycode == 114) /* 'r' - reload */
        {
            weapon_reload(cub->player);
        }
        else if (keycode == 103) /* 'g' - kick */
        {
            weapon_kick(cub);
        }
    }
    return (0);
}

int key_drop(int keycode, t_cub *cub)
{
    if (cub && cub->player)
    {
        if (keycode == W  || keycode == Z || keycode == UP)
            cub->player->key_up = false;
        if (keycode == S || keycode == DOWN)
            cub->player->key_down = false;
        if (keycode == A || keycode == Q)
            cub->player->key_left = false;
        if (keycode == D)
            cub->player->key_right = false;
        if (keycode == LEFT)
            cub->player->left_rotate = false;
        if (keycode == RIGHT)
            cub->player->right_rotate = false;
        if (keycode == 65505) // Shift
            cub->player->is_running = false;
        if (keycode == 65507) // Ctrl
            cub->player->is_crouching = false;
        if (keycode == 32) // Space - arrêt du tir
        {
            cub->player->is_firing = 0;
            /* Quand on relâche Space, on laisse l'animation blood se terminer */
            if (cub->alien)
                cub->alien->show_blood = 0;
        }
    /* key release no longer controls minimap (toggle on press) */
    /* Tab release ignored - minimap only controlled by 'm' (hold) */
    }
    return (0);
}
