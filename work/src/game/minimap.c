#include "../../includes/cub3d.h"

#define MINIMAP_DIAMETER 200
#define MINIMAP_MARGIN 18
#define MINIMAP_ALPHA 0.78
#define MINIMAP_SPAN 9.0

#define MINIMAP_COLOR_BG 0x111217
#define MINIMAP_COLOR_FLOOR 0x1F2329
#define MINIMAP_COLOR_WALL 0xC3C5C7
#define MINIMAP_COLOR_BORDER 0x4A4E55
#define MINIMAP_COLOR_PLAYER 0x7CFF6A
#define MINIMAP_COLOR_ENEMY 0xFF6666
#define MINIMAP_COLOR_TEXT 0xF2F5FA

#define MINIMAP_FLOOR_TILING 2.0
#define MINIMAP_WALL_TILING 1.4

#define LETTER_WIDTH 5
#define LETTER_HEIGHT 5

static void blend_pixel(t_cub *cub, int x, int y, int color, double alpha)
{
    int             bytes;
    long            offset;
    int             dst;
    unsigned char   dr;
    unsigned char   dg;
    unsigned char   db;
    unsigned char   sr;
    unsigned char   sg;
    unsigned char   sb;
    unsigned char   rr;
    unsigned char   rg;
    unsigned char   rb;

    if (!cub || !cub->mlx_data || alpha <= 0.0)
        return ;
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return ;
    bytes = cub->mlx_bpp / 8;
    if (bytes <= 0 || cub->mlx_line_size <= 0)
        return ;
    offset = (long)y * (long)cub->mlx_line_size + (long)x * (long)bytes;
    if (offset < 0)
        return ;
    sr = (color >> 16) & 0xFF;
    sg = (color >> 8) & 0xFF;
    sb = color & 0xFF;
    if (alpha >= 1.0)
    {
        *(int *)(cub->mlx_data + offset) = (sr << 16) | (sg << 8) | sb;
        return ;
    }
    dst = *(int *)(cub->mlx_data + offset);
    dr = (dst >> 16) & 0xFF;
    dg = (dst >> 8) & 0xFF;
    db = dst & 0xFF;
    rr = (unsigned char)(alpha * sr + (1.0 - alpha) * dr);
    rg = (unsigned char)(alpha * sg + (1.0 - alpha) * dg);
    rb = (unsigned char)(alpha * sb + (1.0 - alpha) * db);
    *(int *)(cub->mlx_data + offset) = (rr << 16) | (rg << 8) | rb;
}

