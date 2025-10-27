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

/*
“Convertit l’indice de colonne écran en coordonnée caméra camera_x dans [-1, +1].”
camera_x = (col - (width - 1)/2) / ((width - 1)/2)
→ ~ -1 à gauche, 0 au centre, +1 à droite.
Ça place chaque colonne sur la droite de projection (plan image)
*/
static double normalize_column(int column, int width)
{
    // Transforme la colonne en une coordonnée camera_x ∈ [-1, +1]
    // Projection linéaire normale sans distorsion
    double camera_x = 2.0 * column / (double)width - 1.0;
    
    return camera_x;
}
    
/*
Le plan (plane) est perpendiculaire à la direction du joueur et sa norme est ≈ tan(FOV/2).
Donc plane * camera_x te “décale” latéralement sur le cône de vision.
*/
static t_cord scale_plane(t_cord plane, double camera_x)
{
    // Multiplie le vecteur plane par camera_x
    t_cord scaled;

    scaled.x = plane.x * camera_x;
    scaled.y = plane.y * camera_x;
    return scaled;
}

/*
ray = dir + plane * camera_x. 
Additionne deux vecteurs
*/
static t_cord add_vectors(t_cord a, t_cord b)
{
    t_cord result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

/*
Construis la direction du rayon pour la colonne donnée.”
Détails / Maths
1. camera_x = normalize_column(column, width)
2. plane_scaled = plane * camera_x
3. ray = dir + plane_scaled
- dir est le centre du champ de vision.
- Le plan est perpendiculaire à dir.
- En prenant dir + plane * x avec x ∈ [-1,1],
balayes linéairement tout le FOV.
- Pas besoin de normaliser ray pour DDA,
travaille en paramètre t (delta = 1/|ray_dir|) qui gère l’échelle.
*/
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
/*
"Pour chaque colonne de l’écran, je fabrique le rayon correspondant, 
j’exécute la DDA (dans draw_column chez toi), et dessine la colonne.”
1. Récupère la direction du joueur (cos_angle, sin_angle) 
- le plan est déjà pré-calculé (perp. à dir, norme ≈ tan(FOV/2)).
2.Pour column de 0 à WIDTH-1 :
- ray = make_ray(column, WIDTH, dir, plane)
- draw_column(cub, column, ray.x, ray.y)
- draw_column est censée :
appeler perform_dda(cub, ray.x, ray.y, &side)
convertir la distance en hauteur de mur
choisir la texture/couleur (souvent on assombrit si side == 1 pour un effet 3D)
dessiner la bande verticale.
*/
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
