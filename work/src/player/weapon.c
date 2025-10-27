/* ************************************************************************** */
/*                                                                            */
/*   weapon.c - Weapon system (Hammer & MP)                                  */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#define HAMMER_SPARK_LIFETIME 0.35
#define HAMMER_SPARK_SCREEN_RATIO 0.22
#define HAMMER_SPARK_VERTICAL_OFFSET -40

/* Initialize weapon system */
void weapon_init(t_player *p)
{
    if (!p)
        return;
    p->current_weapon = WEAPON_FISTS;
    p->is_attacking = 0;
    p->attack_timer = 0.0;
    p->attack_frame = 0;
    p->is_aiming = 0;
    p->is_firing = 0;           /* Init flag de tir continu */
    p->fire_timer = 0.0;
    p->weapon_bob_timer = 0.0;
    p->weapon_bob_offset = 0.0;
    p->ammo_clip_size = 30;
    p->ammo_clip = 0;
    p->ammo_reserve = 0;
    p->is_reloading = 0;
    p->reload_timer = 0.0;
    p->mp_reload_overlay = 0.0;
    p->pistol_clip_size = 6;
    p->pistol_clip = 0;
    p->pistol_reserve = 0;
    p->pistol_is_reloading = 0;
    p->pistol_reload_timer = 0.0;
    p->pistol_fire_timer = 0.0;
    p->fist_attack_timer = 0.0;
    p->kick_timer = 0.0;
    p->pickup_touch_timer = 0.0;
    p->show_bullet_impact = 0;
    p->bullet_impact_timer = 0.0;
    p->bullet_impact_x = 0;
    p->bullet_impact_y = 0;
    p->has_hammer = 0;
    p->has_mp = 0;
    p->has_pistol = 0;
}

/* Update weapon animations */
void weapon_update(t_player *p, double dt)
{
    if (!p)
        return;

    p->weapon_bob_timer += dt;

    if (p->is_reloading)
    {
        p->reload_timer -= dt;
        if (p->reload_timer <= 0.0)
        {
            int needed;

            p->reload_timer = 0.0;
            p->is_reloading = 0;
            needed = p->ammo_clip_size - p->ammo_clip;
            if (needed > 0 && p->ammo_reserve > 0)
            {
                if (needed > p->ammo_reserve)
                    needed = p->ammo_reserve;
                p->ammo_clip += needed;
                p->ammo_reserve -= needed;
            }
        }
    }
    if (p->mp_reload_overlay > 0.0)
    {
        p->mp_reload_overlay -= dt;
        if (p->mp_reload_overlay < 0.0)
            p->mp_reload_overlay = 0.0;
    }
    if (p->pistol_is_reloading)
    {
        p->pistol_reload_timer -= dt;
        if (p->pistol_reload_timer <= 0.0)
        {
            int needed = p->pistol_clip_size - p->pistol_clip;
            p->pistol_reload_timer = 0.0;
            p->pistol_is_reloading = 0;
            if (needed > 0 && p->pistol_reserve > 0)
            {
                if (needed > p->pistol_reserve)
                    needed = p->pistol_reserve;
                p->pistol_clip += needed;
                p->pistol_reserve -= needed;
            }
        }
    }
    if (p->pistol_fire_timer > 0.0)
    {
        p->pistol_fire_timer -= dt;
        if (p->pistol_fire_timer <= 0.0)
        {
            p->pistol_fire_timer = 0.0;
            if (p->current_weapon == WEAPON_PISTOL && p->is_attacking)
                p->is_attacking = 0;
        }
    }
    if (p->fist_attack_timer > 0.0)
    {
        p->fist_attack_timer -= dt;
        if (p->fist_attack_timer <= 0.0)
        {
            p->fist_attack_timer = 0.0;
            if (p->current_weapon == WEAPON_FISTS)
                p->is_attacking = 0;
        }
    }
    if (p->kick_timer > 0.0)
    {
        p->kick_timer -= dt;
        if (p->kick_timer <= 0.0)
        {
            p->kick_timer = 0.0;
            if (p->current_weapon == WEAPON_FISTS)
                p->is_attacking = 0;
        }
    }
    if (p->pickup_touch_timer > 0.0)
    {
        p->pickup_touch_timer -= dt;
        if (p->pickup_touch_timer < 0.0)
            p->pickup_touch_timer = 0.0;
    }
    
    /* Update weapon bob when moving (running effect) */
    int is_moving = p->key_up || p->key_down || p->key_left || p->key_right;
    
    if (is_moving)
    {
        /* Bob BEAUCOUP plus vite en course - effet plus prononcé */
        double bob_speed = p->is_running ? 18.0 : 8.0;   /* 18 Hz en course vs 8 Hz normal */
        double bob_amplitude = p->is_running ? 25.0 : 15.0; /* Amplitude plus grande en course */
        double phase = p->weapon_bob_timer * bob_speed;

        /* Sine wave bobbing effect (left-right sway) */
        p->weapon_bob_offset = sin(phase) * bob_amplitude;
    }
    else
    {
        /* Gradually return to center when not moving */
        p->weapon_bob_offset *= 0.85;
        if (fabs(p->weapon_bob_offset) < 0.5)
            p->weapon_bob_offset = 0.0;
    }
    
    /* Update attack animation timer (hammer) */
    if (p->is_attacking && p->current_weapon == WEAPON_HAMMER)
    {
        p->attack_timer += dt;
        
        /* Hammer animation: 6 frames, 0.1s per frame = 0.6s total */
        double frame_duration = 0.1;
        p->attack_frame = (int)(p->attack_timer / frame_duration);
        
        if (p->attack_frame >= 6)
        {
            p->is_attacking = 0;
            p->attack_timer = 0.0;
            p->attack_frame = 0;
        }
    }
    
    /* Update fire animation timer (MP) */
    if (p->fire_timer > 0.0)
    {
        p->fire_timer -= dt;
        if (p->fire_timer <= 0.0)
        {
            p->fire_timer = 0.0;
            if (p->current_weapon == WEAPON_MP)
                p->is_attacking = 0;
        }
    }
    
    /* Update bullet impact effect timer */
    if (p->show_bullet_impact)
    {
        p->bullet_impact_timer -= dt;
        if (p->bullet_impact_timer <= 0.0)
        {
            p->show_bullet_impact = 0;
            p->bullet_impact_timer = 0.0;
        }
    }
}

