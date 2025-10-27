/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:46:22 by adi-marc          #+#    #+#             */
/*   Updated: 2025/10/21 12:19:29 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CUB3D_H
# define CUB3D_H

// Includes

# include "../includes/minilibx-linux/mlx.h"
# include "../libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <math.h>
# include <stdbool.h>
# include <ctype.h>

// Defines

# define M_PI 3.14159265358979323846

# define WIDTH 800
# define HEIGHT 600

#define BLOCK 40

// Texture indices
#define TEX_NORTH 0
#define TEX_SOUTH 1
#define TEX_EAST  2
#define TEX_WEST  3

// Weapon indices
#define WEAPON_HAMMER 0
#define WEAPON_MP 1
#define WEAPON_PISTOL 2
#define WEAPON_FISTS 3

// Definition des codes des touches
#define W 119
#define Z 122
#define DOWN 65364
#define UP 65362
#define A 97
#define Q 113
#define S 115
#define D 100
#define LEFT 65361
#define RIGHT 65363
#define ESC_KEY 65307
#define KEY_COMMA 44
#define KEY_PERIOD 46
#define KEY_LESS 60
#define KEY_GREATER 62

typedef struct s_player t_player;
typedef struct s_color  t_color;
typedef struct s_cord   t_cord;

// Structure pour les textures (doit être définie avant s_cub qui l'utilise)
typedef struct s_tex {
    void *img;     // pointeur image MLX
    char *data;    // data address (pixels)
    int   w, h;    // largeur, hauteur
    int   bpp;     // bits par pixel (souvent 32)
    int   sl;      // size_line (octets par ligne, padding inclus)
    int   endian;  // endianness signalée par MLX
    /* optional cached mask built for this texture (allocated w*h bytes) */
    unsigned char *mask_cache;
    int mask_cache_w;
    int mask_cache_h;
} t_tur;

// Structure utilisée lors de la projection d'une bande de mur
typedef struct s_distortion_data {
    double corrected_distance;    // distance perpendiculaire au plan caméra
    int    raw_wall_height;       // hauteur calculée à partir de la distance
    int    corrected_wall_height; // éventuellement modifiée (actuellement identique)
    int    y_start;               // début Y du mur à l'écran
    int    y_end;                 // fin Y du mur à l'écran
} t_distortion_data;

// Alien state machine
typedef enum e_alien_state
{
    ALIEN_PATROL,
    ALIEN_CHASE,
    ALIEN_ATTACK,
    ALIEN_STALKING,
    ALIEN_SEARCH,
    ALIEN_PREPARING
}   t_alien_state;

// Alien view bucket for texture selection
typedef enum e_alien_view_bucket
{
    ALIEN_VIEW_FRONT,
    ALIEN_VIEW_MID_LEFT,
    ALIEN_VIEW_MID_RIGHT,
    ALIEN_VIEW_SIDE_LEFT,
    ALIEN_VIEW_SIDE_RIGHT,
    ALIEN_VIEW_BACK,
    ALIEN_VIEW_WALK
}   t_alien_view_bucket;

// Structure pour les impacts de balles sur les murs - VERSION SIMPLIFIÉE
#define MAX_BULLET_IMPACTS 20
typedef struct s_bullet_impact
{
    int     active;         /* 1 si l'impact est actif */
    int     screen_x;       /* Position X à l'écran */
    int     screen_y;       /* Position Y à l'écran */
    double  timer;          /* Temps restant avant disparition (15s) */
}   t_bullet_impact;

#define HAMMER_SPARK_FRAMES 4
#define MAX_HAMMER_SPARKS 8
typedef struct s_hammer_spark
{
    int     active;
    double  x;
    double  y;
    double  timer;
    double  duration;
}   t_hammer_spark;

typedef struct s_panel_trigger
{
    int map_x;
    int map_y;
    int activated;
}   t_panel_trigger;

typedef struct s_door_segment
{
    int map_x;
    int map_y;
    int open;
}   t_door_segment;

