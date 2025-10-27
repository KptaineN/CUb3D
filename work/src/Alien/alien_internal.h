/* ************************************************************************** */
/*                                                                            */
/*   alien_internal.h - Internal header for alien module                     */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALIEN_INTERNAL_H
# define ALIEN_INTERNAL_H

# include "../../includes/cub3d.h"
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <math.h>

/* Timing configuration for pre-attack staging */
# define ALIEN_PREP_TOTAL_TIME 0.65
# define ALIEN_PREP_CROUCH_WINDOW 0.22
# define ALIEN_POUNCE_MAX_DURATION 0.85
# define ALIEN_ATTACK_SEQUENCE_DURATION 1.0
# define ALIEN_ATTACK_PHASE_SPLIT 0.5

/* Mask mode enumeration */
typedef enum e_mask_mode
{
	MASK_AUTO = 0,
	MASK_MAGENTA = 1,
	MASK_BLACK = 2,
	MASK_ALPHA = 3
}	t_mask_mode;

/* Global state variables (shared across alien modules) */
extern int g_alien_debug_show_raw;
extern t_mask_mode g_alien_mask_mode;
extern int g_alien_mask_detected;
extern t_mask_mode g_alien_detected_mode;
extern int g_alien_bg_r;
extern int g_alien_bg_g;
extern int g_alien_bg_b;
extern int g_alien_replace_bg_color;

/* ========== alien1.c - Rendering & Textures ========== */

/* Mask detection and building */
void	detect_alien_mask_mode(t_cub *cub);
void	build_alien_mask_map_for_tex(t_cub *cub, t_tur *tex);
void	build_and_report(t_cub *cub, t_tur *tex, const char *name);

/* Sprite drawing */
void	draw_alien_sprite_texture_with(t_cub *cub, t_tur *tex,
			int dx0, int dx1, int dy0, int dy1, double transformY);

/* View selection */
t_alien_view_bucket	alien_select_view_bucket(t_alien *a,
						double view_front, double view_side, int moving);

/* Collision & visibility */
int		line_of_sight(t_cub *cub, t_alien *a, double px, double py);
int		alien_position_clear(t_cub *cub, double x, double y, double radius);

/* ========== alien2.c - AI & Pathfinding ========== */

/* Pathfinding */
void	alien_reset_path(t_alien *a);
void	alien_choose_patrol_target(t_cub *cub, t_alien *a);
void	alien_mark_wall_hit(t_alien *a, int mx, int my);
void	alien_plan_retreat(t_cub *cub, t_alien *a);
void	alien_force_unstuck(t_cub *cub, t_alien *a);

/* State machine logic */
void	alien_update_patrol(t_cub *cub, t_alien *a, double dt);
void	alien_update_chase(t_cub *cub, t_alien *a, double dt);
void	alien_update_attack(t_cub *cub, t_alien *a, double dt);

/* Player detection */
int		alien_can_see_player(t_cub *cub, t_alien *a);
int		alien_player_visible(t_cub *cub, t_alien *a, double *out_dist);

/* Search behavior */
void	alien_enter_search(t_cub *cub, t_alien *a,
			double center_x, double center_y);

/* AI helpers */
void	alien_update_anim(t_alien *a, int threshold);
void	alien_face_target(t_alien *a, double tx, double ty,
			double lerp_speed, double dt);
int		alien_build_path(t_cub *cub, t_alien *a,
			double target_x, double target_y);
int		alien_follow_path(t_cub *cub, t_alien *a,
			double speed, double dt);
int		alien_try_move(t_cub *cub, t_alien *a, double vx,
			double vy, double speed, double dt, int smooth);

/* ========== alien3.c - Combat & Physics ========== */

/* Pounce mechanics */
void	alien_set_pounce_target(t_alien *a, double tx, double ty);
void	alien_prepare_direct_pounce(t_cub *cub, t_alien *a,
			double target_x, double target_y);
void	alien_trigger_pounce(t_cub *cub);

/* Attack & damage */
void	alien_finish_attack(t_cub *cub, t_alien *a, int hit_player);
void	alien_apply_player_damage(t_cub *cub);

/* Collision detection */
int		alien_check_collision(t_cub *cub, double x, double y);

/* Movement physics */
void	alien_move_with_collision(t_cub *cub, t_alien *a,
			double dx, double dy);

#endif