/* Start hammer attack */
void weapon_attack_hammer(t_player *p)
{
    if (!p || p->is_attacking || p->current_weapon != WEAPON_HAMMER
        || !p->has_hammer)
        return;

    p->is_attacking = 1;
    p->attack_timer = 0.0;
    p->attack_frame = 0;
}

void weapon_reload(t_player *p)
{
    if (!p)
        return;
    if (p->current_weapon == WEAPON_MP && p->has_mp)
    {
        if (p->ammo_clip >= p->ammo_clip_size || p->ammo_reserve <= 0 || p->is_reloading)
            return;
        p->is_reloading = 1;
        p->reload_timer = 1.0;
        p->is_attacking = 0;
        p->fire_timer = 0.0;
        p->is_firing = 0;
        p->mp_reload_overlay = 2.0;
    }
    else if (p->current_weapon == WEAPON_PISTOL && p->has_pistol)
    {
        if (p->pistol_clip >= p->pistol_clip_size || p->pistol_reserve <= 0 || p->pistol_is_reloading)
            return;
        p->pistol_is_reloading = 1;
        p->pistol_reload_timer = 1.0;
        p->pistol_fire_timer = 0.0;
        p->is_attacking = 0;
        p->is_firing = 0;
    }
}

/* Start MP fire */
int weapon_fire_mp(t_player *p)
{
    if (!p || p->current_weapon != WEAPON_MP || !p->has_mp)
        return (0);
    if (p->is_reloading)
        return (0);
    if (p->ammo_clip <= 0)
    {
        if (p->ammo_reserve > 0)
            weapon_reload(p);
        else
        return (0);
    }

    p->ammo_clip -= 1;
    p->is_attacking = 1;
    p->fire_timer = 0.12;
    
    /* Déclencher l'effet d'impact de balle au centre de l'écran */
    p->show_bullet_impact = 1;
    p->bullet_impact_timer = 0.15; /* Impact visible pendant 0.15s */
    p->bullet_impact_x = WIDTH / 2; /* Centre de l'écran */
    p->bullet_impact_y = HEIGHT / 2;
    return (1);
}

int weapon_fire_pistol(t_player *p)
{
    if (!p || p->current_weapon != WEAPON_PISTOL || !p->has_pistol)
        return (0);
    if (p->pistol_is_reloading)
        return (0);
    if (p->pistol_fire_timer > 0.0)
        return (0);
    if (p->pistol_clip <= 0)
    {
        if (p->pistol_reserve > 0)
            weapon_reload(p);
        else
        {
            fprintf(stderr, "[WEAPON] Pistolet vide !\n");
            return (0);
        }
    }
    p->pistol_clip -= 1;
    p->pistol_fire_timer = 0.25;
    p->is_attacking = 1;
    p->show_bullet_impact = 1;
    p->bullet_impact_timer = 0.15;
    p->bullet_impact_x = WIDTH / 2;
    p->bullet_impact_y = HEIGHT / 2;
    return (1);
}

void weapon_process_shot_result(t_cub *cub, int shot_fired)
{
    int hit_alien;
    int hit_barrel;

    if (!cub || !shot_fired)
        return;
    hit_alien = 0;
    if (cub->alien)
    {
        hit_alien = alien_check_hit_by_mp(cub);
        if (hit_alien && cub->alien)
            cub->alien->show_blood = 1;
    }
    hit_barrel = 0;
    if (!hit_alien)
        hit_barrel = barrel_handle_shot(cub);
    if (!hit_alien && !hit_barrel)
        weapon_add_wall_impact(cub, WIDTH / 2, HEIGHT / 2);
}

void weapon_auto_fire(t_cub *cub)
{
    t_player    *p;
    int          shot_fired;

    if (!cub || !(p = cub->player))
        return;
    if (!p->is_firing)
        return;
    if (p->current_weapon == WEAPON_MP && p->has_mp)
    {
        if (p->is_reloading || p->fire_timer > 0.0)
            return;
        shot_fired = weapon_fire_mp(p);
        if (!shot_fired)
            p->is_firing = 0;
        weapon_process_shot_result(cub, shot_fired);
    }
}

void weapon_punch(t_cub *cub)
{
    t_player *p;

    if (!cub || !(p = cub->player))
        return;
    if (p->current_weapon != WEAPON_FISTS)
        return;
    if (p->fist_attack_timer > 0.0 || p->kick_timer > 0.0)
        return;
    p->fist_attack_timer = 0.25;
    p->is_attacking = 1;
    p->is_firing = 0;
}

static int clamp_dir(double value)
{
    if (value > 0.4)
        return 1;
    if (value < -0.4)
        return -1;
    return 0;
}

static int weapon_is_available(t_player *p, int weapon)
{
    if (!p)
        return (weapon == WEAPON_FISTS);
    if (weapon == WEAPON_FISTS)
        return 1;
    if (weapon == WEAPON_HAMMER)
        return p->has_hammer;
    if (weapon == WEAPON_MP)
        return p->has_mp;
    if (weapon == WEAPON_PISTOL)
        return p->has_pistol;
    return 0;
}

static int break_soft_wall(t_cub *cub, int map_x, int map_y)
{
    if (!cub || !cub->map)
        return (0);
    if (map_y < 0 || map_x < 0)
        return (0);
    if (!cub->map[map_y])
        return (0);
    if ((size_t)map_x >= ft_strlen(cub->map[map_y]))
        return (0);
    if (cub->map[map_y][map_x] == '6')
    {
        cub->map[map_y][map_x] = '0';
        return (1);
    }
    return (0);
}

