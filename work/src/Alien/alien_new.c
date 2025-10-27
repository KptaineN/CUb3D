void alien_update(t_cub *cub,double dt)
{
    if (!cub || !cub->alien || !cub->player) return;
    t_alien *a = cub->alien;
    if (!a->pos || !a->dir || !cub->player->position) return;
    if (dt > 0.2) dt = 0.2;
    if (dt <= 0.0) dt = 1.0 / 60.0;

    a->is_walking = 0;
    if (dt > 0.0) {
        a->smooth_speed *= 0.96;
        a->smooth_vx *= 0.94;
        a->smooth_vy *= 0.94;
    }
    if (a->attack_cooldown > 0.0)
    {
        a->attack_cooldown -= dt;
        if (a->attack_cooldown < 0.0)
            a->attack_cooldown = 0.0;
    }
    if (a->state_timer > 0.0)
    {
        a->state_timer -= dt;
        if (a->state_timer < 0.0)
            a->state_timer = 0.0;
    }
    if (a->path_recalc_timer > 0.0)
    {
        a->path_recalc_timer -= dt;
        if (a->path_recalc_timer < 0.0)
            a->path_recalc_timer = 0.0;
    }
    if (a->crouch_timer > 0.0)
    {
        a->crouch_timer -= dt;
        if (a->crouch_timer <= 0.0)
        {
            a->crouch_timer = 0.0;
            a->is_crouching = 0;
        }
    }
    if (a->attack_anim_timer > 0.0)
    {
        a->attack_anim_timer -= dt;
        if (a->attack_anim_timer <= 0.0)
        {
            a->attack_anim_timer = 0.0;
            a->has_attacked = 0;
        }
    }
    else if (a->has_attacked && a->attack_cooldown <= 0.0)
        a->has_attacked = 0;

    double prev_x = a->pos->x;
    double prev_y = a->pos->y;

    double px = cub->player->position->x;
    double py = cub->player->position->y;
    double player_dist = 0.0;
    int player_visible = alien_player_visible(cub, a, &player_dist);
    if (player_dist <= 0.0)
    {
        double dx = px - a->pos->x;
        double dy = py - a->pos->y;
        player_dist = sqrt(dx * dx + dy * dy);
    }
    int attack_pipeline = (a->is_preparing || a->is_stalking || a->is_attacking);
    double audible_range = fmin(a->view_distance * 0.75, BLOCK * 6.0);
    int player_audible = (!player_visible && player_dist <= audible_range);
    if (a->state != ALIEN_CHASE)
        a->chase_runup_timer = 0.0;

    if (!attack_pipeline)
    {
        if (player_visible)
        {
            a->last_seen_x = px;
            a->last_seen_y = py;
            a->last_sensed_x = px;
            a->last_sensed_y = py;
            a->state = ALIEN_CHASE;
            a->state_timer = 1.2;
            alien_reset_path(a);
        }
        else if (player_audible)
        {
            a->last_sensed_x = px;
            a->last_sensed_y = py;
            if (!a->is_stalking && !a->is_preparing && !a->is_attacking)
            {
                a->state = ALIEN_STALKING;
                a->is_stalking = 1;
                a->stalk_timer = 1.1;
                alien_reset_path(a);
            }
        }
        else if (a->state == ALIEN_CHASE && a->state_timer <= 0.0)
        {
            double sx = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_x : a->pos->x;
            double sy = (a->last_seen_y != 0.0 || a->last_seen_x != 0.0) ? a->last_seen_y : a->pos->y;
            alien_enter_search(cub, a, sx, sy);
        }
    }

    if (a->is_preparing)
    {
        double focus_x = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_x : px;
        double focus_y = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_y : py;
        alien_set_pounce_target(a, focus_x, focus_y);
        alien_face_target(a, focus_x, focus_y, 14.0, dt);
        a->prepare_timer -= dt;
        if (a->prepare_timer <= 0.0)
        {
            a->is_preparing = 0;
            a->is_attacking = 1;
            a->pounce_timer = ALIEN_POUNCE_MAX_DURATION;
            alien_face_target(a, a->pounce_target_x, a->pounce_target_y, 25.0, dt);
            a->state = ALIEN_CHASE;
        }
        return;
    }

    if (a->is_stalking)
    {
        double stalk_target_x = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_x : px;
        double stalk_target_y = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_y : py;
        if (!alien_build_path(cub, a, stalk_target_x, stalk_target_y))
        {
            double dx = stalk_target_x - a->pos->x;
            double dy = stalk_target_y - a->pos->y;
            double len = sqrt(dx * dx + dy * dy);
            if (len > 1e-3)
            {
                double vx = dx / len;
                double vy = dy / len;
                alien_try_move(cub, a, vx, vy, a->speed_patrol * 0.7, dt, 1);
                alien_face_target(a, stalk_target_x, stalk_target_y, 10.0, dt);
            }
        }
        else
        {
            alien_follow_path(cub, a, a->speed_patrol * 0.7, dt);
        }
        alien_update_anim(a, 6);
        a->stalk_timer -= dt;
        if (a->stalk_timer <= 0.0)
        {
            a->is_stalking = 0;
            a->is_preparing = 1;
            a->prepare_timer = ALIEN_PREP_TOTAL_TIME;
            a->state = ALIEN_PREPARING;
        }
        return;
    }

    if (a->is_attacking)
    {
        const double contact_dist = BLOCK * 0.55;
        const double target_hit_radius = fmax(contact_dist, BLOCK * 0.35);
        double dist_to_target = sqrt((a->pounce_target_x - a->pos->x) * (a->pounce_target_x - a->pos->x)
                + (a->pounce_target_y - a->pos->y) * (a->pounce_target_y - a->pos->y));
        if (player_dist <= contact_dist)
        {
            alien_finish_attack(cub, a, 1);
            return;
        }
        if (dist_to_target <= target_hit_radius)
        {
            alien_finish_attack(cub, a, 0);
            return;
        }
        alien_face_target(a, a->pounce_target_x, a->pounce_target_y, 22.0, dt);
        int moved = alien_try_move(cub, a, a->pounce_vx, a->pounce_vy, a->pounce_speed, dt, 0);
        if (!moved)
        {
            dist_to_target = sqrt((a->pounce_target_x - a->pos->x) * (a->pounce_target_x - a->pos->x)
                    + (a->pounce_target_y - a->pos->y) * (a->pounce_target_y - a->pos->y));
            if (player_dist <= contact_dist + BLOCK * 0.15 || dist_to_target <= target_hit_radius)
                alien_finish_attack(cub, a, player_dist <= contact_dist + BLOCK * 0.15);
            else
            {
                a->is_attacking = 0;
                a->pounce_timer = 0.0;
                if (a->attack_cooldown < 2.5)
                    a->attack_cooldown = 2.5;
                a->state = ALIEN_SEARCH;
                a->state_timer = 2.5;
                alien_reset_path(a);
            }
            return;
        }
        dist_to_target = sqrt((a->pounce_target_x - a->pos->x) * (a->pounce_target_x - a->pos->x)
                + (a->pounce_target_y - a->pos->y) * (a->pounce_target_y - a->pos->y));
        if (player_dist <= contact_dist || dist_to_target <= target_hit_radius)
        {
            alien_finish_attack(cub, a, player_dist <= contact_dist);
            return;
        }
        if (a->pounce_timer > 0.0)
        {
            a->pounce_timer -= dt;
            if (a->pounce_timer <= 0.0)
            {
                a->pounce_timer = 0.0;
                a->is_attacking = 0;
                if (a->attack_cooldown < 2.5)
                    a->attack_cooldown = 2.5;
                a->state = ALIEN_SEARCH;
                a->state_timer = 2.5;
                alien_reset_path(a);
            }
        }
        return;
    }

    switch (a->state)
    {
        case ALIEN_PATROL:
        default:
        {
            if ((a->target_x == 0.0 && a->target_y == 0.0) ||
                hypot(a->target_x - a->pos->x, a->target_y - a->pos->y) < BLOCK * 0.5)
            {
                alien_choose_patrol_target(cub, a);
            }
            if (!alien_build_path(cub, a, a->target_x, a->target_y))
            {
                double dx = a->target_x - a->pos->x;
                double dy = a->target_y - a->pos->y;
                double len = sqrt(dx * dx + dy * dy);
                if (len > 1e-3)
                {
                    double vx = dx / len;
                    double vy = dy / len;
                    alien_try_move(cub, a, vx, vy, a->speed_patrol, dt, 1);
                    alien_face_target(a, a->target_x, a->target_y, 6.0, dt);
                }
            }
            else
            {
                if (alien_follow_path(cub, a, a->speed_patrol, dt))
                    alien_choose_patrol_target(cub, a);
            }
            alien_update_anim(a, 10);
            break;
        }
        case ALIEN_SEARCH:
        {
            double goal_x = a->target_x;
            double goal_y = a->target_y;
            if (a->has_attacked && (fabs(a->retreat_x) > 1.0 || fabs(a->retreat_y) > 1.0))
            {
                goal_x = a->retreat_x;
                goal_y = a->retreat_y;
            }
            if (!alien_build_path(cub, a, goal_x, goal_y))
        {
            double dx = goal_x - a->pos->x;
            double dy = goal_y - a->pos->y;
            double len = sqrt(dx * dx + dy * dy);
            if (len > 1e-3)
            {
                double vx = dx / len;
                double vy = dy / len;
                alien_try_move(cub, a, vx, vy, a->speed_patrol * 0.6, dt, 1);
                alien_face_target(a, goal_x, goal_y, 7.0, dt);
            }
        }
        else
        {
            alien_follow_path(cub, a, a->speed_patrol * 0.65, dt);
        }
        alien_update_anim(a, 8);
        double dist_goal = hypot(goal_x - a->pos->x, goal_y - a->pos->y);
        if (dist_goal < BLOCK * 0.5 || a->state_timer <= 0.0)
        {
            double sx = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_x : goal_x;
            double sy = (a->last_seen_y != 0.0 || a->last_seen_x != 0.0) ? a->last_seen_y : goal_y;
            alien_enter_search(cub, a, sx, sy);
        }
        break;
    }
        case ALIEN_CHASE:
        {
            if (player_visible)
            {
                double dx = px - a->pos->x;
                double dy = py - a->pos->y;
                double len = sqrt(dx * dx + dy * dy);
                if (len > 1e-3)
                {
                    double vx = dx / len;
                    double vy = dy / len;
                    if (!alien_try_move(cub, a, vx, vy, a->speed_chase, dt, 1))
                    {
                        if (alien_build_path(cub, a, px, py))
                            alien_follow_path(cub, a, a->speed_chase * 0.95, dt);
                    }
                    alien_face_target(a, px, py, 12.0, dt);
                    alien_update_anim(a, 5);
                    if (!a->is_preparing && !a->is_attacking && !a->is_stalking)
                    {
                        if (a->is_walking && player_dist <= BLOCK * 1.5)
                            a->chase_runup_timer += dt;
                        else
                        {
                            a->chase_runup_timer -= dt * 0.5;
                            if (a->chase_runup_timer < 0.0)
                                a->chase_runup_timer = 0.0;
                        }
                        if (a->chase_runup_timer >= 0.8 && player_dist <= BLOCK * 1.4 && a->attack_cooldown <= 0.0)
                        {
                            alien_prepare_direct_pounce(cub, a, px, py);
                            a->chase_runup_timer = 0.0;
                            return;
                        }
                    }
                    else
                        a->chase_runup_timer = 0.0;
                }
            }
            else
            {
                if (alien_build_path(cub, a, a->last_seen_x, a->last_seen_y))
                {
                    alien_follow_path(cub, a, a->speed_chase * 0.9, dt);
                    alien_update_anim(a, 6);
                }
                else
                {
                    a->chase_runup_timer = 0.0;
                    a->state = ALIEN_SEARCH;
                    a->state_timer = 2.5;
                    a->target_x = a->last_seen_x;
                    a->target_y = a->last_seen_y;
                }
            }
            break;
        }
        case ALIEN_PREPARING:
            break;
    }

    double moved = sqrt((a->pos->x - prev_x) * (a->pos->x - prev_x) +
                        (a->pos->y - prev_y) * (a->pos->y - prev_y));
    if (moved < 1.0)
        a->stuck_timer += dt;
    else
        a->stuck_timer = 0.0;
    if (a->stuck_timer > 1.5)
    {
        a->stuck_timer = 0.0;
        a->is_crouching = 1;
        a->crouch_timer = 1.0;
        alien_reset_path(a);
        alien_choose_patrol_target(cub, a);
    }
    int cell_mx = (int)(a->pos->x / BLOCK);
    int cell_my = (int)(a->pos->y / BLOCK);
    if (moved < 1.0)
    {
        if (cell_mx == a->stall_cell_mx && cell_my == a->stall_cell_my)
            a->stall_timer += dt;
        else
        {
            a->stall_cell_mx = cell_mx;
            a->stall_cell_my = cell_my;
            a->stall_timer = dt;
        }
    }
    else
    {
        a->stall_cell_mx = cell_mx;
        a->stall_cell_my = cell_my;
        a->stall_timer = 0.0;
    }
    if (a->stall_timer >= 4.0)
    {
        alien_force_unstuck(cub, a);
        a->stall_timer = 0.0;
    }
    a->prev_pos_x = a->pos->x;
    a->prev_pos_y = a->pos->y;
}

