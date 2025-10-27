#include "../../includes/cub3d.h"

#define BARREL_EXPLOSION_FRAMES 11
#define BARREL_FRAME_TIME 0.08
#define BARREL_CHAIN_MARGIN (BLOCK * 0.5)

static void barrel_start_explosion(t_cub *cub, t_barrel *barrel);
static void barrel_chain_explosion(t_cub *cub, t_barrel *source);

static t_barrel *barrel_find_by_cell(t_cub *cub, int map_x, int map_y)
{
    if (!cub || !cub->barrels)
        return (NULL);
    for (int i = 0; i < cub->barrel_count; ++i)
    {
        if (cub->barrels[i].map_x == map_x && cub->barrels[i].map_y == map_y)
            return (&cub->barrels[i]);
    }
    return (NULL);
}

static void barrel_kill_player(t_cub *cub)
{
    t_player *p;

    if (!cub || !(p = cub->player) || p->is_dead)
        return ;
    p->health = 0;
    p->is_dead = 1;
    if (p->death_timer <= 0.0)
        p->death_timer = 3.0;
    p->damage_level = 3;
    p->damage_flash_timer = 0.6;
    p->is_invincible = 0;
    p->invincibility_timer = 0.0;
}

static void barrel_apply_damage(t_cub *cub, t_barrel *barrel)
{
    double radius;

    if (!cub || !barrel)
        return ;
    radius = barrel->explosion_radius;
    if (cub->alien && cub->alien->pos)
    {
        double dx = cub->alien->pos->x - barrel->x;
        double dy = cub->alien->pos->y - barrel->y;
        if (sqrt(dx * dx + dy * dy) <= radius)
        {
            alien_free(cub);
        }
    }
    if (cub->player && cub->player->position)
    {
        double dx = cub->player->position->x - barrel->x;
        double dy = cub->player->position->y - barrel->y;
        if (sqrt(dx * dx + dy * dy) <= radius)
            barrel_kill_player(cub);
    }
}

static void barrel_chain_explosion(t_cub *cub, t_barrel *source)
{
    if (!cub || !cub->barrels || !source)
        return ;
    for (int i = 0; i < cub->barrel_count; ++i)
    {
        t_barrel *b = &cub->barrels[i];
        if (b == source || b->state != BARREL_INTACT)
            continue;
        double dx = b->x - source->x;
        double dy = b->y - source->y;
        if (sqrt(dx * dx + dy * dy) <= source->explosion_radius + BARREL_CHAIN_MARGIN)
        {
            barrel_start_explosion(cub, b);
        }
    }
}

static void barrel_start_explosion(t_cub *cub, t_barrel *barrel)
{
    if (!cub || !barrel || barrel->state != BARREL_INTACT)
        return ;
    barrel->state = BARREL_EXPLODING;
    barrel->anim_timer = 0.0;
    barrel->anim_frame = 0;
    barrel_apply_damage(cub, barrel);
    if (cub->map && cub->map[barrel->map_y])
        cub->map[barrel->map_y][barrel->map_x] = '0';
    barrel_chain_explosion(cub, barrel);
}

int barrel_system_init(t_cub *cub)
{
    int count;
    int y;
    int x;
    int idx;

    if (!cub || !cub->map)
        return (0);
    count = 0;
    y = 0;
    while (cub->map[y])
    {
        x = 0;
        while (cub->map[y][x])
        {
            if (cub->map[y][x] == 'B')
                count++;
            x++;
        }
        y++;
    }
    cub->barrels = NULL;
    cub->barrel_count = 0;
    if (count == 0)
        return (0);
    cub->barrels = (t_barrel *)ft_calloc(count, sizeof(t_barrel));
    if (!cub->barrels)
        return (ft_putendl_fd("Error: failed to allocate barrels", 2), 1);
    cub->barrel_count = count;
    idx = 0;
    y = 0;
    while (cub->map[y])
    {
        x = 0;
        while (cub->map[y][x])
        {
            if (cub->map[y][x] == 'B' && idx < count)
            {
                t_barrel *b = &cub->barrels[idx++];
                b->map_x = x;
                b->map_y = y;
                b->x = (x + 0.5) * (double)BLOCK;
                b->y = (y + 0.5) * (double)BLOCK;
                b->state = BARREL_INTACT;
                b->anim_timer = 0.0;
                b->anim_frame = 0;
                b->explosion_radius = BLOCK * 2.0;
            }
            x++;
        }
        y++;
    }
    return (0);
}