void weapon_break_forward_soft_wall(t_cub *cub)
{
    t_player    *p;
    int         dir_x;
    int         dir_y;
    int         target_x;
    int         target_y;

    if (!cub || !(p = cub->player))
        return;
    dir_x = clamp_dir(p->direction->x);
    dir_y = clamp_dir(p->direction->y);
    if (dir_x == 0 && dir_y == 0)
    {
        if (fabs(p->direction->x) >= fabs(p->direction->y))
            dir_x = (p->direction->x >= 0) ? 1 : -1;
        else
            dir_y = (p->direction->y >= 0) ? 1 : -1;
    }
    target_x = (int)(p->position->x / (double)BLOCK) + dir_x;
    target_y = (int)(p->position->y / (double)BLOCK) + dir_y;
    break_soft_wall(cub, target_x, target_y);
}

void weapon_kick(t_cub *cub)
{
    t_player    *p;
    int         dir_x;
    int         dir_y;
    int         player_mx;
    int         player_my;

    if (!cub || !(p = cub->player))
        return;
    if (p->kick_timer > 0.0)
        return;
    dir_x = clamp_dir(p->direction->x);
    dir_y = clamp_dir(p->direction->y);
    if (dir_x == 0 && dir_y == 0)
    {
        if (fabs(p->direction->x) >= fabs(p->direction->y))
            dir_x = (p->direction->x >= 0) ? 1 : -1;
        else
            dir_y = (p->direction->y >= 0) ? 1 : -1;
    }
    player_mx = (int)(p->position->x / (double)BLOCK);
    player_my = (int)(p->position->y / (double)BLOCK);
    if (dir_x != 0 || dir_y != 0)
    {
        int target_x = player_mx + dir_x;
        int target_y = player_my + dir_y;
        char tile = '0';

        if (cub->map && target_y >= 0 && target_x >= 0 && cub->map[target_y]
            && (size_t)target_x < ft_strlen(cub->map[target_y]))
            tile = cub->map[target_y][target_x];
        if (tile == '6')
            break_soft_wall(cub, target_x, target_y);
        else
            barrel_try_push(cub, target_x, target_y, dir_x, dir_y);
    }
    p->kick_timer = 0.35;
    if (p->current_weapon == WEAPON_FISTS)
        p->is_attacking = 1;
    p->is_firing = 0;
}

/* Check if MP shot hits alien (called from game_keys.c) */
int weapon_mp_hits_alien(t_cub *cub)
{
    if (!cub || !cub->player || !cub->alien)
        return 0;
    
    /* Get player position and direction */
    double px = cub->player->position->x;
    double py = cub->player->position->y;
    double dirX = cub->player->direction->x;
    double dirY = cub->player->direction->y;
    double planeX = cub->player->plane->x;
    double planeY = cub->player->plane->y;
    
    /* Get alien position */
    double ax = cub->alien->pos->x;
    double ay = cub->alien->pos->y;
    
    /* Vector from player to alien */
    double spriteX = ax - px;
    double spriteY = ay - py;
    
    /* Transform alien position to camera space */
    double invDet = 1.0 / (planeX * dirY - dirX * planeY);
    double transformX = invDet * (dirY * spriteX - dirX * spriteY);
    double transformY = invDet * (-planeY * spriteX + planeX * spriteY);
    
    /* Alien must be in front of player */
    if (transformY <= 0.1)
        return 0;
    
    /* Check distance - must be within range */
    double dist = sqrt(spriteX * spriteX + spriteY * spriteY);
    double max_range = 600.0; /* Portée maximale */
    if (dist > max_range || dist < 1.0)
        return 0;
    
    /* Calculate alien screen position */
    int spriteScreenX = (int)((WIDTH / 2.0) * (1.0 + transformX / transformY));
    
    /* Calculate alien size on screen */
    int spriteHeight = abs((int)(HEIGHT / transformY));
    int spriteWidth = abs((int)(HEIGHT / transformY)); /* Carré pour simplifier */
    
    /* Calculate hitbox bounds on screen */
    int drawStartX = -spriteWidth / 2 + spriteScreenX;
    int drawEndX = spriteWidth / 2 + spriteScreenX;
    int drawStartY = -spriteHeight / 2 + HEIGHT / 2;
    int drawEndY = spriteHeight / 2 + HEIGHT / 2;
    
    /* Clipping */
    if (drawStartX < 0) drawStartX = 0;
    if (drawEndX >= WIDTH) drawEndX = WIDTH - 1;
    if (drawStartY < 0) drawStartY = 0;
    if (drawEndY >= HEIGHT) drawEndY = HEIGHT - 1;
    
    /* Crosshair is at center of screen (WIDTH/2, HEIGHT/2) */
    int crosshair_x = WIDTH / 2;
    int crosshair_y = HEIGHT / 2;
    
    /* AMÉLIORATION: Hitbox AGRANDIE pour faciliter le tir */
    /* Marge généreuse de 35% pour rendre le tir plus facile et intuitif */
    int margin_x = (int)(spriteWidth * 0.35);
    int margin_y = (int)(spriteHeight * 0.35);
    
    /* Bonus : ajouter une marge fixe minimum pour les aliens lointains */
    int min_margin = 30; /* 30 pixels minimum de marge */
    if (margin_x < min_margin) margin_x = min_margin;
    if (margin_y < min_margin) margin_y = min_margin;
    
    /* Check if crosshair is within alien's screen hitbox ÉLARGIE */
    if (crosshair_x >= (drawStartX - margin_x) && crosshair_x <= (drawEndX + margin_x) &&
        crosshair_y >= (drawStartY - margin_y) && crosshair_y <= (drawEndY + margin_y))
    {
        return 1; /* Hit ! */
    }
    
    return 0;
}

