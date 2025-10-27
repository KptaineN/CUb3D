/* ************************************************************************** */
/*                                                                            */
/*   alien2.c - AI & Pathfinding logic                                        */
/*                                                                            */
/* ************************************************************************** */

#include "alien_internal.h"

void alien_reset_path(t_alien *a)
{
    if (!a) return;
    a->path_length = 0;
    a->path_index = 0;
    a->path_goal_mx = -1;
    a->path_goal_my = -1;
    a->path_recalc_timer = 0.0;
}

void alien_mark_wall_hit(t_alien *a, int mx, int my)
{
    if (!a) return;
    if (mx == a->wall_hit_mx && my == a->wall_hit_my)
    {
        a->consecutive_hits++;
        return;
    }
    a->wall_hit_prev_mx = a->wall_hit_mx;
    a->wall_hit_prev_my = a->wall_hit_my;
    a->wall_hit_mx = mx;
    a->wall_hit_my = my;
    a->consecutive_hits = 1;
}

static int is_blocking_cell(t_cub *cub, int mx, int my)
{
    if (!cub || !cub->map) return 1;
    if (mx < 0 || my < 0) return 1;
    int map_h = 0;
    while (cub->map[map_h]) map_h++;
    if (my >= map_h) return 1;
    char *row = cub->map[my];
    if (!row) return 1;
    int rowlen = (int)strlen(row);
    if (mx >= rowlen) return 1;
    char c = row[mx];
    if (c == '1' || c == 'P' || c == 'D' || c == ' '
        || c == 'B')
        return 1;
    return 0;
}

int alien_position_clear(t_cub *cub, double x, double y, double radius)
{
    static const double offsets[8][2] = {
        {0.0, 0.0}, {1.0, 0.0}, {-1.0, 0.0}, {0.0, 1.0},
        {0.0, -1.0}, {0.7071, 0.7071}, {-0.7071, 0.7071},
        {0.7071, -0.7071}
    };
    if (!cub) return 0;
    for (int i = 0; i < 8; ++i)
    {
        double sx = x + offsets[i][0] * radius;
        double sy = y + offsets[i][1] * radius;
        int mx = (int)(sx / BLOCK);
        int my = (int)(sy / BLOCK);
        if (is_blocking_cell(cub, mx, my))
            return 0;
    }
    return 1;
}

void alien_plan_retreat(t_cub *cub, t_alien *a)
{
    if (!a)
        return;
    double dirx = -a->pounce_vx;
    double diry = -a->pounce_vy;
    double norm = sqrt(dirx * dirx + diry * diry);
    if (norm < 1e-6)
    {
        dirx = -a->dir->x;
        diry = -a->dir->y;
        norm = sqrt(dirx * dirx + diry * diry);
    }
    if (norm < 1e-6)
    {
        dirx = 0.0;
        diry = -1.0;
        norm = 1.0;
    }
    dirx /= norm;
    diry /= norm;
    double base_x = a->pos->x;
    double base_y = a->pos->y;
    double chosen_x = base_x;
    double chosen_y = base_y;
    double distances[4] = { BLOCK * 4.0, BLOCK * 3.0, BLOCK * 2.0, BLOCK * 1.4 };
    for (int i = 0; i < 4; ++i)
    {
        double tx = base_x + dirx * distances[i];
        double ty = base_y + diry * distances[i];
        if (!cub || alien_position_clear(cub, tx, ty, a->hitbox_radius * 0.8))
        {
            chosen_x = tx;
            chosen_y = ty;
            break;
        }
    }
    a->retreat_x = chosen_x;
    a->retreat_y = chosen_y;
    a->target_x = chosen_x;
    a->target_y = chosen_y;
    a->path_recalc_timer = 0.0;
}

