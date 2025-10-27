#include "../../includes/cub3d.h"

/*
** make_ray :
** ------------
** Construit la direction d’un rayon pour une colonne de l’écran.
**
** Idée :
**   - La direction du joueur = vecteur "nez" (dir).
**   - Le plane = vecteur "épaules", perpendiculaire à dir,
**     étiré selon le champ de vision (FOV).
**   - camera_x ∈ [-1, +1] représente la position horizontale
**     de la colonne dans l’écran : -1 = gauche, 0 = centre, +1 = droite.
**
** Formule :
**     ray = dir + plane * camera_x
**
** Schéma (vue de dessus) :
**
**                 plane(-1)           dir (0)          plane(+1)
**                     \                |                /
**                      \               |               /
**                       \              |              /
**                        \             |             /
**                         \            |            /
**                          \           |           /
**                           \          |          /
**                            \         |         /
**                             \        |        /
**                              \       |       /
**                               \      |      /
**                                \     |     /
**                                 \    |    /
**                                  \   |   /
**                                   \  |  /
**                                    \ | /
**                                     \|/
** ------------------------- Player (0,0) ------------------------
**
*/
static double normalize_column(int column, int width)
{
    // Transforme la colonne en une coordonnée camera_x ∈ [-1, +1]
    double resu;

    resu = ((column - ((width - 1) / 2.0)) / ((width - 1) / 2.0));
    return (resu);
}
    
static t_cord scale_plane(t_cord plane, double camera_x)
{
    // Multiplie le vecteur plane par camera_x
    t_cord scaled;

    scaled.x = plane.x * camera_x;
    scaled.y = plane.y * camera_x;
    return scaled;
}

static t_cord add_vectors(t_cord a, t_cord b)
{
    // Additionne deux vecteurs
    t_cord result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

t_cord make_ray(int column, int width, t_cord dir, t_cord plane)
{
    double camera_x;
    t_cord plane_scaled;
    t_cord ray;

    camera_x = normalize_column(column, width);
    plane_scaled = scale_plane(plane, camera_x);
    ray = add_vectors(dir, plane_scaled);
    return ray;
}

// Forward declaration for draw_column

void	ray_rendu(t_cub *cub)
{
	t_cord	dir;
	t_cord	plane;
	t_cord	ray;
	int		column;

	dir.x = cub->player->cos_angle;
	dir.y = cub->player->sin_angle;
	plane.x = cub->player->plane->x;
	plane.y = cub->player->plane->y;
	column = 0;
	while (column < WIDTH)
	{
		/* fabriquer le rayon pour cette colonne */
		ray = make_ray(column, WIDTH, dir, plane);
		/* dessiner la colonne */
		draw_column(cub, column, ray.x, ray.y);
		column++;
	}
}