/* Toggle aiming (right click) */
void weapon_toggle_aim(t_player *p, int aiming)
{
    if (!p)
        return;
    if (p->current_weapon == WEAPON_MP && p->has_mp)
        p->is_aiming = aiming;
    else
        p->is_aiming = 0;
}

/* Switch weapon */
void weapon_switch(t_player *p)
{
    if (!p || p->is_attacking)
        return;
    int start = p->current_weapon;
    int attempts = 0;
    do
    {
        p->current_weapon = (p->current_weapon + 1) % 4;
        attempts++;
        if (weapon_is_available(p, p->current_weapon))
            break;
    } while (attempts < 4 && p->current_weapon != start);
    if (!weapon_is_available(p, p->current_weapon))
        p->current_weapon = WEAPON_FISTS;
    p->is_aiming = 0;
    p->attack_timer = 0.0;
    p->attack_frame = 0;
    p->fire_timer = 0.0;
    p->is_reloading = 0;
    p->reload_timer = 0.0;
    p->mp_reload_overlay = 0.0;
    p->pistol_is_reloading = 0;
    p->pistol_reload_timer = 0.0;
    p->pistol_fire_timer = 0.0;
    p->fist_attack_timer = 0.0;
    p->kick_timer = 0.0;
    p->is_attacking = 0;
    p->is_firing = 0;
    p->show_bullet_impact = 0;
}

/* Get current weapon texture */
t_tur *weapon_get_texture(t_cub *cub)
{
    if (!cub || !cub->player)
        return NULL;
    
    t_player *p = cub->player;
    if (!weapon_is_available(p, p->current_weapon))
        p->current_weapon = WEAPON_FISTS;
    
    if (p->current_weapon == WEAPON_HAMMER)
    {
        if (p->is_attacking && p->attack_frame < 6)
        {
            /* Return appropriate hammer frame (hammer_1 to hammer_6) */
            return &cub->hammer[p->attack_frame];
        }
        else
        {
            /* Idle hammer (frame 0) */
            return &cub->hammer[0];
        }
    }
    else if (p->current_weapon == WEAPON_MP)
    {
        /* Fire animation avec flash rapide */
        if (p->fire_timer > 0.0)
        {
            /* Alternate TRÈS RAPIDEMENT entre fire_mp1 et fire_mp2 pour effet flash */
            int fire_frame = ((int)(p->fire_timer * 30.0)) % 2; /* 30 Hz flicker - TRÈS rapide */
            return (fire_frame == 0) ? &cub->mp_fire1 : &cub->mp_fire2;
        }
        else if (p->is_firing)
        {
            int fire_frame = ((int)(p->weapon_bob_timer * 30.0)) % 2;
            return (fire_frame == 0) ? &cub->mp_fire1 : &cub->mp_fire2;
        }
        /* Aiming */
        else if (p->is_aiming)
        {
            return &cub->mp_aim;
        }
        /* Idle */
        else
        {
            return &cub->mp_idle;
        }
    }
    else if (p->current_weapon == WEAPON_PISTOL)
    {
        if (p->pistol_fire_timer > 0.0)
            return &cub->pistol_fire_tex;
        return &cub->pistol_idle_tex;
    }
    else if (p->current_weapon == WEAPON_FISTS)
    {
        if (p->kick_timer > 0.0)
        {
            double half = 0.5 * 0.35;
            if (p->kick_timer > half)
                return &cub->kick_tex[0];
            return &cub->kick_tex[1];
        }
        if (p->fist_attack_timer > 0.0)
            return &cub->fists_touch_tex;
        return &cub->fists_idle_tex;
    }
    
    return NULL;
}

