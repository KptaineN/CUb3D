/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:00:00 by nkief             #+#    #+#             */
/*   Updated: 2025/10/08 12:44:28 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include <stdint.h>

int load_fforeground_textures(t_cub *cub);

int load_hit_player_blood(t_cub *cub)
{
    int w;
    int h;

    if (!cub || !cub->mlx)
        return (0);

    cub->hud_lil_dega.img = mlx_xpm_file_to_image(cub->mlx, "textures/HUD/lil_dega.xpm", &w, &h);
    if (!cub->hud_lil_dega.img) return (0);
    cub->hud_lil_dega.data = mlx_get_data_addr(cub->hud_lil_dega.img, &cub->hud_lil_dega.bpp, &cub->hud_lil_dega.sl, &cub->hud_lil_dega.endian);
    cub->hud_lil_dega.w = w; cub->hud_lil_dega.h = h;

    cub->hud_mid_dega.img = mlx_xpm_file_to_image(cub->mlx, "textures/HUD/mid_dega.xpm", &w, &h);
    if (!cub->hud_mid_dega.img) return (0);
    cub->hud_mid_dega.data = mlx_get_data_addr(cub->hud_mid_dega.img, &cub->hud_mid_dega.bpp, &cub->hud_mid_dega.sl, &cub->hud_mid_dega.endian);
    cub->hud_mid_dega.w = w; cub->hud_mid_dega.h = h;

    cub->hud_big_dega.img = mlx_xpm_file_to_image(cub->mlx, "textures/HUD/Big_dega.xpm", &w, &h);
    if (!cub->hud_big_dega.img) return (0);
    cub->hud_big_dega.data = mlx_get_data_addr(cub->hud_big_dega.img, &cub->hud_big_dega.bpp, &cub->hud_big_dega.sl, &cub->hud_big_dega.endian);
    cub->hud_big_dega.w = w; cub->hud_big_dega.h = h;

    cub->hud_game_over.img = mlx_xpm_file_to_image(cub->mlx, "textures/HUD/game_over.xpm", &w, &h);
    if (!cub->hud_game_over.img) return (0);
    cub->hud_game_over.data = mlx_get_data_addr(cub->hud_game_over.img, &cub->hud_game_over.bpp, &cub->hud_game_over.sl, &cub->hud_game_over.endian);
    cub->hud_game_over.w = w; cub->hud_game_over.h = h;

    return (1);
}

int load_hud_textures(t_cub *cub)
{
    if (!cub)
        return (0);
    if (!load_fforeground_textures(cub))
        return (0);
    if (!load_hit_player_blood(cub))
        return (0);
    return (1);
}


int load_fforeground_textures(t_cub *cub)
{
    int w, h;

    if (!cub || !cub->mlx)
        return (0);

    cub->hud_full_hp.img = mlx_xpm_file_to_image(cub->mlx, "textures/HUD/Pv4.xpm", &w, &h);
    if (!cub->hud_full_hp.img) return (0);
    cub->hud_full_hp.data = mlx_get_data_addr(cub->hud_full_hp.img, &cub->hud_full_hp.bpp, &cub->hud_full_hp.sl, &cub->hud_full_hp.endian);
    cub->hud_full_hp.w = w; cub->hud_full_hp.h = h;

    cub->hud_3hp.img = mlx_xpm_file_to_image(cub->mlx, "textures/HUD/Pv3.xpm", &w, &h);
    if (!cub->hud_3hp.img) return (0);
    cub->hud_3hp.data = mlx_get_data_addr(cub->hud_3hp.img, &cub->hud_3hp.bpp, &cub->hud_3hp.sl, &cub->hud_3hp.endian);
    cub->hud_3hp.w = w; cub->hud_3hp.h = h;

    cub->hud_2hp.img = mlx_xpm_file_to_image(cub->mlx, "textures/HUD/Pv2.xpm", &w, &h);
    if (!cub->hud_2hp.img) return (0);
    cub->hud_2hp.data = mlx_get_data_addr(cub->hud_2hp.img, &cub->hud_2hp.bpp, &cub->hud_2hp.sl, &cub->hud_2hp.endian);
    cub->hud_2hp.w = w; cub->hud_2hp.h = h;

    cub->hud_1hp.img = mlx_xpm_file_to_image(cub->mlx, "textures/HUD/Pv1.xpm", &w, &h);
    if (!cub->hud_1hp.img) return (0);
    cub->hud_1hp.data = mlx_get_data_addr(cub->hud_1hp.img, &cub->hud_1hp.bpp, &cub->hud_1hp.sl, &cub->hud_1hp.endian);
    cub->hud_1hp.w = w; cub->hud_1hp.h = h;

    return (1);
}

static int is_magenta_chroma(int color)
{
    unsigned char r = (color >> 16) & 0xFF;
    unsigned char g = (color >> 8) & 0xFF;
    unsigned char b = color & 0xFF;

    /* magenta detection: strong R and B, low G OR exact #FF00FF */
    if (r == 0xFF && g == 0x00 && b == 0xFF)
        return (1);
    if (r > 200 && b > 200 && g < 100)
        return (1);
    if ((int)r + (int)b > 400 && g * 2 < r + b)
        return (1);
    return (0);
}