/* public helpers */
void alien_cycle_mask_mode(t_cub *cub)
{
    (void)cub;
    if (alien_mask_mode == MASK_AUTO) alien_mask_mode = MASK_MAGENTA;
    else if (alien_mask_mode == MASK_MAGENTA) alien_mask_mode = MASK_BLACK;
    else if (alien_mask_mode == MASK_BLACK) alien_mask_mode = MASK_ALPHA;
    else alien_mask_mode = MASK_AUTO;
    alien_mask_detected = 0;
}

void alien_detect_mask(t_cub *cub)
{
    if (!cub)
        return;
    detect_alien_mask_mode(cub);
    build_alien_mask_map_for_tex(cub, &cub->alien_tex);
}

void alien_toggle_force_transparent(t_cub *cub)
{
    (void)cub; static int enabled=0; enabled = !enabled; alien_replace_bg_color = enabled ? 0x000000 : -1;
}

void alien_build_mask(t_cub *cub)
{
    if (!cub) return;
    /* ensure global detection at least once */
    if (!alien_mask_detected) detect_alien_mask_mode(cub);
    /* build masks for all alien-related textures if they exist */
    build_alien_mask_map_for_tex(cub, &cub->alien_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_back_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_w1_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_w2_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_w3_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_attack_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_bond_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_attack1_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_attack2_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_look_right_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_look_left_tex);
    /* view_mi_face textures not currently loaded - skip mask build */

    /* debug report counts only for primary alien texture to avoid log flood */
    build_and_report(cub, &cub->alien_tex, "alien_tex");
}

