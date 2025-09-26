#include "../includes/cub3d.h"

bool is_wall(t_cub *cub, int32_t x, int32_t y)
{
    if (x < 0 || y < 0 || x >= cub->map_width || y >= cub->map_height)
        return (true);
    if ((int32_t)ft_strlen(cub->map[y]) <= x)
        return (true);
    return (cub->map[y][x] == '1');
}

double  perform_dda(t_cub *cub, double ray_dir_x, double ray_dir_y,
        int *side)
{
    double  pos_x;
    double  pos_y;
    int32_t map_x;
    int32_t map_y;
    double  delta_x;
    double  delta_y;
     // side_x / side_y :
    // Distance perpendiculaire depuis la position jusqu’à la première
    // frontière VERTICALE/HORIZONTALE à atteindre.
    double  side_x;
    double  side_y;
        // step_x / step_y : dans quel sens on se déplace d’une case à l’autre
    int32_t step_x;
    int32_t step_y;

    // Position du joueur en cases (pas en pixels) : facilite le DDA
    pos_x = cub->player->position->x / BLOCK;
    pos_y = cub->player->position->y / BLOCK;
    // Case actuelle
    map_x = (int32_t)pos_x;
    map_y = (int32_t)pos_y;
    // delta_x / delta_y :
    // Combien il faut avancer le long du rayon (en “distance perpendiculaire”)
    // pour franchir une frontière VERTICALE (delta_x) ou HORIZONTALE (delta_y) suivante.
    delta_x = (ray_dir_x == 0) ? 1e30 : fabs(1.0 / ray_dir_x);
    delta_y = (ray_dir_y == 0) ? 1e30 : fabs(1.0 / ray_dir_y);
    // Initialisation côté X
    if (ray_dir_x < 0)
    {
        step_x = -1;
        side_x = (pos_x - map_x) * delta_x;
        // distance jusqu’à la frontière gauche
    }
    else
    {
        step_x = 1;
        side_x = (map_x + 1.0 - pos_x) * delta_x;
        // distance jusqu’à la frontière droite
    }
    if (ray_dir_y < 0)
    {
        step_y = -1;
        side_y = (pos_y - map_y) * delta_y;
        // distance jusqu’à la frontière haute
    }
    else
    {
        step_y = 1;
        side_y = (map_y + 1.0 - pos_y) * delta_y;
        // distance jusqu’à la frontière en bas
    }
        // Avance case par case : on “saute” toujours vers la frontière la plus proche
    while (1)
    {
        if (side_x < side_y)
        {
            side_x += delta_x;     // on franchit une frontière VERTICALE
            map_x  += step_x;       // on entre dans la case voisine en X
            *side   = 0;            // 0 => mur de type Est/Ouest (frontière verticale franchie)
        }
        else
        {
            side_y += delta_y;      // on franchit une frontière HORIZONTALE
            map_y  += step_y;       // on entre dans la case voisine en Y
            *side   = 1;            // 1 => mur de type Nord/Sud (frontière horizontale franchie)
        }
        if (is_wall(cub, map_x, map_y))
            break ;// on s’arrête dès qu’on entre dans une case mur
    }
      // ————————————————————————————————————————————————————————————
    // ⚠ Ici on récupère la DISTANCE PERPENDICULAIRE au mur (clé anti fish-eye)
    //    Pourquoi ? Parce que side_x/side_y ont été accumulés en “perp distance”
    //    (c’est la magie du deltaDist = |1/rayDir|).
    //    Si on renvoyait la distance euclidienne (sqrt(dx*dx+dy*dy)), on aurait du
    //    fish-eye. Ici, pas besoin : on renvoie la bonne distance pour la projection.
    // ————————————————————————————————————————————————————————————
    if (*side == 0)
        return ((side_x - delta_x) * BLOCK);  // on retire le dernier incrément
    return ((side_y - delta_y) * BLOCK);  // et on remet en pixels monde
}