typedef enum e_barrel_state
{
    BARREL_INTACT,
    BARREL_EXPLODING,
    BARREL_DESTROYED
}   t_barrel_state;

typedef struct s_barrel
{
    double          x;              /* center position in pixels */
    double          y;
    int             map_x;          /* map cell coordinate */
    int             map_y;
    t_barrel_state  state;
    double          anim_timer;     /* elapsed time within current state */
    int             anim_frame;     /* current explosion frame (0-based) */
    double          explosion_radius;
}   t_barrel;

typedef enum e_pickup_type
{
    PICKUP_MP_AMMO,
    PICKUP_PISTOL_AMMO,
    PICKUP_HEALTH,
    PICKUP_WEAPON_PISTOL,
    PICKUP_WEAPON_MP,
    PICKUP_WEAPON_HAMMER
}   t_pickup_type;

typedef struct s_pickup
{
    double          x;
    double          y;
    int             map_x;
    int             map_y;
    t_pickup_type   type;
    int             active;
}   t_pickup;

// Structure générale qui contient toutes les informations de l'environnement du programme
typedef struct s_cub
{
    char    *no_path;
    char    *so_path;
    char    *we_path;
    char    *ea_path;
    t_color *f_color; // During parsing ft_convert_color() -> transforme la string en struct
    t_color *c_color;
    char    **file;
    char    **map_start;
    char    **map;
    void    *mlx;
    void    *mlx_window;
    void    *mlx_image;
    char    *mlx_data;
    int mlx_bpp;
    int mlx_line_size;
    int mlx_endian;
    t_player    *player;
    t_tur      textur[4];
    // ciel (skybox) et sol (floor)
    t_tur sky;     // grande image panoramique en X
    t_tur floor;   // texture sol (pavés, herbe, etc.)
    t_tur panel_tex;   /* texture pour les panneaux (P) */
    t_tur door_tex;    /* texture pour les portes (D) */
    t_tur barrel_tex;  /* texture pour barils intacts (B) */
    t_tur barrel_explosion[11]; /* barrel1.xpm à barrel11.xpm */
    t_tur wood_tex;    /* texture pour les murs destructibles (6) */
    t_tur pickup_medkit_tex; /* texture pour soin (H) */
    t_tur pickup_ammo_tex;   /* texture pour munition (M) */
    t_tur pistol_ammo_tex;   /* texture pour munition de pistolet */
    t_tur mp_reload_tex;     /* overlay rechargement MP */
    t_tur pistol_idle_tex;   /* pistolet idle */
    t_tur pistol_fire_tex;   /* pistolet tir */
    t_tur fists_idle_tex;    /* texture poings idle */
    t_tur fists_touch_tex;   /* texture poings attaque */
    t_tur kick_tex[2];       /* textures coup de pied */
    t_tur mini_pistol_tex;   /* icone mini pistolet */
    t_tur mini_mp_tex;       /* icone mini MP */
    t_tur mini_hammer_tex;   /* icone mini marteau */
    t_tur digit_tex[10];     /* digits 0..9 pour HUD ammo */
    /* texture dédiée pour les entités (ex: alien) */
    t_tur alien_tex;
    /* optional alternate alien view (back) */
    t_tur alien_back_tex;
    /* alien additional view / action textures (moved from ttur -> textures/alien) */
    t_tur alien_w1_tex;    /* crouch-walk frame 1 (front) */
    t_tur alien_w2_tex;    /* crouch-walk frame 2 (front) */
    t_tur alien_w3_tex;    /* crouch-walk frame 3 (front) */
    t_tur alien_attack_tex; /* attack (bite) texture */
    t_tur alien_bond_tex;  /* bondissement / pounce texture */
    /* additional attack frames shown after pounce */
    t_tur alien_attack1_tex; /* attack frame 1 */
    t_tur alien_attack2_tex; /* attack frame 2 */
    /* side/profile views */
    t_tur alien_look_right_tex; /* profile view looking right */
    t_tur alien_look_left_tex;  /* profile view looking left */
    /* mid-angle face views for nicer profiling */
    t_tur view_mi_face_left1;
    t_tur view_mi_face_left2;
    t_tur view_mi_face_right1;
    t_tur view_mi_face_right2;
    /* texture used as inner background for the circular minimap */
    t_tur minimap_tex;
    /* texture used for wall tiles inside the minimap (optional XPM) */
    t_tur minimap_wall_tex;
    /* minimap player top-view icon */
    t_tur minimap_player_tex;
    /* HUD overlays (barres de vie et effets de dégâts) */
    t_tur hud_full_hp;      /* barre 4 coeurs (verte) */
    t_tur hud_3hp;          /* barre 3 coeurs (jaune) */
    t_tur hud_2hp;          /* barre 2 coeurs (orange) */
    t_tur hud_1hp;          /* barre 1 coeur (rouge) */
    t_tur hud_lil_dega;     /* petit flash rouge */
    t_tur hud_mid_dega;     /* moyen flash rouge */
    t_tur hud_big_dega;     /* grand flash rouge */
    t_tur hud_game_over;    /* écran game over */
    /* Weapon textures */
    t_tur hammer[6];        /* hammer_1.xpm à hammer_6.xpm */
    t_tur hammer_spark_tex[HAMMER_SPARK_FRAMES]; /* spark frames marteau */
    t_tur mp_idle;          /* mp1.xpm */
    t_tur mp_aim;           /* mp2.xpm */
    t_tur mp_fire1;         /* fire_mp1.xpm */
    t_tur mp_fire2;         /* fire_mp2.xpm */
    /* Blood effect textures */
    t_tur blood_alien[6];   /* blood_alien1.xpm à blood_alien6.xpm */
    /* AI entities */
    struct s_alien *alien;
    /* alien spawn position (from 'X' in map, -1 if not set) */
    int alien_spawn_x;
    int alien_spawn_y;
    /* player initial spawn in pixels (stored at player_init) */
    double player_spawn_x;
    double player_spawn_y;
    double player_spawn_angle;
    int    player_spawn_saved; /* 1 if spawn info saved */
    /* game over fade alpha (0.0..1.0) controlled by HUD update */
    double game_over_alpha;
    /* minimap visibility toggle */
    int show_minimap;
    /* minimap smooth-follow state: view center in map cells (float), smoothing alpha and deadzone */
    double minimap_view_x;
    double minimap_view_y;
    double minimap_smooth_alpha; /* 0..1 lerp factor per frame */
    double minimap_deadzone; /* cells */
    /* ephemeral cached player state used by the minimap when opened */
    double minimap_cached_px; /* player x in map cells (fractional) */
    double minimap_cached_py; /* player y in map cells (fractional) */
    double minimap_cached_dirx; /* player facing vector x (cos) */
    double minimap_cached_diry; /* player facing vector y (sin) */
    int    minimap_cache_open; /* boolean: 1 if cache is active while minimap shown */
    /* per-column depth buffer (perpendicular distances) used for sprite occlusion */
    double zbuffer[WIDTH];
    /* bullet impacts on walls */
    t_bullet_impact bullet_impacts[MAX_BULLET_IMPACTS];
    int bullet_impact_count;
    t_hammer_spark hammer_sparks[MAX_HAMMER_SPARKS];
    /* puzzle system: panels & doors */
    t_panel_trigger *panels;
    int panel_count;
    int panels_activated;
    t_door_segment *doors;
    int door_count;
    /* explosive barrels */
    t_barrel        *barrels;
    int             barrel_count;
    /* pickups */
    t_pickup        *pickups;
    int             pickup_count;
}   t_cub;