int alien_init(t_cub *cub)
{
    if (!cub) return 0;
    if (cub->alien) return 1; /* already initialized */
    t_alien *a = (t_alien*)malloc(sizeof(t_alien));
    if (!a) return 0;
    memset(a, 0, sizeof(t_alien));
    a->pos = (t_cord*)malloc(sizeof(t_cord));
    a->dir = (t_cord*)malloc(sizeof(t_cord));
    if (!a->pos || !a->dir) { free(a->pos); free(a->dir); free(a); return 0; }
    /* place at map spawn if available, otherwise default to near origin */
    if (cub->alien_spawn_x >= 0 && cub->alien_spawn_y >= 0)
    {
        a->pos->x = (double)cub->alien_spawn_x * BLOCK + BLOCK / 2.0;
        a->pos->y = (double)cub->alien_spawn_y * BLOCK + BLOCK / 2.0;
    }
    else
    {
        a->pos->x = BLOCK*2 + BLOCK/2; a->pos->y = BLOCK*2 + BLOCK/2;
    }
    a->dir->x = 0.0; a->dir->y = -1.0;
    a->pounce_vx = 0.0;
    a->pounce_vy = -1.0;
    a->pounce_speed = 300.0;
    a->pounce_target_x = a->pos->x;
    a->pounce_target_y = a->pos->y - BLOCK;
    a->pounce_timer = 0.0;
    a->speed_patrol = 40.0; a->speed_chase = 80.0;
    a->hitbox_radius = 12.0;
    a->fov_deg = 60.0; a->view_distance = BLOCK*8;
    a->state = ALIEN_PATROL;
    a->target_x = a->pos->x;
    a->target_y = a->pos->y;
    a->wall_hit_mx = -1;
    a->wall_hit_my = -1;
    a->wall_hit_prev_mx = -1;
    a->wall_hit_prev_my = -1;
    a->path_goal_mx = -1;
    a->path_goal_my = -1;
    a->path_length = 0;
    a->path_index = 0;
    a->path_recalc_timer = 0.0;
    a->stuck_timer = 0.0;
    a->prev_pos_x = a->pos->x;
    a->prev_pos_y = a->pos->y;
    a->retreat_x = a->pos->x;
    a->retreat_y = a->pos->y;
    a->attack_cooldown = 0.0;
    a->is_walking = 0;
    a->smooth_vx = 0.0;
    a->smooth_vy = 0.0;
    a->smooth_speed = 0.0;
    a->stall_timer = 0.0;
    a->stall_cell_mx = -1;
    a->stall_cell_my = -1;
    a->chase_runup_timer = 0.0;
    a->view_state = ALIEN_VIEW_FRONT;
    alien_reset_path(a);
    alien_choose_patrol_target(cub, a);
    cub->alien = a;
    /* ensure masks exist for visibility */
    alien_build_mask(cub);
    return 1;
}

void alien_free(t_cub *cub)
{
    if (!cub || !cub->alien) return;
    t_alien *a = cub->alien;
    if (a->pos) free(a->pos);
    if (a->dir) free(a->dir);
    free(a);
    cub->alien = NULL;
}

void alien_toggle_debug(t_cub *cub __attribute__((unused)))
{
    /* Debug feature disabled for performance */
}

void alien_toggle_replace_bg(t_cub *cub __attribute__((unused)))
{
    (void)cub;
    if (alien_replace_bg_color == -1) alien_replace_bg_color = 0x000000;
    else alien_replace_bg_color = -1;
}

void alien_dump_corners(t_cub *cub)
{
    int w = cub->alien_tex.w, h = cub->alien_tex.h, bytes = cub->alien_tex.bpp/8;
    int corners[4][2] = {{1,1},{w-2,1},{1,h-2},{w-2,h-2}};
    for (int i=0;i<4;++i)
    {
        int sx = corners[i][0], sy = corners[i][1];
        int idx = sy * cub->alien_tex.sl + sx * bytes;
        unsigned char *p = (unsigned char*)cub->alien_tex.data + idx;
    }
}
            
