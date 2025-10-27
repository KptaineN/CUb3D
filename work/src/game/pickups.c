#include "../../includes/cub3d.h"

#define PICKUP_TRIGGER_RADIUS (BLOCK * 0.5)
#define PICKUP_TRIGGER_RADIUS_SQ (PICKUP_TRIGGER_RADIUS * PICKUP_TRIGGER_RADIUS)
#define MP_PICKUP_AMOUNT 30
#define PISTOL_PICKUP_AMOUNT 6

static void pickup_apply_health(t_cub *cub, t_pickup *pickup)
{
    t_player    *p;

    (void)pickup;
    if (!cub || !(p = cub->player))
        return ;
    if (p->health >= p->max_health)
        return ;
    p->health += 1;
    if (p->health > p->max_health)
        p->health = p->max_health;
    if (p->health >= 3)
        p->damage_level = 0;
    else if (p->damage_level > 0)
        p->damage_level -= 1;
    p->damage_flash_timer = 0.0;
    p->is_invincible = 0;
    p->invincibility_timer = 0.0;
    p->pickup_touch_timer = 0.6;
}

static void pickup_apply_mp_ammo(t_cub *cub, t_pickup *pickup)
{
    t_player    *p;

    (void)pickup;
    if (!cub || !(p = cub->player))
        return ;
    p->ammo_reserve += MP_PICKUP_AMOUNT;
    if (p->current_weapon == WEAPON_MP && p->ammo_clip < p->ammo_clip_size)
        weapon_reload(p);
    p->pickup_touch_timer = 0.6;
}

static void pickup_apply_pistol_ammo(t_cub *cub, t_pickup *pickup)
{
    t_player    *p;

    (void)pickup;
    if (!cub || !(p = cub->player))
        return ;
    p->pistol_reserve += PISTOL_PICKUP_AMOUNT;
    if (p->current_weapon == WEAPON_PISTOL
        && p->pistol_clip < p->pistol_clip_size)
        weapon_reload(p);
    p->pickup_touch_timer = 0.6;
}

static void pickup_apply_weapon_pistol(t_cub *cub)
{
    t_player *p;

    if (!cub || !(p = cub->player))
        return;
    if (!p->has_pistol)
    {
        p->has_pistol = 1;
        if (p->pistol_clip <= 0)
            p->pistol_clip = p->pistol_clip_size;
        p->pistol_reserve += PISTOL_PICKUP_AMOUNT;
        if (p->current_weapon == WEAPON_FISTS)
            p->current_weapon = WEAPON_PISTOL;
    }
    else
        pickup_apply_pistol_ammo(cub, NULL);
    if (p->pistol_clip > p->pistol_clip_size)
    {
        p->pistol_reserve += (p->pistol_clip - p->pistol_clip_size);
        p->pistol_clip = p->pistol_clip_size;
    }
    p->pickup_touch_timer = 0.6;
}

static void pickup_apply_weapon_mp(t_cub *cub)
{
    t_player *p;

    if (!cub || !(p = cub->player))
        return;
    if (!p->has_mp)
    {
        p->has_mp = 1;
        if (p->ammo_clip <= 0)
            p->ammo_clip = p->ammo_clip_size;
        p->ammo_reserve += MP_PICKUP_AMOUNT;
        if (p->current_weapon == WEAPON_FISTS)
            p->current_weapon = WEAPON_MP;
    }
    else
        pickup_apply_mp_ammo(cub, NULL);
    if (p->ammo_clip > p->ammo_clip_size)
    {
        p->ammo_reserve += (p->ammo_clip - p->ammo_clip_size);
        p->ammo_clip = p->ammo_clip_size;
    }
    p->pickup_touch_timer = 0.6;
}