// Structure qui contient toutes les informations du joueur 
typedef struct s_player
{
    t_cord  *position;
    t_cord  *direction;
    t_cord  *plane; // chercher ce que c'est concrétement
    double  fov;
    double  current_fov;    /* FOV actuel (change quand on court) */
    double  base_fov;       /* FOV par défaut */
    double  run_fov_delta;  /* augmentation de FOV quand on court */
    double  move_frame;     /* vitesse courante (pixels par frame) */
    double  walk_speed;
    double  run_speed;
    double  crouch_speed;
    double  rotation_frame;
    double  sin_angle;
    double  cos_angle;
    double  angle;
    /* minimap controlled by cub->show_minimap (toggle on 'm') */
    bool    key_up;
    bool    key_down;
    bool    key_left;
    bool    key_right;
    bool    left_rotate;
    bool    right_rotate;
    double  radius; // chercher ce que c'est concrétement
    /* movement states */
    bool    is_running;
    bool    is_crouching;
    /* camera height in pixels: base (constant) and current (runtime interpolated) */
    double  eye_height_base;        /* valeur de référence (pixels) */
    double  eye_height_current;     /* valeur utilisée par le rendu, lissée */
    double  crouch_eye_height_pixels; /* valeur d'accroupi basée sur la base */
    /* smooth half-turn animation */
    bool    is_turning;             /* true pendant l'animation du demi-tour */
    double  turn_target_angle;      /* angle cible à atteindre */
    double  turn_speed;             /* vitesse de rotation rapide du demi-tour (rad/frame) */
    /* système de combat */
    int     health;                 /* points de vie (3 cœurs max) */
    int     max_health;             /* vie maximale (3) */
    double  hitbox_radius;          /* rayon de la hitbox circulaire */
    double  invincibility_timer;    /* temps d'invincibilité après être touché */
    int     is_invincible;          /* 1 si invincible temporairement */
    /* effets visuels de dégâts */
    double  damage_flash_timer;     /* timer pour affichage du flash rouge */
    int     damage_level;           /* niveau de dégâts cumulés (1=lil, 2=mid, 3=big) */
    /* game over state */
    int     is_dead;                /* 1 si le joueur est mort */
    double  death_timer;            /* timer pour le game over (10 secondes) */
    /* weapon system */
    int     current_weapon;         /* voir defines WEAPON_* */
    int     is_attacking;           /* 1 si en train d'attaquer/tirer */
    double  attack_timer;           /* timer pour l'animation d'attaque */
    int     attack_frame;           /* frame actuelle de l'animation */
    int     is_aiming;              /* 1 si en train de viser (clic droit) */
    int     is_firing;              /* 1 si Space maintenu enfoncé (MP) */
    double  fire_timer;             /* timer pour l'animation de tir */
    double  weapon_bob_timer;       /* timer pour balancement pendant la course */
    double  weapon_bob_offset;      /* offset de balancement actuel */
    int     ammo_clip;              /* balles restantes dans le chargeur */
    int     ammo_reserve;           /* balles en réserve */
    int     ammo_clip_size;         /* capacité du chargeur */
    int     is_reloading;           /* flag rechargement en cours */
    double  reload_timer;           /* timer rechargement */
    double  mp_reload_overlay;      /* timer overlay reload MP */
    int     pistol_clip;            /* balles pistolet */
    int     pistol_reserve;         /* réserve pistolet */
    int     pistol_clip_size;       /* capacité chargeur pistolet */
    int     pistol_is_reloading;    /* 1 si pistolet recharge */
    double  pistol_reload_timer;    /* timer reload pistolet */
    double  pistol_fire_timer;      /* timer texture tir pistolet */
    double  fist_attack_timer;      /* timer animation poings */
    double  kick_timer;             /* timer animation coup de pied */
    double  pickup_touch_timer;     /* overlay pickup feedback */
    int     has_hammer;             /* 1 si marteau obtenu */
    int     has_mp;                 /* 1 si MP obtenu */
    int     has_pistol;             /* 1 si pistolet obtenu */
    /* impact effect */
    int     show_bullet_impact;     /* 1 si afficher l'impact de balle */
    double  bullet_impact_timer;    /* timer pour l'effet d'impact */
    int     bullet_impact_x;        /* position X de l'impact à l'écran */
    int     bullet_impact_y;        /* position Y de l'impact à l'écran */
}   t_player;