/* Draw weapon on screen */
void weapon_draw(t_cub *cub)
{
    if (!cub || !cub->player)
        return;
    
    t_player *player = cub->player;
    t_tur *weapon_tex = weapon_get_texture(cub);
    int hide_weapon = (player->pickup_touch_timer > 0.0);

    if (!weapon_tex || !weapon_tex->img || !weapon_tex->data)
        hide_weapon = 1;
    
    /* Base dimensions */
    int weapon_w = weapon_tex ? weapon_tex->w : 0;
    int weapon_h = weapon_tex ? weapon_tex->h : 0;
    double scale = 1.0;
    int offset_x = 0;
    int offset_y = 0;
    int bob_offset = 0;

    if (player->current_weapon == WEAPON_MP)
    {
        scale = 1.8;
        offset_x = 30;
        bob_offset = (int)(player->weapon_bob_offset);
    }
    else if (player->current_weapon == WEAPON_HAMMER)
    {
        offset_x = 200;
        bob_offset = (int)(player->weapon_bob_offset);
    }
    else if (player->current_weapon == WEAPON_PISTOL)
    {
        scale = 2.6;
        offset_x = 0;
        offset_y = -50;
        bob_offset = (int)(player->weapon_bob_offset * 0.35);
    }
    else if (player->current_weapon == WEAPON_FISTS)
    {
        scale = 1.5;
        offset_x = 0;
        offset_y = 40;
        bob_offset = 0;
    }

    int scaled_w = (int)(weapon_w * scale);
    int scaled_h = (int)(weapon_h * scale);
    if (scaled_w <= 0 || scaled_h <= 0)
        hide_weapon = 1;

    int screen_x = (WIDTH - scaled_w) / 2 + offset_x + bob_offset;
    int screen_y = HEIGHT - scaled_h - offset_y;
    int weapon_floor = screen_y + scaled_h;
    if (weapon_floor < 0)
        weapon_floor = 0;
    if (weapon_floor > HEIGHT)
        weapon_floor = HEIGHT;

    /* Draw weapon sprite with magenta transparency and scaling */
    int bytes = weapon_tex ? weapon_tex->bpp / 8 : 0;
    int screen_bytes = cub->mlx_bpp / 8;
    int orig_w = weapon_tex ? weapon_tex->w : 0;
    int orig_h = weapon_tex ? weapon_tex->h : 0;

    if (player->kick_timer > 0.0
        && cub->kick_tex[0].img && cub->kick_tex[0].data)
    {
        double total = 0.35;
        t_tur *kick_tex = (player->kick_timer > total * 0.6)
            ? &cub->kick_tex[0] : &cub->kick_tex[1];
        int kick_w = kick_tex->w;
        int kick_h = kick_tex->h;
        int draw_w = (int)(kick_w * 0.7);
        int draw_h = (int)(kick_h * 0.7);
        if (draw_w <= 0) draw_w = kick_w;
        if (draw_h <= 0) draw_h = kick_h;
        int kick_x = (WIDTH - draw_w) / 2;
        int kick_base = weapon_floor - 20;
        if (kick_base > HEIGHT - 10)
            kick_base = HEIGHT - 10;
        if (kick_base < draw_h)
            kick_base = draw_h;
        int kick_y = kick_base - draw_h;
        if (kick_y < 0)
            kick_y = 0;
        int bytes = kick_tex->bpp / 8;
        for (int y = 0; y < draw_h; ++y)
        {
            int dst_y = kick_y + y;
            if (dst_y < 0 || dst_y >= HEIGHT)
                continue;
            int src_y = (int)((double)y / (double)draw_h * kick_h);
            if (src_y >= kick_h)
                src_y = kick_h - 1;
            for (int x = 0; x < draw_w; ++x)
            {
                int dst_x = kick_x + x;
                if (dst_x < 0 || dst_x >= WIDTH)
                    continue;
                int src_x = (int)((double)x / (double)draw_w * kick_w);
                if (src_x >= kick_w)
                    src_x = kick_w - 1;
                int src_idx = src_y * kick_tex->sl + src_x * bytes;
                unsigned char *src = (unsigned char*)kick_tex->data + src_idx;
                unsigned char b = src[0];
                unsigned char g = (bytes > 1) ? src[1] : 0;
                unsigned char r = (bytes > 2) ? src[2] : 0;
                if (r > 250 && g < 10 && b > 250)
                    continue;
                int dst_idx = dst_y * cub->mlx_line_size + dst_x * screen_bytes;
                unsigned char *dst = (unsigned char*)cub->mlx_data + dst_idx;
                dst[0] = b;
                if (screen_bytes > 1) dst[1] = g;
                if (screen_bytes > 2) dst[2] = r;
            }
        }
    }

    if (!hide_weapon)
    {
        for (int y = 0; y < scaled_h; y++)
        {
            int dst_y = screen_y + y;
            if (dst_y < 0 || dst_y >= HEIGHT)
                continue;
            
            for (int x = 0; x < scaled_w; x++)
            {
                int dst_x = screen_x + x;
                if (dst_x < 0 || dst_x >= WIDTH)
                    continue;
                
                int src_x = (int)(x / scale);
                int src_y = (int)(y / scale);
                
                if (src_x >= orig_w || src_y >= orig_h)
                    continue;
                
                int src_idx = src_y * weapon_tex->sl + src_x * bytes;
                unsigned char *src = (unsigned char*)weapon_tex->data + src_idx;
                unsigned char b = src[0];
                unsigned char g = (bytes > 1) ? src[1] : 0;
                unsigned char r = (bytes > 2) ? src[2] : 0;
                if (r > 250 && g < 10 && b > 250)
                    continue;
                int dst_idx = dst_y * cub->mlx_line_size + dst_x * screen_bytes;
                unsigned char *dst = (unsigned char*)cub->mlx_data + dst_idx;
                dst[0] = b;
                if (screen_bytes > 1) dst[1] = g;
                if (screen_bytes > 2) dst[2] = r;
            }
        }
    }

    /* EFFET D'IMPACT DE BALLE: Dessiner une petite croix noire là où le joueur tire */
    if (player->show_bullet_impact)
    {
        int impact_x = player->bullet_impact_x;
        int impact_y = player->bullet_impact_y;
        int impact_size = 8; /* Taille de la croix d'impact */
        
        /* Dessiner une croix noire (impact de balle) */
        for (int i = -impact_size; i <= impact_size; i++)
        {
            /* Ligne horizontale */
            int px = impact_x + i;
            int py = impact_y;
            if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
            {
                int idx = py * cub->mlx_line_size + px * screen_bytes;
                unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
                pix[0] = 0; /* B */
                if (screen_bytes > 1) pix[1] = 0; /* G */
                if (screen_bytes > 2) pix[2] = 0; /* R */
            }
            
            /* Ligne verticale */
            px = impact_x;
            py = impact_y + i;
            if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
            {
                int idx = py * cub->mlx_line_size + px * screen_bytes;
                unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
                pix[0] = 0; /* B */
                if (screen_bytes > 1) pix[1] = 0; /* G */
                if (screen_bytes > 2) pix[2] = 0; /* R */
            }
        }
        
        /* Dessiner un petit cercle autour de l'impact */
        for (int dy = -impact_size/2; dy <= impact_size/2; dy++)
        {
            for (int dx = -impact_size/2; dx <= impact_size/2; dx++)
            {
                if (dx*dx + dy*dy <= (impact_size/2)*(impact_size/2))
                {
                    int px = impact_x + dx;
                    int py = impact_y + dy;
                    if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
                    {
                        int idx = py * cub->mlx_line_size + px * screen_bytes;
                        unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
                        pix[0] = 20; /* B - gris très foncé */
                        if (screen_bytes > 1) pix[1] = 20; /* G */
                        if (screen_bytes > 2) pix[2] = 20; /* R */
                    }
                }
            }
        }
    }

    if (player->mp_reload_overlay > 0.0
        && cub->mp_reload_tex.img && cub->mp_reload_tex.data)
    {
        int overlay_w = cub->mp_reload_tex.w;
        int overlay_h = cub->mp_reload_tex.h;
        int overlay_x = (WIDTH - overlay_w) / 2;
        int overlay_y = HEIGHT / 3;
        int ov_bytes = cub->mp_reload_tex.bpp / 8;
        for (int y = 0; y < overlay_h; ++y)
        {
            int dst_y = overlay_y + y;
            if (dst_y < 0 || dst_y >= HEIGHT)
                continue;
            for (int x = 0; x < overlay_w; ++x)
            {
                int dst_x = overlay_x + x;
                if (dst_x < 0 || dst_x >= WIDTH)
                    continue;
                int src_idx = y * cub->mp_reload_tex.sl + x * ov_bytes;
                unsigned char *src = (unsigned char*)cub->mp_reload_tex.data + src_idx;
                unsigned char b = src[0];
                unsigned char g = (ov_bytes > 1) ? src[1] : 0;
                unsigned char r = (ov_bytes > 2) ? src[2] : 0;
                if (r > 250 && g < 10 && b > 250)
                    continue;
                int dst_idx = dst_y * cub->mlx_line_size + dst_x * screen_bytes;
                unsigned char *dst = (unsigned char*)cub->mlx_data + dst_idx;
                dst[0] = b;
                if (screen_bytes > 1) dst[1] = g;
                if (screen_bytes > 2) dst[2] = r;
            }
        }
    }
    if (player->pickup_touch_timer > 0.0
        && cub->fists_touch_tex.img && cub->fists_touch_tex.data)
    {
        int touch_w = cub->fists_touch_tex.w;
        int touch_h = cub->fists_touch_tex.h;
        int draw_w = (int)(WIDTH * 0.4);
        if (draw_w <= 0)
            draw_w = touch_w;
        int draw_h = (int)((double)draw_w * ((double)touch_h / (double)touch_w));
        if (draw_h <= 0)
            draw_h = touch_h;
        int touch_x = (WIDTH - draw_w) / 2;
        int touch_base = weapon_floor + 20;
        if (touch_base > HEIGHT - 5)
            touch_base = HEIGHT - 5;
        if (touch_base < draw_h)
            touch_base = draw_h;
        int touch_y = touch_base - draw_h;
        if (touch_y < 0)
            touch_y = 0;
        int bytes = cub->fists_touch_tex.bpp / 8;
        for (int y = 0; y < draw_h; ++y)
        {
            int dst_y = touch_y + y;
            if (dst_y < 0 || dst_y >= HEIGHT)
                continue;
            int src_y = (int)((double)y / (double)draw_h * touch_h);
            if (src_y >= touch_h)
                src_y = touch_h - 1;
            for (int x = 0; x < draw_w; ++x)
            {
                int dst_x = touch_x + x;
                if (dst_x < 0 || dst_x >= WIDTH)
                    continue;
                int src_x = (int)((double)x / (double)draw_w * touch_w);
                if (src_x >= touch_w)
                    src_x = touch_w - 1;
                int src_idx = src_y * cub->fists_touch_tex.sl + src_x * bytes;
                unsigned char *src = (unsigned char*)cub->fists_touch_tex.data + src_idx;
                unsigned char b = src[0];
                unsigned char g = (bytes > 1) ? src[1] : 0;
                unsigned char r = (bytes > 2) ? src[2] : 0;
                if (r > 250 && g < 10 && b > 250)
                    continue;
                int dst_idx = dst_y * cub->mlx_line_size + dst_x * screen_bytes;
                unsigned char *dst = (unsigned char*)cub->mlx_data + dst_idx;
                dst[0] = b;
                if (screen_bytes > 1) dst[1] = g;
                if (screen_bytes > 2) dst[2] = r;
            }
        }
    }
}