/* sanitize helper: convert strong-green pixels to magenta in loaded image data */
/* sanitize_green removed: assets are corrected on-disk; runtime chroma kept as magenta */

static void draw_hud_scaled(t_cub *cub, t_tur *tex, int dst_x, int dst_y, int dst_w, int dst_h)
{ 
    int32_t x, y, src_x, src_y;
    int32_t screen_x, screen_y;
    double scale_x, scale_y;
    int src_bytes;
    int dst_bytes;
    unsigned char *src;
    unsigned char *dst;
    unsigned char b, g, r;

    if (!tex || !tex->data || !cub->mlx_data || dst_w <= 0 || dst_h <= 0)
        return;
    scale_x = (double)tex->w / (double)dst_w;
    scale_y = (double)tex->h / (double)dst_h;
    src_bytes = tex->bpp / 8;
    dst_bytes = cub->mlx_bpp / 8;
    for (y = 0; y < dst_h; y++)
    {
        screen_y = dst_y + y;
        if (screen_y < 0 || screen_y >= HEIGHT)
            continue;
        src_y = (int)(y * scale_y);
        if (src_y >= tex->h) src_y = tex->h - 1;
        for (x = 0; x < dst_w; x++)
        {
            screen_x = dst_x + x;
            if (screen_x < 0 || screen_x >= WIDTH)
                continue;
            src_x = (int)(x * scale_x);
            if (src_x >= tex->w) src_x = tex->w - 1;
            src = (unsigned char *)(tex->data + (src_y * tex->sl + src_x * src_bytes));
            b = src[0];
            g = (src_bytes > 1) ? src[1] : 0;
            r = (src_bytes > 2) ? src[2] : 0;
            if (r > 250 && g < 5 && b > 250)
                continue;
            dst = (unsigned char *)(cub->mlx_data + (screen_y * cub->mlx_line_size + screen_x * dst_bytes));
            dst[0] = b;
            if (dst_bytes > 1) dst[1] = g;
            if (dst_bytes > 2) dst[2] = r;
        }
    }
}

static unsigned char clamp_color(double value)
{
    if (value < 0.0)
        value = 0.0;
    if (value > 255.0)
        value = 255.0;
    return (unsigned char)value;
}