// Structure qui définit les couleurs en format RGB
typedef struct s_color
{
    char    *string;
    unsigned char   red;
    unsigned char   green;
    unsigned char   blue;
}   t_color;

typedef struct s_cord
{
    double  x;
    double  y;
}   t_cord;

typedef struct s_dda_vars
{
    double pos_x, pos_y;
    int32_t map_x, map_y;
    double delta_x, delta_y;
    double side_x, side_y;
    int32_t step_x, step_y;
    int side;
    double distance;
}   t_dda_vars;

typedef struct s_alien
{
    t_cord  *pos;               /* position en pixels */
    t_cord  *dir;               /* vecteur direction normalisé */
    double   speed_patrol;      /* vitesse patrouille (px/frame) */
    double   speed_chase;       /* vitesse en poursuite */
    double   fov_deg;           /* demi-FOV en degrés (cone) */
    double   view_distance;     /* distance max de détection */
    t_alien_state state;        /* état courant */
    double   state_timer;       /* timer pour hystérésis (sec) */
    double   last_seen_x;       /* position du joueur vue */
    double   last_seen_y;
    int      anim_frame;        /* index animation */
    int      anim_timer;        /* compteur frame animation */
    /* collision tracking pour comportement intelligent de patrouille */
    int      wall_hit_mx;       /* coordonnée X du dernier mur touché */
    int      wall_hit_my;       /* coordonnée Y du dernier mur touché */
    int      wall_hit_prev_mx;  /* coordonnée X du mur touché avant */
    int      wall_hit_prev_my;  /* coordonnée Y du mur touché avant */
    int      consecutive_hits;  /* nombre de hits consécutifs sur même paire de murs */
    int      is_rotating;       /* 1 si en train de tourner pour choisir direction */
    double   rotation_timer;    /* timer pour rotation lente */
    double   rotation_angle;    /* angle de rotation accumulé */
    double   target_x;          /* position cible pour aller au milieu */
    double   target_y;
    /* nouvelles mécaniques avancées */
    double   prepare_timer;     /* timer pour préparation avant bond */
    double   stalk_timer;       /* timer pour traque lente */
    int      is_preparing;      /* 1 si en préparation pour bondir */
    int      is_stalking;       /* 1 si en mode traque autour d'un mur */
    double   last_sensed_x;     /* dernière position sentie du joueur */
    double   last_sensed_y;
    double   slow_rotation;     /* angle de rotation lente vers proie */
    /* système d'attaque et cooldown */
    int      has_attacked;      /* 1 si vient d'effectuer une attaque */
    double   attack_cooldown;   /* timer de cooldown après attaque (6 sec) */
    double   retreat_x;         /* position cible de retraite après attaque */
    double   retreat_y;
    /* hitbox pour collisions/combat */
    double   hitbox_radius;     /* rayon de la hitbox circulaire */
    /* crouch / flee behaviour when stuck in 4-cell corner */
    int      is_crouching;      /* 1 si en mode accroupi/marche (utilise w1/w2/w3) */
    double   crouch_timer;      /* temps restant en mode accroupi */
    /* attack animation flag */
    int      is_attacking;      /* 1 si en train d'afficher l'animation d'attaque */
    double   attack_anim_timer; /* durée restante de l'animation d'attaque */
    /* pounce mechanics */
    double   pounce_vx;
    double   pounce_vy;
    double   pounce_speed;
    double   pounce_target_x;
    double   pounce_target_y;
    double   pounce_timer;
    /* pathfinding state */
    int      path_goal_mx;
    int      path_goal_my;
    int      path_length;
    int      path_index;
    double   path_recalc_timer;
    double   stuck_timer;
    double   prev_pos_x;
    double   prev_pos_y;
    /* smooth movement state */
    int      is_walking;
    double   smooth_vx;
    double   smooth_vy;
    double   smooth_speed;
    double   stall_timer;
    int      stall_cell_mx;
    int      stall_cell_my;
    double   chase_runup_timer;
    t_alien_view_bucket view_state;
    /* anti-blocking timers */
    double   post_attack_timer;  /* timer post-attaque pour forcer retour PATROL */
    double   idle_timer;         /* timer d'inactivité (3s) */
    double   last_move_time;     /* temps depuis dernier mouvement (5s) */
    t_alien_state idle_state_check; /* état utilisé pour vérifier idle timeout */
    /* MP weapon stun system */
    int      is_stunned;         /* 1 si alien est immobilisé par MP */
    double   stun_timer;         /* durée du stun (2.0s par défaut) */
    /* Blood effect when hit */
    int      show_blood;         /* 1 si effet de sang actif */
    double   blood_timer;        /* timer pour animation de sang */
    int      blood_frame;        /* frame actuelle de l'animation de sang (0-5) */
}   t_alien;