void alien_force_unstuck(t_cub *cub, t_alien *a)
{
    if (!cub || !a || !cub->player || !cub->player->position)
    {
        alien_choose_patrol_target(cub, a);
        return;
    }
    double px = cub->player->position->x;
    double py = cub->player->position->y;
    double dx = px - a->pos->x;
    double dy = py - a->pos->y;
    double len = sqrt(dx * dx + dy * dy);
    double dirx = (len > 1e-6) ? dx / len : a->dir->x;
    double diry = (len > 1e-6) ? dy / len : a->dir->y;
    double fallback_x = a->pos->x;
    double fallback_y = a->pos->y;
    double distances[4] = { BLOCK * 6.0, BLOCK * 5.0, BLOCK * 4.0, BLOCK * 2.5 };
    int found = 0;
    for (int i = 0; i < 4; ++i)
    {
        double tx = px - dirx * distances[i];
        double ty = py - diry * distances[i];
        if (alien_position_clear(cub, tx, ty, a->hitbox_radius * 0.8))
        {
            fallback_x = tx;
            fallback_y = ty;
            found = 1;
            break;
        }
    }
    if (!found)
        alien_choose_patrol_target(cub, a);
    else
    {
        a->target_x = fallback_x;
        a->target_y = fallback_y;
    }
    a->state = ALIEN_SEARCH;
    a->state_timer = 3.5;
    alien_reset_path(a);
    a->path_recalc_timer = 0.0;
    a->stall_cell_mx = -1;
    a->stall_cell_my = -1;
    a->stall_timer = 0.0;
}

t_alien_view_bucket alien_select_view_bucket(t_alien *a, double view_front, double view_side, int moving)
{
    t_alien_view_bucket current = (t_alien_view_bucket)a->view_state;
    double abs_side = fabs(view_side);
    
    /* BACK: dos au joueur */
    if (view_front < -0.50 || (current == ALIEN_VIEW_BACK && view_front < -0.40))
        return ALIEN_VIEW_BACK;
    
    /* SIDE: profil complet (3h ou 9h) */
    if ((abs_side >= 0.70 && view_front < 0.50) ||
        ((current == ALIEN_VIEW_SIDE_LEFT || current == ALIEN_VIEW_SIDE_RIGHT) && 
         abs_side >= 0.60 && view_front < 0.55))
        return (view_side > 0.0) ? ALIEN_VIEW_SIDE_LEFT : ALIEN_VIEW_SIDE_RIGHT;
    
    /* MID: 3/4 face (11h-12h45) */
    if ((view_front >= 0.30 && view_front < 0.80 && abs_side >= 0.15) ||
        ((current == ALIEN_VIEW_MID_LEFT || current == ALIEN_VIEW_MID_RIGHT) &&
         view_front >= 0.20 && view_front < 0.85 && abs_side >= 0.05))
        return (view_side > 0.0) ? ALIEN_VIEW_MID_LEFT : ALIEN_VIEW_MID_RIGHT;
    
    /* WALK: marche vers joueur */
    if ((moving && view_front >= 0.75) || 
        (current == ALIEN_VIEW_WALK && moving && view_front >= 0.65))
        return ALIEN_VIEW_WALK;
    
    /* FRONT: face complète */
    if (view_front >= 0.75 || (current == ALIEN_VIEW_FRONT && view_front >= 0.65))
        return ALIEN_VIEW_FRONT;
    
    return ALIEN_VIEW_FRONT;
}

void alien_apply_player_damage(t_cub *cub)
{
    if (!cub || !cub->player)
        return;
    t_player *p = cub->player;
    if (p->is_dead || p->is_invincible)
        return;
    if (p->health > 0)
        p->health--;
    p->damage_level = (p->damage_level < 3) ? p->damage_level + 1 : 3;
    p->damage_flash_timer = 0.45;
    p->is_invincible = 1;
    p->invincibility_timer = 1.2;
    if (p->health <= 0)
    {
        p->health = 0;
        if (!p->is_dead)
        {
            p->is_dead = 1;
            if (p->death_timer <= 0.0)
                p->death_timer = 3.0;
        }
    }
}