void barrel_system_destroy(t_cub *cub)
{
    if (!cub)
        return ;
    free(cub->barrels);
    cub->barrels = NULL;
    cub->barrel_count = 0;
}

void barrel_update(t_cub *cub, double dt)
{
    if (!cub || !cub->barrels || cub->barrel_count <= 0)
        return ;
    for (int i = 0; i < cub->barrel_count; ++i)
    {
        t_barrel *b = &cub->barrels[i];
        if (b->state == BARREL_EXPLODING)
        {
            b->anim_timer += dt;
            int frame = (int)(b->anim_timer / BARREL_FRAME_TIME);
            if (frame >= BARREL_EXPLOSION_FRAMES)
            {
                b->state = BARREL_DESTROYED;
                b->anim_frame = BARREL_EXPLOSION_FRAMES - 1;
            }
            else
                b->anim_frame = frame;
        }
    }
}

static void barrel_draw_sprite(t_cub *cub, t_tur *tex, double transformY, int draw_start_y, int draw_end_y, int draw_start_x, int draw_end_x)
{
    if (!cub || !tex || !tex->data)
        return ;
    int bytes = tex->bpp / 8;
    if (bytes <= 0 || tex->w <= 0 || tex->h <= 0)
        return ;
    int spanx = draw_end_x - draw_start_x + 1;
    int spany = draw_end_y - draw_start_y + 1;
    for (int x = draw_start_x; x <= draw_end_x; ++x)
    {
        if (x < 0 || x >= WIDTH)
            continue;
        if (transformY >= cub->zbuffer[x] - 1e-6)
            continue;
        double tx = (double)(x - draw_start_x) / (double)spanx;
        int tex_x = (int)(tx * tex->w);
        if (tex_x < 0)
            tex_x = 0;
        if (tex_x >= tex->w)
            tex_x = tex->w - 1;
        for (int y = draw_start_y; y <= draw_end_y; ++y)
        {
            if (y < 0 || y >= HEIGHT)
                continue;
            double ty = (double)(y - draw_start_y) / (double)spany;
            int tex_y = (int)(ty * tex->h);
            if (tex_y < 0)
                tex_y = 0;
            if (tex_y >= tex->h)
                tex_y = tex->h - 1;
            int tidx = tex_y * tex->sl + tex_x * bytes;
            unsigned char *src = (unsigned char *)(tex->data + tidx);
            unsigned char b = src[0];
            unsigned char g = (bytes > 1) ? src[1] : 0;
            unsigned char r = (bytes > 2) ? src[2] : 0;
            if (r > 250 && g < 5 && b > 250)
                continue;
            int color = *(int *)(tex->data + tidx);
            put_pixel(x, y, color, cub);
        }
    }
}

void barrel_render(t_cub *cub)
{
    if (!cub || !cub->player || !cub->player->position || !cub->player->direction || !cub->player->plane)
        return ;
    if (!cub->barrels || cub->barrel_count <= 0)
        return ;
    double dirX = cub->player->direction->x;
    double dirY = cub->player->direction->y;
    double planeX = cub->player->plane->x;
    double planeY = cub->player->plane->y;
    double invDet = 1.0 / (planeX * dirY - dirX * planeY);
    for (int i = 0; i < cub->barrel_count; ++i)
    {
        t_barrel *b = &cub->barrels[i];
        t_tur *tex;
        if (b->state == BARREL_DESTROYED)
            continue;
        if (b->state == BARREL_EXPLODING)
        {
            int frame = b->anim_frame;
            if (frame < 0)
                frame = 0;
            if (frame >= BARREL_EXPLOSION_FRAMES)
                frame = BARREL_EXPLOSION_FRAMES - 1;
            tex = &cub->barrel_explosion[frame];
        }
        else
            tex = &cub->barrel_tex;
        if (!tex || !tex->img || !tex->data)
            continue;
        double spriteX = (b->x - cub->player->position->x) / (double)BLOCK;
        double spriteY = (b->y - cub->player->position->y) / (double)BLOCK;
        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY);
        if (transformY <= 0.05)
            continue;
        int spriteScreenX = (int)((WIDTH / 2.0) * (1.0 + transformX / transformY));
        int spriteHeight = abs((int)(HEIGHT / transformY));
        spriteHeight = (int)(spriteHeight * 0.65);
        if (spriteHeight <= 0)
            spriteHeight = 1;
        int drawStartY = -spriteHeight / 2 + HEIGHT / 2;
        if (drawStartY < 0)
            drawStartY = 0;
        int drawEndY = spriteHeight / 2 + HEIGHT / 2;
        if (drawEndY >= HEIGHT)
            drawEndY = HEIGHT - 1;
        int spriteWidth = spriteHeight;
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0)
            drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= WIDTH)
            drawEndX = WIDTH - 1;
        barrel_draw_sprite(cub, tex, transformY, drawStartY, drawEndY, drawStartX, drawEndX);
    }
}