static void pickup_apply_weapon_hammer(t_cub *cub)
{
    t_player *p;

    if (!cub || !(p = cub->player))
        return;
    if (!p->has_hammer)
    {
        p->has_hammer = 1;
        if (p->current_weapon == WEAPON_FISTS)
            p->current_weapon = WEAPON_HAMMER;
    }
    p->pickup_touch_timer = 0.6;
}

static void pickup_collect(t_cub *cub, t_pickup *pickup)
{
    if (!cub || !pickup || !pickup->active)
        return ;
    if (cub->map && cub->map[pickup->map_y])
        cub->map[pickup->map_y][pickup->map_x] = '0';
    if (pickup->type == PICKUP_HEALTH)
        pickup_apply_health(cub, pickup);
    else if (pickup->type == PICKUP_MP_AMMO)
        pickup_apply_mp_ammo(cub, pickup);
    else if (pickup->type == PICKUP_PISTOL_AMMO)
        pickup_apply_pistol_ammo(cub, pickup);
    else if (pickup->type == PICKUP_WEAPON_PISTOL)
        pickup_apply_weapon_pistol(cub);
    else if (pickup->type == PICKUP_WEAPON_MP)
        pickup_apply_weapon_mp(cub);
    else if (pickup->type == PICKUP_WEAPON_HAMMER)
        pickup_apply_weapon_hammer(cub);
    pickup->active = 0;
}

int pickup_system_init(t_cub *cub)
{
    int count;
    int y;
    int x;
    int idx;
    char tile;

    if (!cub)
    {
        return (1);
    }
    if (!cub->map)
    {
        return (0);
    }
    count = 0;
    y = 0;
    while (cub->map[y])
    {
        x = 0;
        while (cub->map[y][x])
        {
            tile = cub->map[y][x];
            if (tile == 'M' || tile == 'm' || tile == 'h' || tile == 'H'
                || tile == 'K' || tile == 'k' || tile == 'J' || tile == 'j'
                || tile == 'T' || tile == 't' || tile == 'I' || tile == 'i')
                count++;
            x++;
        }
        y++;
    }
    cub->pickups = NULL;
    cub->pickup_count = 0;
    if (count == 0)
        return (0);
    cub->pickups = (t_pickup *)ft_calloc(count, sizeof(t_pickup));
    if (!cub->pickups)
        return (ft_putendl_fd("Error: failed to allocate pickups", 2), 1);
    cub->pickup_count = count;
    idx = 0;
    y = 0;
    while (cub->map[y])
    {
        x = 0;
        while (cub->map[y][x])
        {
            tile = cub->map[y][x];
            if ((tile == 'M' || tile == 'm' || tile == 'h' || tile == 'H'
                || tile == 'K' || tile == 'k' || tile == 'J' || tile == 'j'
                || tile == 'T' || tile == 't' || tile == 'I' || tile == 'i')
                && idx < count)
            {
                cub->pickups[idx].map_x = x;
                cub->pickups[idx].map_y = y;
                cub->pickups[idx].x = (x + 0.5) * (double)BLOCK;
                cub->pickups[idx].y = (y + 0.5) * (double)BLOCK;
                cub->pickups[idx].active = 1;
                if (tile == 'M' || tile == 'm')
                {
                    cub->pickups[idx].type = PICKUP_MP_AMMO;
                    cub->map[y][x] = 'M';
                }
                else if (tile == 'K' || tile == 'k')
                {
                    cub->pickups[idx].type = PICKUP_PISTOL_AMMO;
                    cub->map[y][x] = 'K';
                }
                else if (tile == 'h' || tile == 'H')
                {
                    cub->pickups[idx].type = PICKUP_HEALTH;
                    cub->map[y][x] = 'h';
                }
                else if (tile == 'I' || tile == 'i')
                {
                    cub->pickups[idx].type = PICKUP_WEAPON_HAMMER;
                    cub->map[y][x] = 'I';
                }
                else if (tile == 'J' || tile == 'j')
                {
                    cub->pickups[idx].type = PICKUP_WEAPON_PISTOL;
                    cub->map[y][x] = 'J';
                }
                else if (tile == 'T' || tile == 't')
                {
                    cub->pickups[idx].type = PICKUP_WEAPON_MP;
                    cub->map[y][x] = 'T';
                }
                idx++;
            }
            x++;
        }
        y++;
    }
    return (0);
}