/* Alien API */
int     alien_init(t_cub *cub);
void    alien_update(t_cub *cub, double dt);
void    alien_free(t_cub *cub);
void    draw_alien_sprite(t_cub *cub);
void    alien_toggle_debug(t_cub *cub);
void    alien_cycle_mask_mode(t_cub *cub);
void    alien_detect_mask(t_cub *cub);
void    alien_toggle_replace_bg(t_cub *cub);
void    alien_dump_corners(t_cub *cub);
void    alien_toggle_force_transparent(t_cub *cub);
void    alien_trigger_pounce(t_cub *cub);
void    alien_apply_stun(t_cub *cub, double duration);
int     alien_check_hit_by_mp(t_cub *cub);

/* HUD API */
int     load_hud_textures(t_cub *cub);
void    draw_health_bar(t_cub *cub);
void    draw_damage_flash(t_cub *cub);
void    draw_game_over(t_cub *cub);
void    cleanup_hud_textures(t_cub *cub);
void    hud_update_reload(t_cub *cub, double dt);
void    hud_update(t_cub *cub, double dt);
void    hud_draw_ammo_counter(t_cub *cub);
void    hud_draw_weapon_icons(t_cub *cub);

// Prototypes

// src/parser/parser.c
int ft_cub_parser(int argc, char **argv, t_cub *cub);

