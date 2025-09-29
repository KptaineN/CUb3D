
/*
** perform_dda :
** -------------
** BUT : Avancer le long d’un rayon dans une MAP en grille, en sautant
**       de frontière de CASE en frontière de CASE (DDA), jusqu’à toucher
**       une case mur. Retourne la distance **perpendiculaire** en pixels,
**       et indique le type de frontière touchée (verticale/horizontale) via *side.
**
** Idée Feynman :
**   - Le monde est une grille de cases [x,y].
**   - Depuis la position (en **cases**) du joueur, on lance un rayon (dir_x, dir_y).
**   - On calcule la distance jusqu’à la prochaine frontière **verticale** (|) : side_x,
**     et jusqu’à la prochaine **horizontale** (—) : side_y.
**   - À chaque pas : on franchit la plus proche (min(side_x, side_y)), on entre
**     dans la case voisine, on ré-accumule la distance du même type (+= delta_*),
**     puis on teste si la case est un mur.
**
** Schéma (vue de dessus) :
**
**   y ↑
**     |        |        |        |        |
**     |--------+--------+--------+--------+--> x
**     |        |        |        |        |
**     |        |   █    |        |        |
**     |        |  (pos) |        |        |
**     |        |        |        |        |
**     |--------+--------+--------+--------+
**
**   - (pos_x,pos_y) en unités **cases** (pas pixels).
**   - (map_x,map_y) = floor(pos).
**   - Prochaine frontière verticale (|) à gauche/droite -> side_x
**     Prochaine frontière horizontale (—) en haut/bas -> side_y
**   - Compare side_x vs side_y :
**       * si side_x < side_y -> on franchit une verticale -> avance en X (map_x += step_x), side=0
**       * sinon              -> on franchit une horizontale -> avance en Y (map_y += step_y), side=1
**
** Pourquoi c’est anti fish-eye ?
**   delta_x = |1/ray_dir_x|, delta_y = |1/ray_dir_y|
**   En accumulant side_x/side_y avec ces deltas, on mesure une **distance perpendiculaire**
**   au plan caméra (pas la longueur du rayon). Donc la projection HEIGHT/(dist/BLOCK)
**   est correcte **sans correction cos(angle)**.
*/
#include "../../includes/cub3d.h"


typedef struct s_dda_vars {
    double pos_x, pos_y;
    int32_t map_x, map_y;
    double delta_x, delta_y;
    double side_x, side_y;
    int32_t step_x, step_y;
} t_dda_vars;

static bool map_is_wall(t_cub *cub, int32_t map_x, int32_t map_y)
{
    char    **map;
    size_t  map_height;
    size_t  row_len;
    char    tile;

    if (!cub || !cub->map)
        return true;
    map = cub->map;
    if (map_y < 0 || map_x < 0)
        return true;
    map_height = 0;
    while (map[map_height])
        map_height++;
    if ((size_t)map_y >= map_height)
        return true;
    row_len = ft_strlen(map[map_y]);
    if ((size_t)map_x >= row_len)
        return true;
    tile = map[map_y][map_x];
    return (tile == '1' || tile == ' ');
}

static void dda_init_position(t_cub *cub, t_dda_vars *v)
{
    v->pos_x = cub->player->position->x / (double)BLOCK;
    v->pos_y = cub->player->position->y / (double)BLOCK;
    v->map_x = (int32_t)v->pos_x;
    v->map_y = (int32_t)v->pos_y;
}

static double dda_calc_delta(double ray_dir)
{
    double resu;

    if (ray_dir == 0.0)
        return 1e30;
    resu = fabs(1.0 / ray_dir);
    return(resu);
}

static void dda_init_delta(double ray_dir_x, double ray_dir_y, t_dda_vars *v)
{
    v->delta_x = dda_calc_delta(ray_dir_x);
    v->delta_y = dda_calc_delta(ray_dir_y);
}

static double dda_side_dist(double pos, int32_t map, double delta, double ray_dir)
{
    if (ray_dir < 0.0)
        return (pos - map) * delta;
    else
        return (map + 1.0 - pos) * delta;
}

static void dda_init_side_step(double ray_dir_x, double ray_dir_y, t_dda_vars *v)
{
    if (ray_dir_x < 0.0)
        v->step_x = -1;
    else
        v->step_x = 1;
    v->side_x = dda_side_dist(v->pos_x, v->map_x, v->delta_x, ray_dir_x);

    if (ray_dir_y < 0.0)
        v->step_y = -1;
    else
        v->step_y = 1;
    v->side_y = dda_side_dist(v->pos_y, v->map_y, v->delta_y, ray_dir_y);
}

static size_t map_step_limit(t_cub *cub)
{
    size_t  height;
    size_t  max_width;
    size_t  len;

    if (!cub || !cub->map)
        return 0;
    height = 0;
    max_width = 0;
    while (cub->map[height])
    {
        len = ft_strlen(cub->map[height]);
        if (len > max_width)
            max_width = len;
        height++;
    }
    if (height == 0 || max_width == 0)
        return 0;
    return height * max_width;
}


static bool dda_loop(t_cub *cub, t_dda_vars *v, int *side)
{
    int steps;
    size_t  step_limit;
    
    steps = 0;

    step_limit = map_step_limit(cub);
    if (step_limit == 0)
        return false;
    steps = 0;
    while (true)
    {
        if (v->side_x < v->side_y)
        {
            v->side_x += v->delta_x;
            v->map_x  += v->step_x;
            *side = 0;
        }
        else
        {
            v->side_y += v->delta_y;
            v->map_y  += v->step_y;
            *side = 1;
        }
        steps++;
        if (map_is_wall(cub, v->map_x, v->map_y))
            return true;
    }
}

double perform_dda(t_cub *cub, double ray_dir_x, double ray_dir_y, int *side)
{
    t_dda_vars dda;
    dda_init_position(cub, &dda);
    dda_init_delta(ray_dir_x, ray_dir_y, &dda);
    dda_init_side_step(ray_dir_x, ray_dir_y, &dda);
    *side = 0;
    if (!dda_loop(cub, &dda, side))
        return 0.0;
    if (*side == 0)
        return (dda.side_x - dda.delta_x) * (double)BLOCK;
    else
        return (dda.side_y - dda.delta_y) * (double)BLOCK;
}
