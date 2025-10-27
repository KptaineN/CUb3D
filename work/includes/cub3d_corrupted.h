/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                          /* attack animation flag */
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
}   t_alien;  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:46:22 by adi-marc          #+#    #+#             */
/*   Updated: 2025/10/09 18:12:50 by nkief            ###   ########.fr       */
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
#include <ctype.h>
// Defines


# define M_PI 3.14159265358979323846

# define WIDTH 800
# define HEIGHT 600


#define BLOCK 40

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
    /* texture used as inner background for the circular minimap */
    t_tur minimap_tex;
    /* texture used for wall tiles inside the minimap (optional XPM) */
    t_tur minimap_wall_tex;
    /* HUD overlays (barres de vie et effets de dégâts) */
    t_tur hud_full_hp;      /* barre 4 coeurs (verte) */
    t_tur hud_3hp;          /* barre 3 coeurs (jaune) */
    t_tur hud_2hp;          /* barre 2 coeurs (orange) */
    t_tur hud_1hp;          /* barre 1 coeur (rouge) */
    t_tur hud_lil_dega;     /* petit flash rouge */
    t_tur hud_mid_dega;     /* moyen flash rouge */
    t_tur hud_big_dega;     /* grand flash rouge */
    t_tur hud_game_over;    /* écran game over */
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
}   t_dda_vars;

typedef enum e_face { TEX_NORTH, TEX_SOUTH, TEX_EAST, TEX_WEST } t_face;

/* Alien AI states */
typedef enum e_alien_state { 
    ALIEN_PATROL,    /* patrouille normale */
    ALIEN_SEARCH,    /* recherche joueur perdu */
    ALIEN_CHASE,     /* poursuite directe */
    ALIEN_STALKING,  /* traque lente autour d'un mur (sent le joueur) */
    ALIEN_PREPARING  /* préparation avant bond (joueur à côté mais pas vu) */
} t_alien_state;

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
    double   attack_anim_timer; /* dur e9e restante de l'animation d'attaque */
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

/* HUD API */
int     load_hud_textures(t_cub *cub);
void    draw_health_bar(t_cub *cub);
void    draw_damage_flash(t_cub *cub);
void    draw_game_over(t_cub *cub);
void    cleanup_hud_textures(t_cub *cub);
void    hud_update_reload(t_cub *cub, double dt);
void    hud_update(t_cub *cub, double dt);


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
int	map_is_closed_and_single_spawn(char **map);
int	is_map_valid(t_cub *cub);

// src/parser/parser_path_checker.c
int is_path_valid(t_cub *cub);

// src/parser/parser_color_checker.c
void retrieve_colors(t_cub *cub);
int valid_colors(t_cub *cub);

// src/parser/parser_map.c
char	*copy_map(const char *current_line);
void	free_partial_map(char **map, int n);
void	free_full_map(char **map);
int	map_fail(t_cub *cub, const char *msg);
int	allocate_map_buffer(t_cub *cub, char ***map_buffer, int pad);
int	map_flood_fill(char **map_buffer, t_cub *cub);

// src/parser/wall_checker.c
int	wall_check(char **buffer_map);

// src/parser/parser_utils.c
int	ft_lastchars(const char *s, const char *suffix, size_t i);
const char *skip_spaces(const char *s);

// src/player_init/player_init.c
int	player_init(t_cub *cub);
int	player_allocate(t_cub *cub);
double	deg_to_radiant(double deg);
void find_player_plane(t_player *p);
void find_player_pos(t_cub *cub);
void find_player_dir(t_cub *cub);

// src/game/game_init.c
int	start_game(t_cub *cub);
int	on_destroy(t_cub *cub);

// src/game/game_cleanup.c
void cleanup_resources(t_cub *cub);
void cleanup_all(t_cub *cub);

// src/game/memory_utils.c
void *safe_malloc(size_t size);
void *safe_calloc(size_t count, size_t size);
void init_mlx_data(t_cub *cub);

// src/game/game_keys.c
int key_press(int keycode, t_cub *cub);
int key_drop(int keycode, t_cub *cub);
// Misc (unused / commented in original)
//int ft_check_player(); // check si le joueur est present dans la map a chaque mouvement
//void    ft_game_init(int argc, char argv, t_list cubinit);

//init
void init_values(t_cub *cub);
int window_init(t_cub *cub);
void    init_mlx(t_cub *cub);

int start_game(t_cub *cub);
void ray_rendu(t_cub *cub);
void  find_player_plane(t_player *p);
t_cord make_ray(int column, int width, t_cord dir, t_cord plane);

void    move_player(t_cub *cub);

/* player helpers */
bool    player_is_moving(t_player *p);

double perform_dda(t_cub *cub, double ray_dir_x, double ray_dir_y, int *side);

void    update_player_trig(t_cub *cub);
void draw_column(t_cub *cub, int column, double ray_x, double ray_y);

void    clear_image(t_cub *cub);
void    put_pixel(int x, int y, int color, t_cub *cub);

// Fonction de correction de distorsion (actuellement non utilisée)
// t_distortion_data apply_distortion_correction(t_cub *cub, int column, double ray_x, double ray_y, double raw_distance);

int init_textures(t_cub *cub);
t_tur *pick_face(t_cub *cub, int side, double rx, double ry);
int compute_tex_x(t_cub *cub, t_tur *tex, int side, double dist, double rx, double ry);

// Fonctions ciel et sol
void draw_skybox(t_cub *cub);
void draw_floor(t_cub *cub);
void render_frame(t_cub *cub);
t_cord make_ray(int column, int width, t_cord dir, t_cord plane);
void draw_minimap(t_cub *cub);


# endif 
