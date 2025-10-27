#include "../../includes/cub3d.h"

/* helper : échantillonnage bilinéaire sur texture (coord en [0..w), [0..h)) */
static int sample_tex_bilinear(t_tur *t, double xf, double yf)
{
    int w = t->w;
    int h = t->h;
    if (w <= 0 || h <= 0) return 0;
    /* wrap coordinates */
    while (xf < 0) xf += w;
    while (yf < 0) yf += h;
    xf = fmod(xf, (double)w);
    yf = fmod(yf, (double)h);

    int x0 = (int)floor(xf);
    int y0i = (int)floor(yf);
    int x1 = (x0 + 1) % w;
    int y1 = (y0i + 1) % h;
    double sx = xf - x0;
    double sy = yf - y0i;

    int bytes = t->bpp / 8;
    int o00 = y0i * t->sl + x0 * bytes;
    int o10 = y0i * t->sl + x1 * bytes;
    int o01 = y1 * t->sl + x0 * bytes;
    int o11 = y1 * t->sl + x1 * bytes;
    int c00 = *(int *)(t->data + o00);
    int c10 = *(int *)(t->data + o10);
    int c01 = *(int *)(t->data + o01);
    int c11 = *(int *)(t->data + o11);

    unsigned char *p00 = (unsigned char *)&c00;
    unsigned char *p10 = (unsigned char *)&c10;
    unsigned char *p01 = (unsigned char *)&c01;
    unsigned char *p11 = (unsigned char *)&c11;

    unsigned int r = (unsigned int)((1 - sx) * (1 - sy) * p00[2] + sx * (1 - sy) * p10[2] + (1 - sx) * sy * p01[2] + sx * sy * p11[2]);
    unsigned int g = (unsigned int)((1 - sx) * (1 - sy) * p00[1] + sx * (1 - sy) * p10[1] + (1 - sx) * sy * p01[1] + sx * sy * p11[1]);
    unsigned int b = (unsigned int)((1 - sx) * (1 - sy) * p00[0] + sx * (1 - sy) * p10[0] + (1 - sx) * sy * p01[0] + sx * sy * p11[0]);

    int out = (int)((r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF));
    return out;
}

/*
 * put_pixel : wrapper qui écrit un pixel dans l'image MLX associée au cub
 * Signature dans header : void put_pixel(int x, int y, int color, t_cub *cub);
 */
void put_pixel(int x, int y, int color, t_cub *cub)
{
    if (!cub || !cub->mlx_image || !cub->mlx_data)
        return;
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;
    int bytes = cub->mlx_bpp / 8;
    int offset = y * cub->mlx_line_size + x * bytes;
    *(int *)(cub->mlx_data + offset) = color;
}

/* clear_image : met à zéro la surface image avant de redessiner */
void clear_image(t_cub *cub)
{
    if (!cub || !cub->mlx_image || !cub->mlx_data)
        return;
    size_t total = HEIGHT * (size_t)cub->mlx_line_size;
    memset(cub->mlx_data, 0, total);
}

/*
 * draw_column : prototype dans header :
 *   void draw_column(t_cub *cub, int column, double ray_x, double ray_y);
 * Ici on calcule la distance avec perform_dda(cub, ray_x, ray_y, &side)
 * puis on affiche la colonne. Le mapping vertical est fait en espace-monde
 * : 1 unité world vertical -> tex->h texels, ce qui préserve l'échelle
 * de la texture quand on s'approche du mur.
 */

void draw_column(t_cub *cub, int column, double ray_x, double ray_y)
{
    int side = 0;
    char hit_tile = '1';
    double perp_dist = perform_dda(cub, ray_x, ray_y, &side, &hit_tile);
    if (perp_dist <= 0.0)
        perp_dist = 0.0001;

    /* store perpendicular distance for this column so sprites can be occluded */
    if (cub && column >= 0 && column < WIDTH)
        cub->zbuffer[column] = perp_dist;

    int line_h = (int)(HEIGHT / perp_dist);
    if (line_h <= 0) line_h = 1;
    int y0 = (HEIGHT - line_h) / 2;
    int y1 = (HEIGHT + line_h) / 2;

    t_tur *tex = pick_face(cub, side, ray_x, ray_y, hit_tile);
    if (!tex || !tex->img || !tex->data)
    {
        if (side == 0)
            tex = (ray_x < 0.0) ? &cub->textur[TEX_EAST] : &cub->textur[TEX_WEST];
        else
            tex = (ray_y < 0.0) ? &cub->textur[TEX_NORTH] : &cub->textur[TEX_SOUTH];
        if (!tex || !tex->img || !tex->data)
            return;
    }
    /* compute hit point in world-space to obtain stable texture X (fixed when rotating)
     * use perp_dist (returned by perform_dda) and ray_x/ray_y to compute hit coordinates
     */
    double pos_x = cub->player->position->x / (double)BLOCK;
    double pos_y = cub->player->position->y / (double)BLOCK;
    double hit_x = pos_x + perp_dist * ray_x;
    double hit_y = pos_y + perp_dist * ray_y;
    double tex_coord = (side == 0) ? hit_y - floor(hit_y) : hit_x - floor(hit_x);
    if (tex_coord < 0) tex_coord += 1.0;
    double xf = tex_coord * (double)tex->w; /* fractional x coord in texture space (world-stable) */
    /* Parameters: how many screen pixels per one texture tile vertically.
     * Ajuste cette valeur si tu veux que les motifs apparaissent plus grands/plus petits.
     */
    const double TILE_SCREEN_PIXELS = 64.0;
    /* horizontal texture coordinate in screen-space so the pattern doesn't slide with world */
    double txf_base = fmod((double)column / TILE_SCREEN_PIXELS * (double)tex->w, (double)tex->w);
    if (txf_base < 0) txf_base += tex->w;

    

    /* Only draw wall slice here. Sky and floor are rendered by their own functions. */
    for (int y = y0; y <= y1; ++y)
    {
        int rel = y - y0; /* pixel index inside the wall slice */
        int wall_height = y1 - y0 + 1;
        /* proportional mapping in world-space: top->1.0, bottom->0.0 */
        double world_v = 1.0 - ((double)rel / (double)wall_height);
        if (world_v < 0.0) world_v = 0.0;
        if (world_v > 1.0) world_v = 1.0;
        double tyf = world_v * (double)tex->h;
        if (tyf < 0) tyf = 0;
        if (tyf >= tex->h) tyf = tex->h - 1;
        int color = sample_tex_bilinear(tex, xf, tyf);
        if (side == 1)
        {
            unsigned char *c = (unsigned char *)&color;
            c[0] = (unsigned char)(c[0] * 0.75);
            c[1] = (unsigned char)(c[1] * 0.75);
            c[2] = (unsigned char)(c[2] * 0.75);
        }
        put_pixel(column, y, color, cub);
    }
}