static void draw_weapon_icon(t_cub *cub, t_tur *tex, int dst_x, int dst_y,
    double scale, int unlocked, int equipped)
{
    int32_t x, y, src_x, src_y;
    int32_t dst_w, dst_h;
    double brightness;
    int src_bytes;
    int dst_bytes;
    unsigned char *src;
    unsigned char *dst;
    unsigned char b, g, r;

    if (!cub || !tex || !tex->data || !cub->mlx_data)
        return;
    dst_w = (int)(tex->w * scale);
    dst_h = (int)(tex->h * scale);
    if (dst_w <= 0 || dst_h <= 0)
        return;
    src_bytes = tex->bpp / 8;
    dst_bytes = cub->mlx_bpp / 8;
    brightness = unlocked ? 1.0 : 0.35;
    if (equipped)
        brightness = 1.25;
    for (y = 0; y < dst_h; ++y)
    {
        int screen_y = dst_y + y;
        if (screen_y < 0 || screen_y >= HEIGHT)
            continue;
        src_y = (int)((double)y / (double)dst_h * tex->h);
        if (src_y >= tex->h)
            src_y = tex->h - 1;
        for (x = 0; x < dst_w; ++x)
        {
            int screen_x = dst_x + x;
            if (screen_x < 0 || screen_x >= WIDTH)
                continue;
            src_x = (int)((double)x / (double)dst_w * tex->w);
            if (src_x >= tex->w)
                src_x = tex->w - 1;
            src = (unsigned char*)(tex->data + src_y * tex->sl + src_x * src_bytes);
            b = src[0];
            g = (src_bytes > 1) ? src[1] : 0;
            r = (src_bytes > 2) ? src[2] : 0;
            if (r > 250 && g < 10 && b > 250)
                continue;
            double rb = r * brightness;
            double gb = g * brightness;
            double bb = b * brightness;
            if (!unlocked)
            {
                rb *= 0.6;
                gb *= 0.6;
                bb *= 0.6;
            }
            dst = (unsigned char*)(cub->mlx_data + screen_y * cub->mlx_line_size + screen_x * dst_bytes);
            dst[0] = clamp_color(bb);
            if (dst_bytes > 1) dst[1] = clamp_color(gb);
            if (dst_bytes > 2) dst[2] = clamp_color(rb);
        }
    }
}

static void hud_draw_digit_sequence(t_cub *cub, const char *str, int start_x, int start_y, int digit_size)
{
    int idx;
    int x;
    int digit;

    if (!cub)
        return;
    x = start_x;
    idx = 0;
    while (str && str[idx])
    {
        if (ft_isdigit((unsigned char)str[idx]))
        {
            digit = str[idx] - '0';
            if (digit >= 0 && digit < 10 && cub->digit_tex[digit].img)
                draw_hud_scaled(cub, &cub->digit_tex[digit], x, start_y, digit_size, digit_size);
            x += digit_size + 4;
        }
        idx++;
    }
}

static void hud_draw_slash(t_cub *cub, int x, int y, int height)
{
    int i;
    int screen_bytes;
    int px;
    int py;
    int idx;
    unsigned char *pix;

    if (!cub || !cub->mlx_data || cub->mlx_bpp <= 0)
        return;
    screen_bytes = cub->mlx_bpp / 8;
    for (i = 0; i < height; ++i)
    {
        px = x + i / 3;
        py = y + i;
        if (px < 0 || px >= WIDTH || py < 0 || py >= HEIGHT)
            continue;
        idx = py * cub->mlx_line_size + px * screen_bytes;
        pix = (unsigned char *)(cub->mlx_data + idx);
        pix[0] = 215;
        if (screen_bytes > 1)
            pix[1] = 215;
        if (screen_bytes > 2)
            pix[2] = 215;
    }
}