void weapon_update_sparks(t_cub *cub, double dt)
{
    if (!cub)
        return;
    for (int i = 0; i < MAX_HAMMER_SPARKS; ++i)
    {
        t_hammer_spark *spark = &cub->hammer_sparks[i];

        if (!spark->active)
            continue;
        if (spark->duration <= 0.0)
        {
            spark->active = 0;
            spark->timer = 0.0;
            spark->duration = 0.0;
            continue;
        }
        spark->timer -= dt;
        if (spark->timer <= 0.0)
        {
            spark->active = 0;
            spark->timer = 0.0;
            spark->duration = 0.0;
        }
    }
}

void hammer_spawn_panel_spark(t_cub *cub, double world_x, double world_y)
{
    int             slot;
    t_hammer_spark  *spark;

    if (!cub)
        return;
    slot = -1;
    for (int i = 0; i < MAX_HAMMER_SPARKS; ++i)
    {
        if (!cub->hammer_sparks[i].active)
        {
            slot = i;
            break;
        }
    }
    if (slot == -1)
    {
        double min_timer = 1e9;

        for (int i = 0; i < MAX_HAMMER_SPARKS; ++i)
        {
            if (cub->hammer_sparks[i].timer < min_timer)
            {
                min_timer = cub->hammer_sparks[i].timer;
                slot = i;
            }
        }
        if (slot == -1)
            slot = 0;
    }
    spark = &cub->hammer_sparks[slot];
    spark->active = 1;
    spark->duration = HAMMER_SPARK_LIFETIME;
    spark->timer = HAMMER_SPARK_LIFETIME;
    spark->x = world_x;
    spark->y = world_y;
    if (cub->player && cub->player->direction && cub->player->position)
    {
        double front_x = cub->player->position->x
            + cub->player->direction->x * (BLOCK * 0.45);
        double front_y = cub->player->position->y
            + cub->player->direction->y * (BLOCK * 0.45);
        spark->x = front_x;
        spark->y = front_y;
    }
}