// src/parser/parser_check_elements.c
int	check_elements(t_cub *cub);
int	check_map(t_cub *cub);
int	line_is_empty(char *line);
int	count_map_lines(char **start);
int	check_spawn(char **padded);
int	find_alien_spawn(char **map, int *spawn_x, int *spawn_y);

// src/parser/parser_utils.c
int ft_lastchars(const char *s, const char *suffix, size_t i);

// src/parser/parser_check_elements.c
const char *skip_spaces(const char *s);

// src/parser/parser_color_checker.c
int	check_color(char *color);
int	ft_convert_color(t_color *color);
void retrieve_colors(t_cub *cub);
int valid_colors(t_cub *cub);

// src/parser/parser_path_checker.c
int	check_texture_paths(t_cub *cub);
int is_path_valid(t_cub *cub);

// src/parser/check_parse_map.c
int is_map_valid(t_cub *cub);
int allocate_map_buffer(t_cub *cub, char ***out, int pad);

// src/parser/wall_checker.c
int wall_check(char **buffer_map);

// src/parser/parser_map.c
void    free_full_map(char **map);
int map_fail(t_cub *cub, const char *msg);

// src/player/player_init.c
int	player_init(t_cub *cub);
t_cord	*set_direction(char spawn);
t_cord	*get_position(char **map);
void	set_plane(t_player *player);
void    find_player_pos(t_cub *cub);
void    find_player_dir(t_cub *cub);
void    find_player_plane(t_player *p);

// src/player/player_init_utils.c
double  deg_to_radiant(double deg);
int     player_allocate(t_cub *cub);

// src/player/player_utils.c
void	player_rotate_left(t_player *player);
void	player_rotate_right(t_player *player);
bool    player_is_moving(t_player *p);