void hud_draw_ammo_counter(t_cub *cub)
{
    char    clip_str[8];
    char    reserve_str[8];
    int     digit_size;
    int     base_x;
    int     base_y;
    int     icon_size;
    int     clip_len;
    int     reserve_len;
    int     total_width;
    int     digits_x;
    int     slash_x;
    t_tur  *icon_tex;
    int     clip;
    int     reserve;

    if (!cub || !cub->player)
        return;
    if (cub->player->current_weapon == WEAPON_MP && cub->player->has_mp)
    {
        clip = cub->player->ammo_clip;
        reserve = cub->player->ammo_reserve;
        icon_tex = &cub->pickup_ammo_tex;
    }
    else if (cub->player->current_weapon == WEAPON_PISTOL && cub->player->has_pistol)
    {
        clip = cub->player->pistol_clip;
        reserve = cub->player->pistol_reserve;
        icon_tex = &cub->pistol_ammo_tex;
    }
    else
        return;
    if (!cub->digit_tex[0].img || !icon_tex->img)
        return;

    digit_size = 22;
    icon_size = digit_size + 6;
    base_y = 16;

    snprintf(clip_str, sizeof(clip_str), "%d", clip);
    snprintf(reserve_str, sizeof(reserve_str), "%d", reserve);

    clip_len = ft_strlen(clip_str);
    reserve_len = ft_strlen(reserve_str);
    total_width = icon_size + 8
        + clip_len * (digit_size + 2)
        + digit_size / 2
        + reserve_len * (digit_size + 2);
    base_x = WIDTH - total_width - 20;
    digits_x = base_x + icon_size + 8;
    slash_x = digits_x + clip_len * (digit_size + 2) + 4;

    draw_hud_scaled(cub, icon_tex, base_x, base_y - 4,
        icon_size, icon_size);

    hud_draw_digit_sequence(cub, clip_str, digits_x, base_y, digit_size);
    hud_draw_slash(cub, slash_x, base_y, digit_size);
    hud_draw_digit_sequence(cub, reserve_str,
        slash_x + digit_size / 2 + 6, base_y, digit_size);
}

void hud_draw_weapon_icons(t_cub *cub)
{
    t_player *p;
    int icon_size;
    int spacing;
    int count;
    int start_y;
    int cursor_y;

    if (!cub || !(p = cub->player))
        return;
    icon_size = 52;
    spacing = icon_size + 10;
    count = (p->has_hammer != 0) + (p->has_mp != 0) + (p->has_pistol != 0);
    if (count <= 0)
        return;
    start_y = HEIGHT / 2 - (count * spacing - 10) / 2;
    if (start_y < 20)
        start_y = 20;
    cursor_y = start_y;
    if (p->has_hammer)
    {
        draw_weapon_icon(cub, &cub->mini_hammer_tex, 28, cursor_y, 0.55,
            1, p->current_weapon == WEAPON_HAMMER);
        cursor_y += spacing;
    }
    if (p->has_mp)
    {
        draw_weapon_icon(cub, &cub->mini_mp_tex, 28, cursor_y, 0.6,
            1, p->current_weapon == WEAPON_MP);
        cursor_y += spacing;
    }
    if (p->has_pistol)
    {
        draw_weapon_icon(cub, &cub->mini_pistol_tex, 28, cursor_y, 0.6,
            1, p->current_weapon == WEAPON_PISTOL);
    }
}

/*
 * Rotate / regenerate HUD hit textures every 25 seconds according to player HP.
 * This is a lightweight approach: we reload the file variants when the timer
 * elapses. cub->hud_reload_timer is in seconds (float/double) and should be
 * updated from the main loop (deduct dt). If not present, we fallback to no-op.
 */