void alien_prepare_direct_pounce(t_cub *cub, t_alien *a, double target_x, double target_y)
{
    (void)cub;
    if (!a)
        return;
    alien_set_pounce_target(a, target_x, target_y);
    a->is_stalking = 0;
    a->is_preparing = 1;
    a->is_attacking = 0;
    a->prepare_timer = ALIEN_PREP_TOTAL_TIME * 0.6;
    if (a->prepare_timer < 0.2)
        a->prepare_timer = 0.2;
    a->state = ALIEN_PREPARING;
    a->state_timer = a->prepare_timer + ALIEN_POUNCE_MAX_DURATION;
    a->stalk_timer = 0.0;
    alien_reset_path(a);
}

void alien_enter_search(t_cub *cub, t_alien *a, double center_x, double center_y)
{
    if (!a)
        return;
    alien_reset_path(a);
    a->state = ALIEN_SEARCH;
    a->state_timer = 4.0;
    a->is_stalking = 0;
    a->is_preparing = 0;
    a->is_attacking = 0;
    a->chase_runup_timer = 0.0;
    double target_x = center_x;
    double target_y = center_y;
    double radius_min = BLOCK * 0.8;
    double radius_max = BLOCK * 3.5;
    for (int attempt = 0; attempt < 12; ++attempt)
    {
        double angle = ((double)rand() / (double)RAND_MAX) * 2.0 * M_PI;
        double radius = radius_min + ((double)rand() / (double)RAND_MAX) * (radius_max - radius_min);
        double tx = center_x + cos(angle) * radius;
        double ty = center_y + sin(angle) * radius;
        if (!cub || alien_position_clear(cub, tx, ty, a->hitbox_radius))
        {
            target_x = tx;
            target_y = ty;
            break;
        }
    }
    a->target_x = target_x;
    a->target_y = target_y;
    a->path_recalc_timer = 0.0;
}
int alien_try_move(t_cub *cub, t_alien *a, double vx, double vy, double speed, double dt, int smooth)
{
    if (!cub || !a || !a->pos) return 0;
    double step = speed * dt;
    if (step <= 0.0) return 0;
    
    double mvx = vx;
    double mvy = vy;
    
    if (smooth)
    {
        double blend = fmin(1.0, dt * 6.0);
        a->smooth_vx = (1.0 - blend) * a->smooth_vx + blend * vx;
        a->smooth_vy = (1.0 - blend) * a->smooth_vy + blend * vy;
        double mag = sqrt(a->smooth_vx * a->smooth_vx + a->smooth_vy * a->smooth_vy);
        if (mag > 1e-6)
        {
            mvx = a->smooth_vx / mag;
            mvy = a->smooth_vy / mag;
        }
        a->smooth_speed = (1.0 - blend) * a->smooth_speed + blend * step;
        step = a->smooth_speed;
    }
    else
    {
        a->smooth_vx = vx;
        a->smooth_vy = vy;
        a->smooth_speed = step;
    }
    
    double nx = a->pos->x + mvx * step;
    double ny = a->pos->y + mvy * step;
    
    /* Tentative 1: Mouvement direct */
    if (alien_position_clear(cub, nx, ny, a->hitbox_radius))
    {
        a->pos->x = nx;
        a->pos->y = ny;
        a->is_walking = 1;
        return 1;
    }
    
    int moved = 0;
    
    /* Tentative 2: Glissement sur X uniquement */
    if (alien_position_clear(cub, a->pos->x + mvx * step, a->pos->y, a->hitbox_radius))
    {
        a->pos->x += mvx * step;
        moved = 1;
    }
    
    /* Tentative 3: Glissement sur Y uniquement */
    if (alien_position_clear(cub, a->pos->x, a->pos->y + mvy * step, a->hitbox_radius))
    {
        a->pos->y += mvy * step;
        moved = 1;
    }
    
    /* Tentative 4: Essayer des directions diagonales - Seulement si pas déjà en train de bouger */
    if (!moved && a->stall_timer < 1.0)  /* Optimisation: skip si trop bloqué */
    {
        /* Réduire le nombre de tentatives de 6 à 4 pour optimiser */
        double angles[4] = {0.3, -0.3, 0.5, -0.5}; /* ±17°, ±28° */
        for (int i = 0; i < 4; i++)
        {
            double angle = angles[i];
            double cos_a = cos(angle);
            double sin_a = sin(angle);
            double rot_vx = mvx * cos_a - mvy * sin_a;
            double rot_vy = mvx * sin_a + mvy * cos_a;
            double test_x = a->pos->x + rot_vx * step;
            double test_y = a->pos->y + rot_vy * step;
            
            if (alien_position_clear(cub, test_x, test_y, a->hitbox_radius))
            {
                a->pos->x = test_x;
                a->pos->y = test_y;
                moved = 1;
                break;
            }
        }
    }
    
    /* Si toujours bloqué, marquer le mur */
    if (!moved)
    {
        int mx = (int)(nx / BLOCK);
        int my = (int)(ny / BLOCK);
        alien_mark_wall_hit(a, mx, my);
    }
    else
    {
        a->is_walking = 1;
    }
    return moved;
}

