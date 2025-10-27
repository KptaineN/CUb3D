#include "../../includes/cub3d.h"


void    put_pixel(int x, int y, int color, t_cub *cub)
{
    int32_t i; //index du premier octet du pixel (x,y)

    if (!cub || !cub->mlx_data)
        return ;
    if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return ;
    i = (int32_t)y * cub->mlx_line_size + (int32_t)x * (cub->mlx_bpp / 8);
    cub->mlx_data[i] = color & 0xFF;                    //BLEU
    cub->mlx_data[i + 1] = (color >> 8) & 0xFF;         //VERT
    cub->mlx_data[i + 2] = (color >> 16) & 0xFF;        //ROUGE
}

void draw_column(t_cub *cub, int column, double ray_dir_x, double ray_dir_y)
{
    double  distance;
    int     line_height;
    int     draw_start;
    int     draw_end;
    int     y;
    int     color;
    int     side;

    distance = perform_dda(cub, ray_dir_x, ray_dir_y, &side);
    if (distance < 1e-6)
        distance = 1e-6;

    line_height = (int)((double)HEIGHT / (distance / BLOCK));
    draw_start = -line_height / 2 + HEIGHT / 2;
    if (draw_start < 0)
        draw_start = 0;
    draw_end = line_height / 2 + HEIGHT / 2;
    if (draw_end >= HEIGHT)
        draw_end = HEIGHT - 1;

    color = (side == 1) ? 0x777777 : 0xCCCCCC;

    y = draw_start;
    while (y <= draw_end)
        put_pixel(column, y++, color, cub);
}

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
t_cord make_ray(int column, int width, t_cord dir, t_cord plane)
{
    t_cord ray;
    double camera_x;

    // Normalisation colonne → [-1, +1]
    camera_x = 2.0 * column / (double)width - 1.0;

    // Rayon = direction centrale + (plane * camera_x)
    ray.x = dir.x + plane.x * camera_x;
    ray.y = dir.y + plane.y * camera_x;

    return ray;
}

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
 