void hud_update_reload(t_cub *cub, double dt)
{
    static double accumulator = 0.0;
    int w, h;

    if (!cub || !cub->mlx || !cub->player)
        return;
    accumulator += dt;
    if (accumulator < 25.0)
        return;
    accumulator = 0.0;

    /* choose variant by player health: we expect files named with suffixes
       like lil_dega_0.xpm.. we fallback to original names if absent. */
    if (cub->player->health >= 3)
    {
        if (cub->hud_lil_dega.img)
            mlx_destroy_image(cub->mlx, cub->hud_lil_dega.img);
        cub->hud_lil_dega.img = mlx_xpm_file_to_image(cub->mlx, "textures/HUD/lil_dega.xpm", &w, &h);
        if (cub->hud_lil_dega.img)
            cub->hud_lil_dega.data = mlx_get_data_addr(cub->hud_lil_dega.img, &cub->hud_lil_dega.bpp, &cub->hud_lil_dega.sl, &cub->hud_lil_dega.endian);
        cub->hud_lil_dega.w = w; cub->hud_lil_dega.h = h;
    }
    if (cub->player->health == 2)
    {
        if (cub->hud_mid_dega.img)
            mlx_destroy_image(cub->mlx, cub->hud_mid_dega.img);
        cub->hud_mid_dega.img = mlx_xpm_file_to_image(cub->mlx, "textures/HUD/mid_dega.xpm", &w, &h);
        if (cub->hud_mid_dega.img)
            cub->hud_mid_dega.data = mlx_get_data_addr(cub->hud_mid_dega.img, &cub->hud_mid_dega.bpp, &cub->hud_mid_dega.sl, &cub->hud_mid_dega.endian);
        cub->hud_mid_dega.w = w; cub->hud_mid_dega.h = h;
    }
    if (cub->player->health <= 1)
    {
        if (cub->hud_big_dega.img)
            mlx_destroy_image(cub->mlx, cub->hud_big_dega.img);
        cub->hud_big_dega.img = mlx_xpm_file_to_image(cub->mlx, "textures/HUD/Big_dega.xpm", &w, &h);
        if (cub->hud_big_dega.img)
            cub->hud_big_dega.data = mlx_get_data_addr(cub->hud_big_dega.img, &cub->hud_big_dega.bpp, &cub->hud_big_dega.sl, &cub->hud_big_dega.endian);
        cub->hud_big_dega.w = w; cub->hud_big_dega.h = h;
    }
}

/* Update HUD state per-frame (fade transitions, timers) */
void hud_update(t_cub *cub, double dt)
{
    if (!cub)
        return;
    /* fade-in game over when dead, fade-out when alive */
    if (cub->player && cub->player->is_dead)
    {
        cub->game_over_alpha += dt * 0.5; /* fade speed: 0.5 per second -> 2s fade */
        if (cub->game_over_alpha > 1.0)
            cub->game_over_alpha = 1.0;
    }
    else
    {
        cub->game_over_alpha -= dt * 0.8; /* faster fade-out */
        if (cub->game_over_alpha < 0.0)
            cub->game_over_alpha = 0.0;
    }
}

void draw_health_bar(t_cub *cub)
{
    t_tur *health_tex = NULL;
    
    if (!cub || !cub->player)
        return;
    if (cub->player->health >= 4)
        health_tex = &cub->hud_full_hp;
    else if (cub->player->health == 3)
        health_tex = &cub->hud_3hp;
    else if (cub->player->health == 2)
        health_tex = &cub->hud_2hp;
    else if (cub->player->health == 1)
        health_tex = &cub->hud_1hp;
    if (health_tex && health_tex->data)
    {
    /* draw health texture scaled to fill the entire screen (foreground overlay) */
    draw_hud_scaled(cub, health_tex, 0, 0, WIDTH, HEIGHT);
    }
}

void draw_damage_flash(t_cub *cub)
{
    t_tur *flash_tex = NULL;
    
    if (!cub || !cub->player)
        return;
    if (cub->player->damage_flash_timer > 0.0)
    {
        if (cub->player->damage_level == 1)
            flash_tex = &cub->hud_lil_dega;
        else if (cub->player->damage_level == 2)
            flash_tex = &cub->hud_mid_dega;
        else if (cub->player->damage_level >= 3)
            flash_tex = &cub->hud_big_dega;
        if (flash_tex && flash_tex->data)
            draw_hud_scaled(cub, flash_tex, 0, 0, WIDTH, HEIGHT);
    }
}