void weapon_draw_hammer_sparks(t_cub *cub)
{
    int best_idx;
    double best_progress;

    if (!cub)
        return;
    best_idx = -1;
    best_progress = -1.0;
    for (int i = 0; i < MAX_HAMMER_SPARKS; ++i)
    {
        t_hammer_spark *spark = &cub->hammer_sparks[i];
        double progress;

        if (!spark->active || spark->duration <= 0.0)
            continue;
        progress = 1.0 - (spark->timer / spark->duration);
        if (progress < 0.0)
            progress = 0.0;
        if (progress > 1.0)
            progress = 1.0;
        if (progress > best_progress)
        {
            best_progress = progress;
            best_idx = i;
        }
    }
    if (best_idx == -1 || !cub->hammer_spark_tex[0].img)
        return;
    int frame_idx = (int)(best_progress * HAMMER_SPARK_FRAMES);
    if (frame_idx >= HAMMER_SPARK_FRAMES)
        frame_idx = HAMMER_SPARK_FRAMES - 1;
    t_tur *tex = &cub->hammer_spark_tex[frame_idx];
    if (!tex->img || !tex->data)
        return;
    int bytes = tex->bpp / 8;
    int screen_bytes = cub->mlx_bpp / 8;
    if (bytes <= 0 || screen_bytes <= 0)
        return;
    int draw_w = (int)(WIDTH * HAMMER_SPARK_SCREEN_RATIO);
    if (draw_w <= 0)
        draw_w = tex->w;
    int draw_h = (int)((double)draw_w * ((double)tex->h / (double)tex->w));
    if (draw_h <= 0)
        draw_h = tex->h;
    int spark_x = (WIDTH - draw_w) / 2;
    int spark_y = HEIGHT / 2 - draw_h + HAMMER_SPARK_VERTICAL_OFFSET;
    if (spark_y < 0)
        spark_y = 0;
    if (spark_y + draw_h >= HEIGHT)
        draw_h = HEIGHT - spark_y - 1;
    if (draw_h <= 0)
        return;
    for (int y = 0; y < draw_h; ++y)
    {
        int dst_y = spark_y + y;
        if (dst_y < 0 || dst_y >= HEIGHT)
            continue;
        int src_y = (int)((double)y / (double)draw_h * tex->h);
        if (src_y >= tex->h)
            src_y = tex->h - 1;
        for (int x = 0; x < draw_w; ++x)
        {
            int dst_x = spark_x + x;
            if (dst_x < 0 || dst_x >= WIDTH)
                continue;
            int src_x = (int)((double)x / (double)draw_w * tex->w);
            if (src_x >= tex->w)
                src_x = tex->w - 1;
            int src_idx = src_y * tex->sl + src_x * bytes;
            unsigned char *src = (unsigned char*)tex->data + src_idx;
            unsigned char b = src[0];
            unsigned char g = (bytes > 1) ? src[1] : 0;
            unsigned char r = (bytes > 2) ? src[2] : 0;
            if (r > 250 && g < 10 && b > 250)
                continue;
            int dst_idx = dst_y * cub->mlx_line_size + dst_x * screen_bytes;
            unsigned char *dst = (unsigned char*)cub->mlx_data + dst_idx;
            dst[0] = b;
            if (screen_bytes > 1) dst[1] = g;
            if (screen_bytes > 2) dst[2] = r;
        }
    }
}

/* Ajouter un impact de balle sur un mur */
void weapon_add_wall_impact(t_cub *cub, double x, double y)
{
    if (!cub)
        return;
    
    /* VERSION SIMPLIFIÉE : x et y sont déjà des coordonnées d'écran */
    int screen_x = (int)x;
    int screen_y = (int)y;
    
    /* Trouver un slot libre ou réutiliser le plus ancien */
    int slot = -1;
    for (int i = 0; i < MAX_BULLET_IMPACTS; i++)
    {
        if (!cub->bullet_impacts[i].active)
        {
            slot = i;
            break;
        }
    }
    
    /* Si aucun slot libre, réutiliser le plus ancien (premier dans le tableau) */
    if (slot == -1)
    {
        /* Décaler tous les impacts vers le haut */
        for (int i = 0; i < MAX_BULLET_IMPACTS - 1; i++)
        {
            cub->bullet_impacts[i] = cub->bullet_impacts[i + 1];
        }
        slot = MAX_BULLET_IMPACTS - 1;
    }
    
    /* Créer le nouvel impact */
    cub->bullet_impacts[slot].active = 1;
    cub->bullet_impacts[slot].screen_x = screen_x;
    cub->bullet_impacts[slot].screen_y = screen_y;
    cub->bullet_impacts[slot].timer = 15.0; /* 15 secondes */
    
    if (cub->bullet_impact_count < MAX_BULLET_IMPACTS)
        cub->bullet_impact_count++;
}

/* Mettre à jour les timers des impacts */
void weapon_update_impacts(t_cub *cub, double dt)
{
    if (!cub)
        return;
    
    for (int i = 0; i < MAX_BULLET_IMPACTS; i++)
    {
        if (cub->bullet_impacts[i].active)
        {
            cub->bullet_impacts[i].timer -= dt;
            if (cub->bullet_impacts[i].timer <= 0.0)
            {
                cub->bullet_impacts[i].active = 0;
                if (cub->bullet_impact_count > 0)
                    cub->bullet_impact_count--;
            }
        }
    }
}