void pickup_system_destroy(t_cub *cub)
{
    if (!cub)
        return ;
    free(cub->pickups);
    cub->pickups = NULL;
    cub->pickup_count = 0;
}

void pickup_update(t_cub *cub)
{
    int         i;
    t_pickup    *p;
    double      dx;
    double      dy;
    double      dist_sq;

    if (!cub || !cub->player || !cub->player->position || !cub->pickups)
        return ;
    i = 0;
    while (i < cub->pickup_count)
    {
        p = &cub->pickups[i];
        if (p->active)
        {
            dx = cub->player->position->x - p->x;
            dy = cub->player->position->y - p->y;
            dist_sq = dx * dx + dy * dy;
            if (dist_sq <= PICKUP_TRIGGER_RADIUS_SQ)
            {
                if (p->type == PICKUP_HEALTH
                    && cub->player->health >= cub->player->max_health)
                {
                    /* laisser le soin pour plus tard */
                }
                else
                    pickup_collect(cub, p);
            }
        }
        i++;
    }
}

static void pickup_draw_sprite(t_cub *cub, t_tur *tex,
    double transform_y, int draw_start_y, int draw_end_y,
    int draw_start_x, int draw_end_x)
{
    int bytes;
    int spanx;
    int spany;
    int x;
    int y;
    double tx;
    int tex_x;
    double ty;
    int tex_y;
    int tidx;
    unsigned char *src;
    unsigned char b;
    unsigned char g;
    unsigned char r;
    int color;

    if (!cub || !tex || !tex->data)
        return ;
    bytes = tex->bpp / 8;
    if (bytes <= 0 || tex->w <= 0 || tex->h <= 0)
        return ;
    spanx = draw_end_x - draw_start_x + 1;
    spany = draw_end_y - draw_start_y + 1;
    x = draw_start_x;
    while (x <= draw_end_x)
    {
        if (x >= 0 && x < WIDTH && transform_y < cub->zbuffer[x] - 1e-6)
        {
            tx = (double)(x - draw_start_x) / (double)spanx;
            tex_x = (int)(tx * tex->w);
            if (tex_x < 0)
                tex_x = 0;
            if (tex_x >= tex->w)
                tex_x = tex->w - 1;
            y = draw_start_y;
            while (y <= draw_end_y)
            {
                if (y >= 0 && y < HEIGHT)
                {
                    ty = (double)(y - draw_start_y) / (double)spany;
                    tex_y = (int)(ty * tex->h);
                    if (tex_y < 0)
                        tex_y = 0;
                    if (tex_y >= tex->h)
                        tex_y = tex->h - 1;
                    tidx = tex_y * tex->sl + tex_x * bytes;
                    src = (unsigned char *)(tex->data + tidx);
                    b = src[0];
                    g = (bytes > 1) ? src[1] : 0;
                    r = (bytes > 2) ? src[2] : 0;
                    if (!(r > 250 && g < 5 && b > 250))
                    {
                        color = *(int *)(tex->data + tidx);
                        put_pixel(x, y, color, cub);
                    }
                }
                y++;
            }
        }
        x++;
    }
}