// src/player/player_move.c
void	move_player(t_cub *cub);
void    player_update(t_cub *cub, double dt);
void    update_player_trig(t_cub *cub);

// src/player/weapon.c
void    weapon_init(t_player *p);
void    weapon_update(t_player *p, double dt);
void    weapon_attack_hammer(t_player *p);
int     weapon_fire_mp(t_player *p);
void    weapon_toggle_aim(t_player *p, int aiming);
void    weapon_switch(t_player *p);
t_tur   *weapon_get_texture(t_cub *cub);
void    weapon_draw(t_cub *cub);
int     weapon_mp_hits_alien(t_cub *cub);
void    weapon_add_wall_impact(t_cub *cub, double x, double y);
void    weapon_update_impacts(t_cub *cub, double dt);
void    weapon_draw_crosshair(t_cub *cub);
void    weapon_draw_wall_impacts(t_cub *cub);
void    weapon_reload(t_player *p);
int     weapon_fire_pistol(t_player *p);
void    weapon_punch(t_cub *cub);
void    weapon_kick(t_cub *cub);
void    weapon_break_forward_soft_wall(t_cub *cub);
void    weapon_auto_fire(t_cub *cub);
void    weapon_process_shot_result(t_cub *cub, int shot_fired);
void    weapon_update_sparks(t_cub *cub, double dt);
void    weapon_draw_hammer_sparks(t_cub *cub);
void    hammer_spawn_panel_spark(t_cub *cub, double world_x, double world_y);

/* Barrel system */
int     barrel_system_init(t_cub *cub);
void    barrel_system_destroy(t_cub *cub);
void    barrel_update(t_cub *cub, double dt);
void    barrel_render(t_cub *cub);
int     barrel_handle_shot(t_cub *cub);
int     barrel_try_push(t_cub *cub, int map_x, int map_y, int dir_x, int dir_y);

/* Pickup system */
int     pickup_system_init(t_cub *cub);
void    pickup_system_destroy(t_cub *cub);
void    pickup_update(t_cub *cub);
void    pickup_render(t_cub *cub);

// src/game/doors.c
int     door_system_init(t_cub *cub);
void    door_system_destroy(t_cub *cub);
void    door_try_activate(t_cub *cub);

// src/game/game_init.c
int	window_init(t_cub *cub);

// src/game/game_keys.c
int	key_press(int keycode, t_cub *cub);
int	key_release(int keycode, t_cub *cub);
int key_drop(int keycode, t_cub *cub);

// src/game/game_cleanup.c
int	cleanup(t_cub *cub);
void    cleanup_all(t_cub *cub);
int on_destroy(t_cub *cub);

// src/textures/textures.c
int     init_textures(t_cub *cub);
t_tur  *pick_face(t_cub *cub, int side, double rx, double ry, char tile);
int     compute_tex_x(t_cub *cub, t_tur *tex, int side, double dist, double rx, double ry);

// src/textures/skybox.c
void    draw_skybox(t_cub *cub);

// src/textures/floor.c
void    draw_floor(t_cub *cub);

// src/game/drawing.c
void	pixel_to_image(t_cub *cub, int x, int y, int color);
t_cord	make_ray(int x, int width, t_cord dir, t_cord plane);
void	draw_column(t_cub *cub, int x, double rx, double ry);
void    clear_image(t_cub *cub);
void    put_pixel(int x, int y, int color, t_cub *cub);

// src/game/dda.c
double  perform_dda(t_cub *cub, double ray_dir_x, double ray_dir_y, int *side, char *hit_tile);

// src/game/render_frame.c
void    render_frame(t_cub *cub);

// src/game/minimap.c
void draw_minimap(t_cub *cub);

// src/game/launcher.c
int	launcher(t_cub *cub);
int start_game(t_cub *cub);

// src/init.c
void	init_values(t_cub *cub);
void    init_mlx_data(t_cub *cub);

#endif