void draw_game_over(t_cub *cub)
{
    
    if (!cub || !cub->player)
        return;
    if (cub->hud_game_over.data && cub->game_over_alpha > 0.001)
    {
        /* draw full-screen but blend according to game_over_alpha */
        /* We reuse draw_hud_scaled to copy pixels but do per-pixel alpha blending */
        int dst_w = WIDTH;
        int dst_h = HEIGHT;
        int x, y, src_x, src_y;
        double sx = (double)cub->hud_game_over.w / (double)dst_w;
        double sy = (double)cub->hud_game_over.h / (double)dst_h;
        int bytes = cub->mlx_bpp / 8;
        size_t max_bytes = (size_t)cub->mlx_line_size * (size_t)HEIGHT;
        for (y = 0; y < dst_h; y++)
        {
            int screen_y = y;
            if (screen_y < 0 || screen_y >= HEIGHT) continue;
            src_y = (int)(y * sy);
            if (src_y >= cub->hud_game_over.h) src_y = cub->hud_game_over.h - 1;
            for (x = 0; x < dst_w; x++)
            {
                int screen_x = x;
                if (screen_x < 0 || screen_x >= WIDTH) continue;
                src_x = (int)(x * sx);
                if (src_x >= cub->hud_game_over.w) src_x = cub->hud_game_over.w - 1;
                int color = *(int *)(cub->hud_game_over.data + (src_y * cub->hud_game_over.sl + src_x * (cub->hud_game_over.bpp / 8)));
                if (is_magenta_chroma(color))
                    continue;
                /* blend with existing pixel safely */
                if (bytes <= 0)
                    continue;
                size_t dst_offset = (size_t)screen_y * (size_t)cub->mlx_line_size + (size_t)screen_x * (size_t)bytes;
                if (dst_offset + (size_t)bytes > max_bytes)
                    continue;
                int dst_color = *(int *)(cub->mlx_data + dst_offset);
                unsigned char sr = (color >> 16) & 0xFF;
                unsigned char sg = (color >> 8) & 0xFF;
                unsigned char sb = color & 0xFF;
                unsigned char dr = (dst_color >> 16) & 0xFF;
                unsigned char dg = (dst_color >> 8) & 0xFF;
                unsigned char db = dst_color & 0xFF;
                double alpha = cub->game_over_alpha;
                unsigned char rr = (unsigned char)(alpha * sr + (1.0 - alpha) * dr);
                unsigned char rg = (unsigned char)(alpha * sg + (1.0 - alpha) * dg);
                unsigned char rb = (unsigned char)(alpha * sb + (1.0 - alpha) * db);
                *(int *)(cub->mlx_data + dst_offset) = (rr << 16) | (rg << 8) | rb;
            }
        }
    }
}

void cleanup_hud_textures(t_cub *cub)
{
    if (!cub || !cub->mlx)
        return;
    if (cub->hud_full_hp.img)
        mlx_destroy_image(cub->mlx, cub->hud_full_hp.img);
    if (cub->hud_3hp.img)
        mlx_destroy_image(cub->mlx, cub->hud_3hp.img);
    if (cub->hud_2hp.img)
        mlx_destroy_image(cub->mlx, cub->hud_2hp.img);
    if (cub->hud_1hp.img)
        mlx_destroy_image(cub->mlx, cub->hud_1hp.img);
    if (cub->hud_lil_dega.img)
        mlx_destroy_image(cub->mlx, cub->hud_lil_dega.img);
    if (cub->hud_mid_dega.img)
        mlx_destroy_image(cub->mlx, cub->hud_mid_dega.img);
    if (cub->hud_big_dega.img)
        mlx_destroy_image(cub->mlx, cub->hud_big_dega.img);
    if (cub->hud_game_over.img)
        mlx_destroy_image(cub->mlx, cub->hud_game_over.img);
}