void pickup_render(t_cub *cub)
{
    int             i;
    t_pickup        *p;
    t_tur           *tex;
    double          dir_x;
    double          dir_y;
    double          plane_x;
    double          plane_y;
    double          inv_det;
    double          sprite_x;
    double          sprite_y;
    double          transform_x;
    double          transform_y;
    int             sprite_screen_x;
    int             sprite_height;
    int             draw_start_y;
    int             draw_end_y;
    int             sprite_width;
    int             draw_start_x;
    int             draw_end_x;
    struct timeval  tv;
    double          anim_time;

    if (!cub || !cub->player || !cub->player->position
        || !cub->player->direction || !cub->player->plane
        || !cub->pickups || cub->pickup_count <= 0)
        return ;

    gettimeofday(&tv, NULL);
    anim_time = tv.tv_sec + tv.tv_usec / 1000000.0;

    dir_x = cub->player->direction->x;
    dir_y = cub->player->direction->y;
    plane_x = cub->player->plane->x;
    plane_y = cub->player->plane->y;
    inv_det = 1.0 / (plane_x * dir_y - dir_x * plane_y);
    i = 0;
    while (i < cub->pickup_count)
    {
        p = &cub->pickups[i];
        if (!p->active)
        {
            i++;
            continue;
        }
        tex = NULL;
        if (p->type == PICKUP_HEALTH)
            tex = &cub->pickup_medkit_tex;
        else if (p->type == PICKUP_MP_AMMO)
            tex = &cub->pickup_ammo_tex;
        else if (p->type == PICKUP_PISTOL_AMMO)
            tex = &cub->pistol_ammo_tex;
        else if (p->type == PICKUP_WEAPON_PISTOL)
        {
            if (cub->player && cub->player->has_pistol)
            {
                i++;
                continue;
            }
            tex = &cub->mini_pistol_tex;
        }
        else if (p->type == PICKUP_WEAPON_MP)
        {
            if (cub->player && cub->player->has_mp)
            {
                i++;
                continue;
            }
            tex = &cub->mini_mp_tex;
        }
        else if (p->type == PICKUP_WEAPON_HAMMER)
        {
            if (cub->player && cub->player->has_hammer)
            {
                i++;
                continue;
            }
            tex = &cub->mini_hammer_tex;
        }
        else
        {
            i++;
            continue;
        }
        if (!tex || !tex->img || !tex->data)
        {
            i++;
            continue;
        }
        sprite_x = (p->x - cub->player->position->x) / (double)BLOCK;
        sprite_y = (p->y - cub->player->position->y) / (double)BLOCK;
        transform_x = inv_det * (dir_y * sprite_x - dir_x * sprite_y);
        transform_y = inv_det * (-plane_y * sprite_x + plane_x * sprite_y);
        if (transform_y <= 0.05)
        {
            i++;
            continue;
        }
        sprite_screen_x = (int)((WIDTH / 2.0) * (1.0 + transform_x / transform_y));
        sprite_height = abs((int)(HEIGHT / transform_y));
        sprite_height = (int)(sprite_height * 0.45);
        if (sprite_height <= 0)
            sprite_height = 1;
        double spin = fabs(sin(anim_time * 3.0 + i));
        double scale = 0.75 + 0.25 * spin;
        sprite_height = (int)(sprite_height * scale);
        if (sprite_height <= 0)
            sprite_height = 1;
        draw_start_y = -sprite_height / 2 + HEIGHT / 2;
        draw_end_y = sprite_height / 2 + HEIGHT / 2;
        int bob = (int)(sin(anim_time * 2.5 + i) * 10.0);
        draw_start_y += bob;
        draw_end_y += bob;
        if (draw_start_y < 0)
            draw_start_y = 0;
        if (draw_end_y >= HEIGHT)
            draw_end_y = HEIGHT - 1;
        sprite_width = sprite_height;
        draw_start_x = -sprite_width / 2 + sprite_screen_x;
        if (draw_start_x < 0)
            draw_start_x = 0;
        draw_end_x = sprite_width / 2 + sprite_screen_x;
        if (draw_end_x >= WIDTH)
            draw_end_x = WIDTH - 1;
        pickup_draw_sprite(cub, tex, transform_y,
            draw_start_y, draw_end_y, draw_start_x, draw_end_x);
        i++;
    }
}