static void draw_line(t_cub *cub, int x0, int y0, int x1, int y1, int color,
        double alpha, int clip_cx, int clip_cy, int clip_radius)
{
    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    int e2;
    int ddx;
    int ddy;

    dx = abs(x1 - x0);
    sx = (x0 < x1) ? 1 : -1;
    dy = -abs(y1 - y0);
    sy = (y0 < y1) ? 1 : -1;
    err = dx + dy;
    while (1)
    {
        ddx = x0 - clip_cx;
        ddy = y0 - clip_cy;
        if (ddx * ddx + ddy * ddy <= clip_radius * clip_radius)
            blend_pixel(cub, x0, y0, color, alpha);
        if (x0 == x1 && y0 == y1)
            break ;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

static bool has_texture(const t_tur *tex)
{
    if (!tex)
        return (false);
    return (tex->data && tex->w > 0 && tex->h > 0 && tex->bpp >= 24 && tex->sl > 0);
}

static int sample_texture(const t_tur *tex, double u, double v, int fallback)
{
    int bytes;
    int tx;
    int ty;
    long offset;
    long max_bytes;
    int color;

    if (!has_texture(tex))
        return (fallback & 0x00FFFFFF);
    bytes = tex->bpp / 8;
    if (bytes <= 0)
        return (fallback & 0x00FFFFFF);
    u -= floor(u);
    v -= floor(v);
    if (u < 0.0)
        u = 0.0;
    if (v < 0.0)
        v = 0.0;
    tx = (int)(u * (double)(tex->w - 1));
    ty = (int)(v * (double)(tex->h - 1));
    if (tx < 0)
        tx = 0;
    if (ty < 0)
        ty = 0;
    offset = (long)ty * (long)tex->sl + (long)tx * (long)bytes;
    max_bytes = (long)tex->sl * (long)tex->h;
    if (offset < 0 || offset + bytes > max_bytes)
        return (fallback & 0x00FFFFFF);
    color = *(int *)(tex->data + offset);
    return (color & 0x00FFFFFF);
}

static int draw_player_icon(t_cub *cub, int cx, int cy, int clip_radius)
{
    t_tur       *tex;
    t_player    *player;
    double      dir_x;
    double      dir_y;
    double      angle;
    double      cos_a;
    double      sin_a;
    int         size;
    int         half;
    int         bytes;

    if (!cub)
        return (0);
    tex = &cub->minimap_player_tex;
    player = cub->player;
    if (!has_texture(tex) || !player)
        return (0);
    if (!player->direction)
        return (0);
    dir_x = player->direction->x;
    dir_y = player->direction->y;
    if (fabs(dir_x) < 1e-6 && fabs(dir_y) < 1e-6)
        return (0);
    angle = atan2(dir_y, dir_x);
    cos_a = cos(angle);
    sin_a = sin(angle);
    size = clip_radius / 3;
    if (size < 18)
        size = 18;
    if (size > clip_radius)
        size = clip_radius;
    if (size % 2 != 0)
        size++;
    half = size / 2;
    bytes = tex->bpp / 8;
    if (bytes <= 0)
        return (0);
    double inv_size = 1.0 / (double)size;
    int clip_sq = clip_radius * clip_radius;
    for (int y = 0; y < size; ++y)
    {
        double v_norm;
        int py;

        py = cy + (y - half);
        if (py < 0 || py >= HEIGHT)
            continue;
        v_norm = ((double)y + 0.5) * inv_size - 0.5;
        for (int x = 0; x < size; ++x)
        {
            double u_norm;
            double rot_u;
            double rot_v;
            double tex_u;
            double tex_v;
            int tx;
            int ty;
            int px;
            int offset;
            unsigned char *src;
            unsigned char b;
            unsigned char g;
            unsigned char r;

            px = cx + (x - half);
            if (px < 0 || px >= WIDTH)
                continue;
            if ((px - cx) * (px - cx) + (py - cy) * (py - cy) > clip_sq)
                continue;
            u_norm = ((double)x + 0.5) * inv_size - 0.5;
            rot_u = cos_a * u_norm + sin_a * v_norm;
            rot_v = -sin_a * u_norm + cos_a * v_norm;
            tex_u = rot_u + 0.5;
            tex_v = rot_v + 0.5;
            if (tex_u < 0.0 || tex_u > 1.0 || tex_v < 0.0 || tex_v > 1.0)
                continue;
            tx = (int)(tex_u * (tex->w - 1));
            ty = (int)(tex_v * (tex->h - 1));
            if (tx < 0 || ty < 0 || tx >= tex->w || ty >= tex->h)
                continue;
            offset = ty * tex->sl + tx * bytes;
            src = (unsigned char*)tex->data + offset;
            b = src[0];
            g = (bytes > 1) ? src[1] : 0;
            r = (bytes > 2) ? src[2] : 0;
            if (r > 250 && g < 5 && b > 250)
                continue;
            blend_pixel(cub, px, py, (r << 16) | (g << 8) | b, 1.0);
        }
    }
    return (1);
}

static void draw_marker(t_cub *cub, int cx, int cy, int radius, int clip_cx,
        int clip_cy, int clip_radius, int color)
{
    int dx;
    int dy;
    int px;
    int py;
    int ddx;
    int ddy;

    for (dy = -radius; dy <= radius; ++dy)
    {
        for (dx = -radius; dx <= radius; ++dx)
        {
            if (dx * dx + dy * dy > radius * radius)
                continue ;
            px = cx + dx;
            py = cy + dy;
            ddx = px - clip_cx;
            ddy = py - clip_cy;
            if (ddx * ddx + ddy * ddy > clip_radius * clip_radius)
                continue ;
            blend_pixel(cub, px, py, color, 1.0);
        }
    }
}

static void draw_direction_arrow(t_cub *cub, int cx, int cy, double dir_x,
        double dir_y, int clip_cx, int clip_cy, int clip_radius, int color)
{
    int     tip_x;
    int     tip_y;
    int     base_x;
    int     base_y;
    int     left_x;
    int     left_y;
    int     right_x;
    int     right_y;
    double  perp_x;
    double  perp_y;

    if (!cub)
        return ;
    tip_x = cx + (int)(dir_x * 5.0);
    tip_y = cy + (int)(dir_y * 5.0);
    base_x = cx - (int)(dir_x * 2.0);
    base_y = cy - (int)(dir_y * 2.0);
    perp_x = -dir_y;
    perp_y = dir_x;
    left_x = base_x + (int)(perp_x * 2.5);
    left_y = base_y + (int)(perp_y * 2.5);
    right_x = base_x - (int)(perp_x * 2.5);
    right_y = base_y - (int)(perp_y * 2.5);
    draw_line(cub, base_x, base_y, tip_x, tip_y, color, 1.0,
        clip_cx, clip_cy, clip_radius);
    draw_line(cub, tip_x, tip_y, left_x, left_y, color, 1.0,
        clip_cx, clip_cy, clip_radius);
    draw_line(cub, tip_x, tip_y, right_x, right_y, color, 1.0,
        clip_cx, clip_cy, clip_radius);
    draw_line(cub, left_x, left_y, right_x, right_y, color, 1.0,
        clip_cx, clip_cy, clip_radius);
}

static bool project_world_point(double wx, double wy, double view_min_x,
        double view_min_y, double span, int diameter, int *out_dx, int *out_dy)
{
    double norm_x;
    double norm_y;
    double screen_x;
    double screen_y;
    int     radius;

    norm_x = (wx - view_min_x) / span;
    norm_y = (wy - view_min_y) / span;
    if (norm_x < 0.0 || norm_x > 1.0 || norm_y < 0.0 || norm_y > 1.0)
        return (false);
    screen_x = norm_x * (double)(diameter - 1);
    screen_y = norm_y * (double)(diameter - 1);
    radius = diameter / 2;
    *out_dx = (int)round(screen_x) - radius;
    *out_dy = (int)round(screen_y) - radius;
    return (true);
}

static void draw_letter(t_cub *cub, int ox, int oy,
        const unsigned char pattern[LETTER_HEIGHT], int color)
{
    int x;
    int y;

    for (y = 0; y < LETTER_HEIGHT; ++y)
    {
        for (x = 0; x < LETTER_WIDTH; ++x)
        {
            if (!(pattern[y] & (1U << (LETTER_WIDTH - 1 - x))))
                continue ;
            blend_pixel(cub, ox + x, oy + y, color, 1.0);
        }
    }
}

static void draw_cardinal_letters(t_cub *cub, int cx, int cy, int radius)
{
    static const unsigned char north[LETTER_HEIGHT] = {
        0b10001,
        0b11001,
        0b10101,
        0b10011,
        0b10001
    };
    static const unsigned char east[LETTER_HEIGHT] = {
        0b11111,
        0b10000,
        0b11110,
        0b10000,
        0b11111
    };
    static const unsigned char south[LETTER_HEIGHT] = {
        0b01111,
        0b10000,
        0b01110,
        0b00001,
        0b11110
    };
    static const unsigned char west[LETTER_HEIGHT] = {
        0b10001,
        0b10001,
        0b10101,
        0b11011,
        0b10001
    };
    int margin;

    if (!cub)
        return ;
    margin = 10;
    draw_letter(cub, cx - LETTER_WIDTH / 2, cy - radius + margin,
        north, MINIMAP_COLOR_TEXT);
    draw_letter(cub, cx + radius - LETTER_WIDTH - margin,
        cy - LETTER_HEIGHT / 2, east, MINIMAP_COLOR_TEXT);
    draw_letter(cub, cx - LETTER_WIDTH / 2,
        cy + radius - LETTER_HEIGHT - margin, south, MINIMAP_COLOR_TEXT);
    draw_letter(cub, cx - radius + margin,
        cy - LETTER_HEIGHT / 2, west, MINIMAP_COLOR_TEXT);
}

static int map_height(char **map)
{
    int h;

    if (!map)
        return (0);
    h = 0;
    while (map[h])
        h++;
    return (h);
}

static bool is_wall_cell(char c)
{
    return (c == '1' || c == 'P' || c == 'D' || c == 'B' || c == '6');
}

static double distance_to_view_boundary(double px, double py, double dx,
        double dy, double view_min_x, double view_min_y, double span)
{
    double max_t;
    double limit;

    max_t = span * 2.0;
    if (dx > 1e-6)
    {
        limit = (view_min_x + span - px) / dx;
        if (limit < max_t)
            max_t = limit;
    }
    else if (dx < -1e-6)
    {
        limit = (view_min_x - px) / dx;
        if (limit < max_t)
            max_t = limit;
    }
    if (dy > 1e-6)
    {
        limit = (view_min_y + span - py) / dy;
        if (limit < max_t)
            max_t = limit;
    }
    else if (dy < -1e-6)
    {
        limit = (view_min_y - py) / dy;
        if (limit < max_t)
            max_t = limit;
    }
    if (max_t < 0.0)
        return (0.0);
    return (max_t);
}

static double cast_minimap_ray(t_cub *cub, double start_x, double start_y,
        double dirx, double diry, double max_dist)
{
    int         map_x;
    int         map_y;
    int         step_x;
    int         step_y;
    double      side_dist_x;
    double      side_dist_y;
    double      delta_dist_x;
    double      delta_dist_y;
    double      distance;
    int         side;
    size_t      row_len;

    if (!cub || !cub->map || max_dist <= 0.0)
        return (0.0);
    map_x = (int)floor(start_x);
    map_y = (int)floor(start_y);
    if (dirx == 0.0)
        delta_dist_x = 1e30;
    else
        delta_dist_x = fabs(1.0 / dirx);
    if (diry == 0.0)
        delta_dist_y = 1e30;
    else
        delta_dist_y = fabs(1.0 / diry);
    if (dirx < 0.0)
    {
        step_x = -1;
        side_dist_x = (start_x - (double)map_x) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = ((double)(map_x + 1) - start_x) * delta_dist_x;
    }
    if (diry < 0.0)
    {
        step_y = -1;
        side_dist_y = (start_y - (double)map_y) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = ((double)(map_y + 1) - start_y) * delta_dist_y;
    }
    distance = 0.0;
    while (distance < max_dist)
    {
        if (side_dist_x < side_dist_y)
        {
            distance = side_dist_x;
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0;
        }
        else
        {
            distance = side_dist_y;
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1;
        }
        if (distance > max_dist)
            break ;
        if (map_y < 0 || !cub->map[map_y])
            return (distance);
        row_len = ft_strlen(cub->map[map_y]);
        if (map_x < 0 || map_x >= (int)row_len)
            return (distance);
        if (cub->map[map_y][map_x] == '1'
            || cub->map[map_y][map_x] == 'P'
            || cub->map[map_y][map_x] == 'D'
            || cub->map[map_y][map_x] == 'B')
            return (distance);
    }
    (void)side;
    return (max_dist);
}

void draw_minimap(t_cub *cub)
{
    const char  *row;
    int         row_len;
    int         diameter;
    int         radius;
    int         center_x;
    int         center_y;
    double      player_x;
    double      player_y;
    double      half_span;
    double      world_per_pixel;
    double      view_min_x;
    double      view_min_y;
    int         sy;
    int         sx;
    int         dy;
    int         dx;
    int         x;
    int         y;
    double      world_x;
    double      world_y;
    int         map_x;
    int         map_y;
    int         height;
    int         color;
    int         frame_outer;
    int         frame_inner;
    int         ddx;
    int         ddy;
    int         enemy_dx;
    int         enemy_dy;

    if (!cub || !cub->show_minimap || !cub->map || !cub->player)
        return ;
    if (!cub->player->position || !cub->player->direction)
        return ;
    if (!cub->mlx_data || cub->mlx_bpp <= 0 || cub->mlx_line_size <= 0)
        return ;
    player_x = cub->player->position->x / (double)BLOCK;
    player_y = cub->player->position->y / (double)BLOCK;
    diameter = MINIMAP_DIAMETER;
    radius = diameter / 2;
    center_x = MINIMAP_MARGIN + radius;
    center_y = MINIMAP_MARGIN + radius;
    half_span = MINIMAP_SPAN / 2.0;
    world_per_pixel = MINIMAP_SPAN / (double)diameter;
    view_min_x = player_x - half_span;
    view_min_y = player_y - half_span;
    height = map_height(cub->map);
    for (sy = 0; sy < diameter; ++sy)
    {
        dy = sy - radius;
        y = center_y + dy;
        if (y < 0 || y >= HEIGHT)
            continue ;
        world_y = view_min_y + sy * world_per_pixel;
        map_y = (int)floor(world_y);
        if (map_y < 0 || map_y >= height || !cub->map[map_y])
        {
            row = NULL;
            row_len = 0;
        }
        else
        {
            row = cub->map[map_y];
            row_len = (int)ft_strlen(row);
        }
        for (sx = 0; sx < diameter; ++sx)
        {
            dx = sx - radius;
            if (dx * dx + dy * dy > radius * radius)
                continue ;
            x = center_x + dx;
            if (x < 0 || x >= WIDTH)
                continue ;
            world_x = view_min_x + sx * world_per_pixel;
            map_x = (int)floor(world_x);
            color = MINIMAP_COLOR_BG;
                if (row && map_x >= 0 && map_x < row_len)
                {
                    if (row[map_x] == 'B')
                        color = 0xCC5500;
                    else if (row[map_x] == 'h')
                        color = 0x3BE35A;
                    else if (row[map_x] == '6')
                        color = sample_texture(&cub->wood_tex,
                                (world_x - (double)map_x) * MINIMAP_WALL_TILING,
                                (world_y - (double)map_y) * MINIMAP_WALL_TILING,
                                0x8B6A4F);
                    else if (is_wall_cell(row[map_x]))
                    color = sample_texture(&cub->minimap_wall_tex,
                            (world_x - (double)map_x) * MINIMAP_WALL_TILING,
                            (world_y - (double)map_y) * MINIMAP_WALL_TILING,
                            MINIMAP_COLOR_WALL);
                else
                    color = sample_texture(&cub->minimap_tex,
                            (world_x - (double)map_x) * MINIMAP_FLOOR_TILING,
                            (world_y - (double)map_y) * MINIMAP_FLOOR_TILING,
                            MINIMAP_COLOR_FLOOR);
            }
            blend_pixel(cub, x, y, color, MINIMAP_ALPHA);
        }
    }
    if (!draw_player_icon(cub, center_x, center_y, radius))
    {
        draw_marker(cub, center_x, center_y, 4, center_x, center_y, radius,
            MINIMAP_COLOR_PLAYER);
        {
            double max_view_dist;
            double hit_dist;
            double ray_end_x;
            double ray_end_y;
            int     ray_dx;
            int     ray_dy;

            max_view_dist = distance_to_view_boundary(player_x, player_y,
                    cub->player->cos_angle, cub->player->sin_angle,
                    view_min_x, view_min_y, MINIMAP_SPAN);
            if (max_view_dist < 0.0)
                max_view_dist = 0.0;
            hit_dist = cast_minimap_ray(cub, player_x, player_y,
                    cub->player->cos_angle, cub->player->sin_angle,
                    max_view_dist);
            if (hit_dist < 0.0)
                hit_dist = 0.0;
            if (hit_dist > max_view_dist)
                hit_dist = max_view_dist;
            ray_end_x = player_x + cub->player->cos_angle * hit_dist;
            ray_end_y = player_y + cub->player->sin_angle * hit_dist;
            if (project_world_point(ray_end_x, ray_end_y, view_min_x, view_min_y,
                    MINIMAP_SPAN, diameter, &ray_dx, &ray_dy))
            {
                int end_x = center_x + ray_dx;
                int end_y = center_y + ray_dy;
                draw_line(cub, center_x, center_y, end_x, end_y,
                    MINIMAP_COLOR_PLAYER, 1.0, center_x, center_y, radius);
            }
        }
    }
    if (cub->alien && cub->alien->pos && cub->alien->dir)
    {
        if (project_world_point(cub->alien->pos->x / (double)BLOCK,
                cub->alien->pos->y / (double)BLOCK, view_min_x, view_min_y,
                MINIMAP_SPAN, diameter, &enemy_dx, &enemy_dy))
            draw_direction_arrow(cub, center_x + enemy_dx, center_y + enemy_dy,
                cub->alien->dir->x, cub->alien->dir->y,
                center_x, center_y, radius, MINIMAP_COLOR_ENEMY);
    }
    frame_outer = radius + 2;
    frame_inner = radius - 1;
    for (dy = -frame_outer; dy <= frame_outer; ++dy)
    {
        y = center_y + dy;
        if (y < 0 || y >= HEIGHT)
            continue ;
        for (dx = -frame_outer; dx <= frame_outer; ++dx)
        {
            x = center_x + dx;
            if (x < 0 || x >= WIDTH)
                continue ;
            ddx = dx;
            ddy = dy;
            if (ddx * ddx + ddy * ddy > frame_outer * frame_outer)
                continue ;
            if (ddx * ddx + ddy * ddy < frame_inner * frame_inner)
                continue ;
            blend_pixel(cub, x, y, MINIMAP_COLOR_BORDER, 0.9);
        }
    }
    draw_cardinal_letters(cub, center_x, center_y, radius);
}