/* Dessiner un crosshair style Predator pour le MP */
void weapon_draw_crosshair(t_cub *cub)
{
    if (!cub || !cub->player)
        return;
    
    /* N'afficher le crosshair que pour les armes à feu */
    if (!cub->player)
        return;
    if (!((cub->player->current_weapon == WEAPON_MP && cub->player->has_mp)
        || (cub->player->current_weapon == WEAPON_PISTOL && cub->player->has_pistol)))
        return;
    
    int center_x = WIDTH / 2;
    int center_y = HEIGHT / 2;
    int screen_bytes = cub->mlx_bpp / 8;
    
    /* Couleur rouge style Predator (R=255, G=0, B=0) */
    unsigned char r = 255, g = 0, b = 0;
    
    /* AMÉLIORATION : Crosshair ultra minimaliste et plus petit */
    int size = 11; /* Réduit encore (14 → 11) */
    int corner_len = 4; /* Longueur de chaque coin réduite (5 → 4) */
    
    /* COIN SUPÉRIEUR GAUCHE - partie horizontale */
    for (int x = center_x - size; x <= center_x - size + corner_len; x++)
    {
        int y = center_y - size/2;
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        {
            int idx = y * cub->mlx_line_size + x * screen_bytes;
            unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
            pix[0] = b;
            if (screen_bytes > 1) pix[1] = g;
            if (screen_bytes > 2) pix[2] = r;
        }
    }
    
    /* COIN SUPÉRIEUR GAUCHE - partie diagonale */
    for (int i = 0; i <= corner_len; i++)
    {
        int x = center_x - size + i;
        int y = center_y - size/2 + i;
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        {
            int idx = y * cub->mlx_line_size + x * screen_bytes;
            unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
            pix[0] = b;
            if (screen_bytes > 1) pix[1] = g;
            if (screen_bytes > 2) pix[2] = r;
        }
    }
    
    /* COIN SUPÉRIEUR DROIT - partie horizontale */
    for (int x = center_x + size - corner_len; x <= center_x + size; x++)
    {
        int y = center_y - size/2;
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        {
            int idx = y * cub->mlx_line_size + x * screen_bytes;
            unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
            pix[0] = b;
            if (screen_bytes > 1) pix[1] = g;
            if (screen_bytes > 2) pix[2] = r;
        }
    }
    
    /* COIN SUPÉRIEUR DROIT - partie diagonale */
    for (int i = 0; i <= corner_len; i++)
    {
        int x = center_x + size - i;
        int y = center_y - size/2 + i;
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        {
            int idx = y * cub->mlx_line_size + x * screen_bytes;
            unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
            pix[0] = b;
            if (screen_bytes > 1) pix[1] = g;
            if (screen_bytes > 2) pix[2] = r;
        }
    }
    
    /* COIN INFÉRIEUR (pointe du triangle) */
    for (int i = 0; i <= corner_len; i++)
    {
        /* Ligne diagonale gauche de la pointe */
        int x1 = center_x - i;
        int y1 = center_y + size/2 - i;
        if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT)
        {
            int idx = y1 * cub->mlx_line_size + x1 * screen_bytes;
            unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
            pix[0] = b;
            if (screen_bytes > 1) pix[1] = g;
            if (screen_bytes > 2) pix[2] = r;
        }
        
        /* Ligne diagonale droite de la pointe */
        int x2 = center_x + i;
        int y2 = center_y + size/2 - i;
        if (x2 >= 0 && x2 < WIDTH && y2 >= 0 && y2 < HEIGHT)
        {
            int idx = y2 * cub->mlx_line_size + x2 * screen_bytes;
            unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
            pix[0] = b;
            if (screen_bytes > 1) pix[1] = g;
            if (screen_bytes > 2) pix[2] = r;
        }
    }
    
    /* POINT CENTRAL (mire) - MINI CROIX + (3 pixels en forme de +) */
    /* Pixel central */
    if (center_x >= 0 && center_x < WIDTH && center_y >= 0 && center_y < HEIGHT)
    {
        int idx = center_y * cub->mlx_line_size + center_x * screen_bytes;
        unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
        pix[0] = b;
        if (screen_bytes > 1) pix[1] = g;
        if (screen_bytes > 2) pix[2] = r;
    }
    
    /* Pixel gauche */
    if ((center_x - 1) >= 0 && (center_x - 1) < WIDTH && center_y >= 0 && center_y < HEIGHT)
    {
        int idx = center_y * cub->mlx_line_size + (center_x - 1) * screen_bytes;
        unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
        pix[0] = b;
        if (screen_bytes > 1) pix[1] = g;
        if (screen_bytes > 2) pix[2] = r;
    }
    
    /* Pixel droit */
    if ((center_x + 1) >= 0 && (center_x + 1) < WIDTH && center_y >= 0 && center_y < HEIGHT)
    {
        int idx = center_y * cub->mlx_line_size + (center_x + 1) * screen_bytes;
        unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
        pix[0] = b;
        if (screen_bytes > 1) pix[1] = g;
        if (screen_bytes > 2) pix[2] = r;
    }
}

/* Dessiner les impacts de balles sur les murs - VERSION SIMPLIFIÉE */
void weapon_draw_wall_impacts(t_cub *cub)
{
    if (!cub || cub->bullet_impact_count == 0)
        return;
    
    int screen_bytes = cub->mlx_bpp / 8;
    
    for (int i = 0; i < MAX_BULLET_IMPACTS; i++)
    {
        if (!cub->bullet_impacts[i].active)
            continue;
        
        /* Calculer l'opacité basée sur le timer (fade out progressif) */
        double alpha = cub->bullet_impacts[i].timer / 15.0;
        if (alpha > 1.0) alpha = 1.0;
        if (alpha < 0.0) alpha = 0.0;
        
        /* Position simplifiée: juste stocker screen_x, screen_y */
        int screen_x = cub->bullet_impacts[i].screen_x;
        int screen_y = cub->bullet_impacts[i].screen_y;
        
        /* Dessiner un simple point noir (impact de balle) - VERSION SIMPLE */
        int impact_size = 2; /* Petit impact simple */
        for (int dy = -impact_size; dy <= impact_size; dy++)
        {
            for (int dx = -impact_size; dx <= impact_size; dx++)
            {
                int px = screen_x + dx;
                int py = screen_y + dy;
                if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
                {
                    int idx = py * cub->mlx_line_size + px * screen_bytes;
                    unsigned char *pix = (unsigned char*)cub->mlx_data + idx;
                    
                    /* Couleur gris foncé avec alpha pour fade out */
                    unsigned char color = (unsigned char)(40 * alpha);
                    pix[0] = color;
                    if (screen_bytes > 1) pix[1] = color;
                    if (screen_bytes > 2) pix[2] = color;
                }
            }
        }
    }
}
