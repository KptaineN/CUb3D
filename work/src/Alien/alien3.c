/* ************************************************************************** */
/*                                                                            */
/*   alien3.c - Combat mechanics & Physics                                    */
/*                                                                            */
/* ************************************************************************** */

#include "alien_internal.h"

void alien_set_pounce_target(t_alien *a, double tx, double ty)
{
    if (!a || !a->pos || !a->dir)
        return;
    a->pounce_target_x = tx;
    a->pounce_target_y = ty;
    double dx = tx - a->pos->x;
    double dy = ty - a->pos->y;
    double len = sqrt(dx * dx + dy * dy);
    if (len < 1e-6)
    {
        /* fallback on current facing direction when target is extremely close */
        dx = a->dir->x;
        dy = a->dir->y;
        len = sqrt(dx * dx + dy * dy);
        if (len < 1e-6)
            len = 1.0;
    }
    a->pounce_vx = dx / len;
    a->pounce_vy = dy / len;
}

void alien_finish_attack(t_cub *cub, t_alien *a, int hit_player)
{
    if (!a)
        return;
    a->is_attacking = 0;
    a->pounce_timer = 0.0;
    if (hit_player)
    {
        a->has_attacked = 1;
        a->attack_anim_timer = ALIEN_ATTACK_SEQUENCE_DURATION;
        if (a->attack_cooldown < 4.5)
            a->attack_cooldown = 4.5;
        alien_apply_player_damage(cub);
    }
    else
    {
        a->has_attacked = 0;
        a->attack_anim_timer = 0.0;
        if (a->attack_cooldown < 2.5)
            a->attack_cooldown = 2.5;
    }
    
    /* Activer le timer post-attaque pour forcer retour en patrouille si bloqué */
    a->post_attack_timer = 4.0; /* 4 secondes pour chercher, puis retour PATROL */
    
    alien_plan_retreat(cub, a);
    a->state = ALIEN_SEARCH;
    a->state_timer = 3.0;
    alien_reset_path(a);
    
}

/* Apply stun effect to alien when hit by MP */
void alien_apply_stun(t_cub *cub, double duration)
{
    if (!cub || !cub->alien)
        return;
    
    t_alien *a = cub->alien;
    
    a->is_stunned = 1;
    a->stun_timer = duration;
    
    /* Activer l'effet de sang */
    a->show_blood = 1;
    a->blood_timer = 0.0;
    a->blood_frame = 0;
    
    /* Reset velocities */
    a->smooth_vx = 0.0;
    a->smooth_vy = 0.0;
    a->pounce_vx = 0.0;
    a->pounce_vy = 0.0;
    
    /* Cancel attack if attacking */
    if (a->is_attacking)
    {
        a->is_attacking = 0;
        a->pounce_timer = 0.0;
        a->attack_anim_timer = 0.0;
    }
    
}

/* Check if MP shot hits alien (called when firing) */
int alien_check_hit_by_mp(t_cub *cub)
{
    if (!cub || !cub->player || !cub->alien)
        return 0;
    
    /* Already stunned, don't stack */
    if (cub->alien->is_stunned)
        return 0;
    
    /* Use the weapon hit detection */
    int hit = weapon_mp_hits_alien(cub);
    
    if (hit)
    {
        /* Apply 2 second stun */
        alien_apply_stun(cub, 2.0);
    }
    
    return hit;
}
