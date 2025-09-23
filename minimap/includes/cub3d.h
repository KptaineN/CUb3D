/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:46:22 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/23 18:26:34 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CUB3D_H
# define CUB3D_H

// Includes

// # include "../minilibx-linux/mlx.h"
# include "LIBFT/libft.h"
# include "mlx/mlx.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <stdbool.h>
# include <math.h>
# include <stdint.h>

// Defines
# define BLOCK 40
#define WIDTH 800
#define HEIGHT 600

# define LEFT 65361
# define RIGHT 65363

# define PI 3.14159265353
// Définitions des touches
#define W 119
#define A 97
#define S 115
#define D 100
#define ESC_KEY 65307

typedef struct s_player t_player;
typedef struct s_color  t_color;
typedef struct s_cord   t_cord;

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
    int window_width; // default value : 800
    int window_lenght; // default value : 600
    // Champs MLX intégrés (anciennement dans t_game)
    void    *mlx;
    void    *window;
    void    *image;
    char    *data;
    int     bpp;
    int     size_line;
    int     endian;
    t_player    *player;
}   t_cub;

// Structure qui contient toutes les informations du joueur 
typedef struct s_player
{
    t_cord  *position;
    t_cord  *direction;
    t_cord  *plane; // chercher ce que c'est concrétement
    float angle;
    double  fov;
    double  move_frame;
    double  rotation_frame;
    double  radius; // chercher ce que c'est concrétement
    bool    key_up;
    bool    key_down;
    bool    key_left;
    bool    key_right;
    bool    left_rotate;
    bool    right_rotate;
    
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

// Prototypes

// parser.c
int ft_cub_parser(int argc, char **argv, t_cub *cub);
// parser_check_elements.c
int	check_elements(t_cub *cub);
int	check_map(t_cub *cub);
int	line_is_empty(char *line);
int	count_map_lines(char **start);
int	check_spawn(char **padded);

int	map_is_closed_and_single_spawn(char **map);
int	is_map_valid(t_cub *cub);

/* exported helpers from parser_map.c */
char	*copy_map(const char *current_line);
void	free_partial_map(char **map, int n);
void	free_full_map(char **map);
int	map_fail(t_cub *cub, const char *msg);

/* allocate a map buffer with padding space; returns 0 on success, 1 on error */
int	allocate_map_buffer(t_cub *cub, char ***map_buffer, int pad);
int map_flood_fill(char **map_buffer, t_cub *cub);
int wall_check(char **buffer_map);
int ft_lastchars(const char *s, const char *suffix, size_t i);

// Fonctions de jeu
int16_t start_game(t_cub *cubing);
int16_t initialisation(t_cub *cubing);
void init_mlx(t_cub *cub);
int draw_loop(t_cub *cubing);

// Fonctions de dessin
void put_pixel(int x, int y, int color, t_cub *cub);
void draw_y_triangle(int x, int y, int size, int color, t_cub *cub);
void close_image(t_cub *cub);
void draw_square(int x, int y, int size, int color, t_cub *cub);
void draw_map(t_cub *cub);

// Fonctions du joueur
void init_player(t_player *player);
void move_player(t_player *player);
int key_press(int keycode, t_cub *cub);
int key_drop(int keycode, t_cub *cub);
void draw_map(t_cub *cub);
int player_init(t_cub *cub);
/* player_init utilities */
double deg_to_radiant(double deg);
int player_allocate(t_cub *cub);
void player_cleanup(t_cub *cub);

// Fonctions de nettoyage
void cleanup_resources(t_cub *cub);

//int ft_check_player(); // check si le joueur est present dans la map a chaque mouvement
//void    ft_game_init(int argc, char argv, t_list cubinit);


# endif 