void alien_face_target(t_alien *a, double tx, double ty, double lerp_speed, double dt)
{
    if (!a || !a->pos || !a->dir) return;
    
    double dx = tx - a->pos->x;
    double dy = ty - a->pos->y;
    double len = sqrt(dx * dx + dy * dy);
    if (len < 1e-6) return;
    
    double target_x = dx / len;
    double target_y = dy / len;
    
    /* Calculer l'angle entre la direction actuelle et la cible */
    double dot = a->dir->x * target_x + a->dir->y * target_y;
    
    /* Optimisation: Si l'alien est bloqué ET déjà bien aligné, ne pas tourner */
    if ((a->stuck_timer > 0.3 || a->stall_timer > 0.3) && dot > 0.92)
        return;
    
    /* Si déjà bien aligné (dot > 0.98 = ~11 degrés), ne pas tourner */
    if (dot > 0.98)
        return;
    
    /* Réduire la vitesse de rotation pour plus de stabilité */
    double blend = lerp_speed * dt;
    if (blend > 1.0) 
        blend = 1.0;
    
    /* Appliquer un facteur de stabilisation supplémentaire */
    blend *= 0.6;
    
    a->dir->x = (1.0 - blend) * a->dir->x + blend * target_x;
    a->dir->y = (1.0 - blend) * a->dir->y + blend * target_y;
    
    double norm = sqrt(a->dir->x * a->dir->x + a->dir->y * a->dir->y);
    if (norm > 1e-6)
    {
        a->dir->x /= norm;
        a->dir->y /= norm;
    }
}

void alien_update_anim(t_alien *a, int threshold)
{
    if (!a) return;
    
    /* Ralentir l'animation quand l'alien est bloqué pour éviter le flickering */
    int adjusted_threshold = threshold;
    if (a->stuck_timer > 0.5 || a->stall_timer > 0.5)
        adjusted_threshold = threshold * 3; /* 3x plus lent quand bloqué */
    
    if (++a->anim_timer >= adjusted_threshold)
    {
        a->anim_timer = 0;
        a->anim_frame = (a->anim_frame + 1) % 4;
    }
}