static int barrel_tile_blocking(char tile)
{
    return (tile == '1' || tile == 'P' || tile == 'D' || tile == ' ');
}

static int barrel_map_tile(t_cub *cub, int map_x, int map_y, char *tile_out)
{
    size_t row_len;

    if (tile_out)
        *tile_out = '1';
    if (!cub || !cub->map || map_y < 0 || map_x < 0)
        return (-1);
    if (!cub->map[map_y])
        return (-1);
    row_len = ft_strlen(cub->map[map_y]);
    if ((size_t)map_x >= row_len)
        return (-1);
    if (tile_out)
        *tile_out = cub->map[map_y][map_x];
    return (0);
}

int barrel_handle_shot(t_cub *cub)
{
    double px;
    double py;
    double dirX;
    double dirY;
    double max_dist;
    double step;

    if (!cub || !cub->player || !cub->barrels || cub->barrel_count <= 0)
        return (0);
    px = cub->player->position->x;
    py = cub->player->position->y;
    dirX = cub->player->direction->x;
    dirY = cub->player->direction->y;
    max_dist = 600.0;
    step = BLOCK / 6.0;
    for (double dist = cub->player->radius; dist <= max_dist; dist += step)
    {
        double sample_x = px + dirX * dist;
        double sample_y = py + dirY * dist;
        int map_x = (int)floor(sample_x / (double)BLOCK);
        int map_y = (int)floor(sample_y / (double)BLOCK);
        char tile;
        if (barrel_map_tile(cub, map_x, map_y, &tile) != 0)
            return (0);
        if (tile == 'B')
        {
            t_barrel *barrel = barrel_find_by_cell(cub, map_x, map_y);
            if (barrel && barrel->state == BARREL_INTACT)
            {
                barrel_start_explosion(cub, barrel);
                return (1);
            }
        }
        if (barrel_tile_blocking(tile))
            break;
    }
    return (0);
}

int barrel_try_push(t_cub *cub, int map_x, int map_y, int dir_x, int dir_y)
{
    t_barrel *barrel;
    char      dest_tile;
    int       dest_x;
    int       dest_y;

    if (!cub || (dir_x == 0 && dir_y == 0))
        return (0);
    barrel = barrel_find_by_cell(cub, map_x, map_y);
    if (!barrel)
        return (0);
    dest_x = map_x + dir_x;
    dest_y = map_y + dir_y;
    if (barrel_map_tile(cub, dest_x, dest_y, &dest_tile) != 0)
        return (0);
    if (dest_tile == '1' || dest_tile == 'P' || dest_tile == 'D'
        || dest_tile == 'B' || dest_tile == ' ')
        return (0);
    if (dest_tile != '0')
        return (0);
    if (cub->map && cub->map[barrel->map_y])
        cub->map[barrel->map_y][barrel->map_x] = '0';
    barrel->map_x = dest_x;
    barrel->map_y = dest_y;
    barrel->x = (dest_x + 0.5) * (double)BLOCK;
    barrel->y = (dest_y + 0.5) * (double)BLOCK;
    if (cub->map && cub->map[dest_y])
        cub->map[dest_y][dest_x] = 'B';
    return (1);
}
