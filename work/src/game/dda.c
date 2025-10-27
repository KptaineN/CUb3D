
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

/*
1.Vérifie que cub et cub->map existent.
2.Si map_x ou map_y est négatif → mur.
3.Calcule la hauteur de la carte (compte les lignes).
4.Si map_y est en dehors de [0, hauteur-1] → mur.
5.Récupère la longueur de la ligne map[map_y]
6.Si map_x est en dehors de [0, row_len-1] → mur.
7.Récupère le caractère tile = map[map_y][map_x] et renvoie
true si tile == '1' ou tile == ' '.
*/
static bool map_is_wall(t_cub *cub, int32_t map_x, int32_t map_y, char *tile_out)
{
    char    **map;
    size_t  map_height;
    size_t  row_len;
    char    tile;

    if (tile_out)
        *tile_out = '1';
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
    if (tile_out)
        *tile_out = tile;
    return (tile == '1' || tile == ' ' || tile == 'P' || tile == 'D' || tile == '6');
}

/*
Convertir la position du joueur en coordonnées carte (unités « cases »)
et en déduire la cellule courante.
Si BLOCK = 64 px et que tu es à x = 208 px, 
alors pos_x = 208/64 = 3.25 
3.25 → dans la case 3 et à 0.25
*/
static void dda_init_position(t_cub *cub, t_dda_vars *dda)
{
    dda->pos_x = cub->player->position->x / (double)BLOCK;
    dda->pos_y = cub->player->position->y / (double)BLOCK;
    dda->map_x = (int32_t)dda->pos_x;
    dda->map_y = (int32_t)dda->pos_y;
}


/*
Calculer la distance paramétrique
pour franchir une case dans l’axe correspondant.
Si ray_dir == 0, renvoie un grand nombre (1e30)
pour éviter la division par zéro 
(un rayon parallèle à l’autre axe ne franchit
jamais de lignes dans cet axe).
Sinon delta = |1 / ray_dir|.
rayon paramétré par P(t) = pos + t * dir
il faut augmenter x (et ossi y) de 1 pour passer d’une ligne
→ donc augmenter t de 1/|dir_x|.
*/
static double dda_calc_delta(double ray_dir)
{
    double resu;

    if (ray_dir == 0.0)
        return 1e30;
    resu = fabs(1.0 / ray_dir);
    return(resu);
}

/*
initialise le rayon courant pour delta_x et delta_y
*/
static void dda_init_delta(double ray_dir_x, double ray_dir_y, t_dda_vars *dda)
{
    dda->delta_x = dda_calc_delta(ray_dir_x);
    dda->delta_y = dda_calc_delta(ray_dir_y);
}

/*
Calculer la distance paramétrique initiale (en t)
entre la position actuelle et la première ligne de grille
rencontrée dans la direction du rayon pour cet axe.

Si on va vers le 'négatif' (ray_dir < 0)
→ première ligne à gauche/haut est à pos - map.
Donc side = (pos - map) * delta.

Sinon on va vers le 'positif'
→ première ligne à droite/bas est à (map + 1 - pos).
Donc side = (map + 1 - pos) * delta.
*/
static double dda_side_dist(double pos, int32_t map, double delta, double ray_dir)
{
    double resu;

    if (ray_dir < 0.0)
    {
        resu = (pos - map) * delta;
        return resu;
    }
        
    else
    {
        resu = (map + 1.0 - pos) * delta;
        return(resu); 
    }
}

/*
Déterminer de combien on avance en case
à chaque saut DDA sur X et Y (step_x = ±1, step_y = ±1).
Calcule les premières distances side_x et side_y 
pour la ligne de grille à franchir de chaque axe.
step_x dit si, quand on franchit une ligne verticale
on entre dans la case à gauche (-1) ou à droite (+1).
side_x/side_y sont nos compteurs de distance
vers la prochaine frontière dans chaque axe.
*/
static void dda_init_side_step(double ray_dir_x, double ray_dir_y, t_dda_vars *dda)
{
    if (ray_dir_x < 0.0)
        dda->step_x = -1;
    else
        dda->step_x = 1;
    dda->side_x = dda_side_dist(dda->pos_x, dda->map_x, dda->delta_x, ray_dir_x);

    if (ray_dir_y < 0.0)
        dda->step_y = -1;
    else
        dda->step_y = 1;
    dda->side_y = dda_side_dist(dda->pos_y, dda->map_y, dda->delta_y, ray_dir_y);
}

/*
Sécurité anti-boucle infinie :
renvoie un upper bound du nombre de pas DDA possibles
ici hauteur * largeur_max.
Ne devrait jamais traverser plus de cases que le nombre 
total de cases de la carte si tout va bien.
*/
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
    return (height * max_width);
}

/*
Avance case par case le long du rayon,
En choisissant à chaque fois la prochaine frontière la plus proche
(x ou y). S’arrête quand tape un mur.
1. step_limit = map_step_limit(cub) → plafond de sécurité.
2. Boucle while (steps < step_limit) :
-Si side_x < side_y → on rencontrera d’abord une frontière verticale :
~ side_x += delta_x (on programme la prochaine verticale)
~ map_x += step_x (on entre dans la case voisine de ce côté)
~ *side = 0 (impact sur mur vertical)
-Sinon → frontière horizontale :
~ side_y += delta_y
~ map_y += step_y
~ *side = 1 (impact sur mur horizontal)
-Si map_is_wall(cub, map_x, map_y) → touché → return true.
3. Si on dépasse le plafond → false (rien touché / carte mal formée).
Comme 2 chronos (side_x, side_y) qui tournent. Celui qui “sonne”
d’abord indique si on traverse une ligne verticale ou horizontale.
*/
static bool dda_loop(t_cub *cub, t_dda_vars *dda, int *side, char *hit_tile)
{
    int steps;
    size_t  step_limit;
    
    steps = 0;

    step_limit = map_step_limit(cub);
    if (step_limit == 0)
        return false;
    steps = 0;
    while (steps < (int)step_limit)
    {
        char cell;
        if (dda->side_x < dda->side_y)
        {
            dda->side_x += dda->delta_x;
            dda->map_x  += dda->step_x;
            *side = 0;
        }
        else
        {
            dda->side_y += dda->delta_y;
            dda->map_y  += dda->step_y;
            *side = 1;
        }
        steps++;
        cell = '1';
        if (map_is_wall(cub, dda->map_x, dda->map_y, &cell))
        {
            if (hit_tile)
                *hit_tile = cell;
            return (true);
        }
    }
    return false;
}

/*
Lance l'algo DDA, et renvoie la distance perpendiculaire au premier mur touché
(en unités ‘t’, i.e. en cases)
1.Initialise pos, map, delta, side_x/side_y/step.
2.Lance dda_loop. S’il ne trouve rien → 0.0.
3.Si on a frappé un mur vertical (*side == 0) :
- La dernière mise à jour a dépassé la frontière,
- donc la bonne distance est side_x - delta_x.
4.Pareil pour horizontal : side_y - delta_y.
*/
double perform_dda(t_cub *cub, double ray_dir_x, double ray_dir_y, int *side, char *hit_tile)
{
    t_dda_vars dda;
    dda_init_position(cub, &dda);
    dda_init_delta(ray_dir_x, ray_dir_y, &dda);
    dda_init_side_step(ray_dir_x, ray_dir_y, &dda);
    *side = 0;
    if (!dda_loop(cub, &dda, side, hit_tile))
        return 0.0;
    if (*side == 0)
        return (dda.side_x - dda.delta_x);
    else
        return (dda.side_y - dda.delta_y);
}