void alien_choose_patrol_target(t_cub *cub, t_alien *a)
{
    if (!cub || !cub->map || !a) return;
    int map_h = 0;
    int map_w = 0;
    for (int y = 0; cub->map[y]; ++y)
    {
        int len = (int)strlen(cub->map[y]);
        if (len > map_w) map_w = len;
        map_h++;
    }
    if (map_h == 0 || map_w == 0) return;
    double best_x = a->pos->x;
    double best_y = a->pos->y;
    double best_score = -1e9;
    int found = 0;
    double ideal_dist = BLOCK * 7.0; /* Distance idéale augmentée pour plus de stabilité */
    
    for (int attempt = 0; attempt < 64; ++attempt)
    {
        int mx = rand() % map_w;
        int my = rand() % map_h;
        if (is_blocking_cell(cub, mx, my))
            continue;
        double px = mx * BLOCK + BLOCK * 0.5;
        double py = my * BLOCK + BLOCK * 0.5;
        double dx = px - a->pos->x;
        double dy = py - a->pos->y;
        double dist = sqrt(dx * dx + dy * dy);
        
        /* Augmenter la distance minimale pour éviter les changements trop fréquents */
        if (dist < BLOCK * 4.0 || dist > BLOCK * 15.0)
            continue;
        
        double norm = (dist > 1e-6) ? dist : 1.0;
        double dirx = dx / norm;
        double diry = dy / norm;
        
        /* Privilégier fortement la direction actuelle pour stabilité */
        double alignment = a->dir->x * dirx + a->dir->y * diry;
        double dist_score = 1.0 - fabs(dist - ideal_dist) / ideal_dist;
        if (dist_score < -1.0) dist_score = -1.0;
        
        /* Pondération 85% alignement, 15% distance = plus de stabilité directionnelle */
        double score = alignment * 0.85 + dist_score * 0.15;
        
        if (!found || score > best_score)
        {
            best_score = score;
            best_x = px;
            best_y = py;
            found = 1;
        }
    }
    if (!found)
    {
        int fallback_mx = (int)(a->pos->x / BLOCK);
        int fallback_my = (int)(a->pos->y / BLOCK);
        for (int dy = -3; dy <= 3 && !found; ++dy)
        {
            for (int dx = -3; dx <= 3 && !found; ++dx)
            {
                int mx = fallback_mx + dx;
                int my = fallback_my + dy;
                if (is_blocking_cell(cub, mx, my))
                    continue;
                best_x = mx * BLOCK + BLOCK * 0.5;
                best_y = my * BLOCK + BLOCK * 0.5;
                found = 1;
            }
        }
    }
    a->target_x = best_x;
    a->target_y = best_y;
    alien_reset_path(a);
}

int alien_build_path(t_cub *cub, t_alien *a, double target_x, double target_y)
{
    /* FIXME: This function requires ALIEN_MAX_PATH constant and path_points array in t_alien
     * which are not currently defined. Commenting out for now.
     */
    (void)cub;
    (void)a;
    (void)target_x;
    (void)target_y;
    return 0;
}

int alien_follow_path(t_cub *cub, t_alien *a, double speed, double dt)
{
    if (!a || a->path_length <= 0 || a->path_index >= a->path_length)
        return 1;
    /* FIXME: path_points array not defined in t_alien structure
    double tolerance = fmax(6.0, speed * dt * 1.5);
    while (a->path_index < a->path_length)
    {
        t_cord target = a->path_points[a->path_index];
        double dx = target.x - a->pos->x;
        double dy = target.y - a->pos->y;
        double dist = sqrt(dx * dx + dy * dy);
        if (dist < tolerance)
        {
            a->path_index++;
            continue;
        }
        if (dist < 1e-6)
        {
            a->path_index++;
            continue;
        }
        double vx = dx / dist;
        double vy = dy / dist;
        alien_face_target(a, target.x, target.y, 8.0, dt);
        if (!alien_try_move(cub, a, vx, vy, speed, dt, 1))
        {
            alien_reset_path(a);
        }
        return 0;
    }
    */
    (void)cub;
    (void)speed;
    (void)dt;
    return 1;
}

int alien_player_visible(t_cub *cub, t_alien *a, double *out_dist)
{
    if (!cub || !a || !cub->player || !cub->player->position) return 0;
    double px = cub->player->position->x;
    double py = cub->player->position->y;
    double dx = px - a->pos->x;
    double dy = py - a->pos->y;
    double dist = sqrt(dx * dx + dy * dy);
    if (out_dist) *out_dist = dist;
    if (dist > a->view_distance)
        return 0;
    double len = dist;
    if (len < 1e-6)
        return 1;
    double ndx = dx / len;
    double ndy = dy / len;
    double dot = a->dir->x * ndx + a->dir->y * ndy;
    double cos_fov = cos(a->fov_deg * (M_PI / 180.0));
    if (dot < cos_fov)
        return 0;
    if (!line_of_sight(cub, a, px, py))
        return 0;
    return 1;
}

/* simplified movement handlers omitted for brevity: keep existing implementations in other files if present */
