/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:46:22 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/29 12:47:38 by nkief            ###   ########.fr       */
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
    void    *mlx;
    void    *mlx_window;
    void    *mlx_image;
    char    *mlx_data;
    int mlx_bpp;
    int mlx_line_size;
    int mlx_endian;
    t_player    *player;
}   t_cub;

// Structure qui contient toutes les informations du joueur 
typedef struct s_player
{
    t_cord  *position;
    t_cord  *direction;
    t_cord  *plane; // chercher ce que c'est concrétement
    double  fov;
    double  move_frame;
    double  rotation_frame;
    double  sin_angle;
    double  cos_angle;
    double  angle;
    bool    key_up;
    bool    key_down;
    bool    key_left;
    bool    key_right;
    bool    left_rotate;
    bool    right_rotate;
    // double  radius; // chercher ce que c'est concrétement
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

// src/parser/parser.c
int ft_cub_parser(int argc, char **argv, t_cub *cub);

// src/parser/parser_check_elements.c
int	check_elements(t_cub *cub);
int	check_map(t_cub *cub);
int	line_is_empty(char *line);
int	count_map_lines(char **start);
int	check_spawn(char **padded);
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

// src/game/game_init.c
int	start_game(t_cub *cub);
int	on_destroy(t_cub *cub);

// src/game/game_cleanup.c
int	cleanup_resources(t_cub *cub);

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
void  find_player_plane(t_cub *cub);

void    move_player(t_cub *cub);
double perform_dda(t_cub *cub, double ray_dir_x, double ray_dir_y, int *side);


# endif 