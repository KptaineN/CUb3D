/*
une texture, c’est une image. quand ton rayon tape un mur, 
récupère où il <<tape sur cette face (un nombre wall_x entre 0 et 1),
le convertis en colonne de la texture (tex_x),
puis pour chaque pixel écran de la colonne, 
prend la bonne ligne de la texture (tex_y) et tu copies la couleur → un sticker vertical.
*/

#include "../../includes/cub3d.h"

static int textur_load(t_cub *cub, t_tur *t, const char *path)
{
    t->img = mlx_xpm_file_to_image(cub->mlx, (char *)path, &t->w, &t->h);
    if (!t->img)
        return (-1);
    t->data = mlx_get_data_addr(t->img, &t->bpp, &t->sl, &t->endian);
    if (!t->data)
    {
        /* cleanup image if data retrieval failed to avoid MLX allocation leak */
        if (t->img && cub->mlx)
            mlx_destroy_image(cub->mlx, t->img);
        t->img = NULL;
        return (-1);
    }
    return (0);
}

/*
rtical : tu entres par la droite (→ OUEST) si tu vas vers +X ; par la gauche (→ EST) si tu vas vers −X.

horizontal : tu entres par le bas (→ SUD) si tu vas vers +Y ; par le haut (→ NORD) si tu vas vers −Y.
*/
t_tur *pick_face(t_cub *cub, int side, double rx, double ry, char tile)
{
    t_tur *face;
    if (tile == 'P' && cub->panel_tex.img && cub->panel_tex.data)
        return (&cub->panel_tex);
    if (tile == 'D' && cub->door_tex.img && cub->door_tex.data)
        return (&cub->door_tex);
    if (tile == '6' && cub->wood_tex.img && cub->wood_tex.data)
        return (&cub->wood_tex);
    if (side == 0) // vertical → face EST si on allait vers -X, sinon OUEST
    {
        if (rx < 0.0)
            face = &cub->textur[TEX_EAST];
        else
            face = &cub->textur[TEX_WEST];
    }
    else // horizontal → face NORD si on allait vers -Y, sinon SUD
    {
        if (ry < 0.0)
            face = &cub->textur[TEX_NORTH];
        else
            face = &cub->textur[TEX_SOUTH];
    }
    return face;
}

int compute_tex_x(t_cub *cub, t_tur *tex, int side, double dist, double rx, double ry)
{
    // Système unifié pour l'alignement parfait des textures
    double pos_x = cub->player->position->x / (double)BLOCK;
    double pos_y = cub->player->position->y / (double)BLOCK;
    
    // Point d'intersection avec le mur dans l'espace monde
    double hit_x = pos_x + dist * rx;
    double hit_y = pos_y + dist * ry;
    
    // Coordonnée de texture basée sur la position absolue dans le monde
    double tex_coord;
    
    if (side == 0) {
        // Mur vertical : utilise la coordonnée Y
        tex_coord = hit_y;
    } else {
        // Mur horizontal : utilise la coordonnée X
        tex_coord = hit_x;
    }
    
    // Normaliser entre 0 et 1 (partie fractionnaire)
    tex_coord = tex_coord - floor(tex_coord);
    
    // Assurer que tex_coord est toujours positive
    if (tex_coord < 0) tex_coord += 1.0;
    
    // Lissage anti-loupe : éviter les sauts brusques près des bords
    if (dist < 0.3) {  // Zone critique près des murs
        // Stabilisation douce pour éviter les effets de zoom
        double stability_factor = dist / 0.3;
        if (tex_coord < 0.1 || tex_coord > 0.9) {
            double center_pull = 0.5 - tex_coord;
            tex_coord += center_pull * (1.0 - stability_factor) * 0.1;
        }
    }
    
    // Conversion en coordonnée de texture avec arrondi intelligent
    int tex_x = (int)(tex_coord * (double)tex->w + 0.5);
    
    // Protection des limites
    if (tex_x < 0) tex_x = 0;
    if (tex_x >= tex->w) tex_x = tex->w - 1;
    
    return tex_x;
}


int init_textures(t_cub *cub)
{
    // Charger les textures depuis les chemins définis dans la map
    if (textur_load(cub, &cub->textur[TEX_NORTH], cub->no_path) < 0) return (-1);
    if (textur_load(cub, &cub->textur[TEX_SOUTH], cub->so_path) < 0) return (-1);
    if (textur_load(cub, &cub->textur[TEX_EAST],  cub->ea_path) < 0) return (-1);
    if (textur_load(cub, &cub->textur[TEX_WEST],  cub->we_path) < 0) return (-1);
     // ciel (skybox panoramique) et sol
    if (textur_load(cub, &cub->sky,   "textures/tiles/sky.xpm")   < 0) return (-1);
    if (textur_load(cub, &cub->floor, "textures/tiles/floor.xpm") < 0) return (-1);
    if (textur_load(cub, &cub->panel_tex, "textures/tiles/panel.xpm") < 0) return (-1);
    if (textur_load(cub, &cub->door_tex,  "textures/tiles/door.xpm")  < 0) return (-1);
    if (textur_load(cub, &cub->wood_tex,  "textures/tiles/wood.xpm")  < 0) return (-1);
    if (textur_load(cub, &cub->barrel_tex, "textures/barrel/barrel.xpm") < 0) return (-1);
    for (int i = 0; i < 11; ++i)
    {
        char path[64];
        snprintf(path, sizeof(path), "textures/barrel/barrel%d.xpm", i + 1);
        if (textur_load(cub, &cub->barrel_explosion[i], path) < 0)
            return (-1);
    }
    if (textur_load(cub, &cub->pickup_medkit_tex, "textures/util/soin1.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->pickup_ammo_tex, "textures/util/munition1.xpm") < 0)
        return (-1);
    for (int i = 0; i < 10; ++i)
    {
        char path[64];
        snprintf(path, sizeof(path), "textures/util/num%d.xpm", i);
        if (textur_load(cub, &cub->digit_tex[i], path) < 0)
            return (-1);
    }
    if (textur_load(cub, &cub->pistol_ammo_tex, "textures/util/munition_pistol.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->mp_reload_tex, "textures/gun/mp/reload_mp.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->pistol_idle_tex, "textures/gun/pistol/pistol_nofire.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->pistol_fire_tex, "textures/gun/pistol/pistol_fire.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->fists_idle_tex, "textures/gun/kick/poing_idle.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->fists_touch_tex, "textures/gun/kick/touch.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->kick_tex[0], "textures/gun/kick/kick1.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->kick_tex[1], "textures/gun/kick/kick2.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->mini_pistol_tex, "textures/gun/pistol/mini_pistol.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->mini_mp_tex, "textures/gun/mp/mini_mp.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->mini_hammer_tex, "textures/gun/hammer/mini_hammer.xpm") < 0)
        return (-1);
    /* optional alien texture: try common names, ignore failure gracefully */
    if (textur_load(cub, &cub->alien_tex, "textures/alien/alien.xpm") < 0)
    {
        /* try alternate name alien_front.xpm to be tolerant with naming */
        if (textur_load(cub, &cub->alien_tex, "textures/alien/alien_front.xpm") < 0)
        {
            cub->alien_tex.img = NULL;
            cub->alien_tex.data = NULL;
            cub->alien_tex.w = 0;
            cub->alien_tex.h = 0;
            cub->alien_tex.bpp = 0;
            cub->alien_tex.sl = 0;
            cub->alien_tex.endian = 0;
            cub->alien_tex.mask_cache = NULL; cub->alien_tex.mask_cache_w = 0; cub->alien_tex.mask_cache_h = 0;
        }
        else
        {
            /* build masks now that texture loaded */
            alien_detect_mask(cub);
            extern void alien_build_mask(t_cub *cub);
            alien_build_mask(cub);
        }
    }
    else
    {
        /* texture loaded: let alien module sample corners to detect mask and build mask once */
        alien_detect_mask(cub);
        /* explicit build to avoid building at first draw */
        extern void alien_build_mask(t_cub *cub);
        alien_build_mask(cub);
    }

    /* try loading an alternate back-facing alien texture if present */
    /* load back-facing alien texture from textures/alien/ if present */
    if (textur_load(cub, &cub->alien_back_tex, "textures/alien/alien_back.xpm") < 0)
    {
        cub->alien_back_tex.img = NULL; cub->alien_back_tex.data = NULL;
        cub->alien_back_tex.w = 0; cub->alien_back_tex.h = 0;
        cub->alien_back_tex.bpp = 0; cub->alien_back_tex.sl = 0; cub->alien_back_tex.endian = 0;
        cub->alien_back_tex.mask_cache = NULL; cub->alien_back_tex.mask_cache_w = 0; cub->alien_back_tex.mask_cache_h = 0;
    }

    /* New: load alien-specific action/view textures from textures/alien/ */
    if (textur_load(cub, &cub->alien_w1_tex, "textures/alien/w1.xpm") < 0)
    {
        cub->alien_w1_tex.img = NULL; cub->alien_w1_tex.data = NULL; cub->alien_w1_tex.w=0; cub->alien_w1_tex.h=0; cub->alien_w1_tex.bpp=0; cub->alien_w1_tex.sl=0; cub->alien_w1_tex.endian=0;
        cub->alien_w1_tex.mask_cache = NULL; cub->alien_w1_tex.mask_cache_w = 0; cub->alien_w1_tex.mask_cache_h = 0;
    }
    if (textur_load(cub, &cub->alien_w2_tex, "textures/alien/w2.xpm") < 0)
    {
        cub->alien_w2_tex.img = NULL; cub->alien_w2_tex.data = NULL; cub->alien_w2_tex.w=0; cub->alien_w2_tex.h=0; cub->alien_w2_tex.bpp=0; cub->alien_w2_tex.sl=0; cub->alien_w2_tex.endian=0;
        cub->alien_w2_tex.mask_cache = NULL; cub->alien_w2_tex.mask_cache_w = 0; cub->alien_w2_tex.mask_cache_h = 0;
    }
    if (textur_load(cub, &cub->alien_w3_tex, "textures/alien/w3.xpm") < 0)
    {
        cub->alien_w3_tex.img = NULL; cub->alien_w3_tex.data = NULL; cub->alien_w3_tex.w=0; cub->alien_w3_tex.h=0; cub->alien_w3_tex.bpp=0; cub->alien_w3_tex.sl=0; cub->alien_w3_tex.endian=0;
        cub->alien_w3_tex.mask_cache = NULL; cub->alien_w3_tex.mask_cache_w = 0; cub->alien_w3_tex.mask_cache_h = 0;
    }
    if (textur_load(cub, &cub->alien_attack_tex, "textures/alien/just_before_bondissement.xpm") < 0)
    {
        /* try original attaque name as fallback for legacy packs */
        if (textur_load(cub, &cub->alien_attack_tex, "textures/alien/attaque.xpm") < 0)
        {
            cub->alien_attack_tex.img = NULL; cub->alien_attack_tex.data = NULL; cub->alien_attack_tex.w=0; cub->alien_attack_tex.h=0; cub->alien_attack_tex.bpp=0; cub->alien_attack_tex.sl=0; cub->alien_attack_tex.endian=0;
            cub->alien_attack_tex.mask_cache = NULL; cub->alien_attack_tex.mask_cache_w = 0; cub->alien_attack_tex.mask_cache_h = 0;
        }
    }
    if (textur_load(cub, &cub->alien_bond_tex, "textures/alien/bondissement.xpm") < 0)
    {
        cub->alien_bond_tex.img = NULL; cub->alien_bond_tex.data = NULL; cub->alien_bond_tex.w=0; cub->alien_bond_tex.h=0; cub->alien_bond_tex.bpp=0; cub->alien_bond_tex.sl=0; cub->alien_bond_tex.endian=0;
        cub->alien_bond_tex.mask_cache = NULL; cub->alien_bond_tex.mask_cache_w = 0; cub->alien_bond_tex.mask_cache_h = 0;
    }
    /* new attack frames (sequence after pounce) */
    if (textur_load(cub, &cub->alien_attack1_tex, "textures/alien/attaque1.xpm") < 0)
    {
        cub->alien_attack1_tex.img = NULL; cub->alien_attack1_tex.data = NULL; cub->alien_attack1_tex.w=0; cub->alien_attack1_tex.h=0; cub->alien_attack1_tex.bpp=0; cub->alien_attack1_tex.sl=0; cub->alien_attack1_tex.endian=0;
        cub->alien_attack1_tex.mask_cache = NULL; cub->alien_attack1_tex.mask_cache_w = 0; cub->alien_attack1_tex.mask_cache_h = 0;
    }
    if (textur_load(cub, &cub->alien_attack2_tex, "textures/alien/attaque2.xpm") < 0)
    {
        cub->alien_attack2_tex.img = NULL; cub->alien_attack2_tex.data = NULL; cub->alien_attack2_tex.w=0; cub->alien_attack2_tex.h=0; cub->alien_attack2_tex.bpp=0; cub->alien_attack2_tex.sl=0; cub->alien_attack2_tex.endian=0;
        cub->alien_attack2_tex.mask_cache = NULL; cub->alien_attack2_tex.mask_cache_w = 0; cub->alien_attack2_tex.mask_cache_h = 0;
    }
    /* profile views for side-facing display */
    if (textur_load(cub, &cub->alien_look_right_tex, "textures/alien/ok_right.xpm") < 0)
    {
        if (textur_load(cub, &cub->alien_look_right_tex, "textures/alien/look_right.xpm") < 0)
        {
            cub->alien_look_right_tex.img = NULL; cub->alien_look_right_tex.data = NULL; cub->alien_look_right_tex.w=0; cub->alien_look_right_tex.h=0; cub->alien_look_right_tex.bpp=0; cub->alien_look_right_tex.sl=0; cub->alien_look_right_tex.endian=0;
            cub->alien_look_right_tex.mask_cache = NULL; cub->alien_look_right_tex.mask_cache_w = 0; cub->alien_look_right_tex.mask_cache_h = 0;
        }
    }
    if (textur_load(cub, &cub->alien_look_left_tex, "textures/alien/look_left.xpm") < 0)
    {
        cub->alien_look_left_tex.img = NULL; cub->alien_look_left_tex.data = NULL; cub->alien_look_left_tex.w=0; cub->alien_look_left_tex.h=0; cub->alien_look_left_tex.bpp=0; cub->alien_look_left_tex.sl=0; cub->alien_look_left_tex.endian=0;
        cub->alien_look_left_tex.mask_cache = NULL; cub->alien_look_left_tex.mask_cache_w = 0; cub->alien_look_left_tex.mask_cache_h = 0;
    }
    /* view_mi_face textures (mid-range frontal views with animation) */
    if (textur_load(cub, &cub->view_mi_face_left1, "textures/alien/view_mi_face_left1.xpm") < 0)
    {
        cub->view_mi_face_left1.img = NULL; cub->view_mi_face_left1.data = NULL; cub->view_mi_face_left1.w=0; cub->view_mi_face_left1.h=0; cub->view_mi_face_left1.bpp=0; cub->view_mi_face_left1.sl=0; cub->view_mi_face_left1.endian=0;
        cub->view_mi_face_left1.mask_cache = NULL; cub->view_mi_face_left1.mask_cache_w = 0; cub->view_mi_face_left1.mask_cache_h = 0;
    }
    if (textur_load(cub, &cub->view_mi_face_left2, "textures/alien/view_mi_face_left2.xpm") < 0)
    {
        cub->view_mi_face_left2.img = NULL; cub->view_mi_face_left2.data = NULL; cub->view_mi_face_left2.w=0; cub->view_mi_face_left2.h=0; cub->view_mi_face_left2.bpp=0; cub->view_mi_face_left2.sl=0; cub->view_mi_face_left2.endian=0;
        cub->view_mi_face_left2.mask_cache = NULL; cub->view_mi_face_left2.mask_cache_w = 0; cub->view_mi_face_left2.mask_cache_h = 0;
    }
    if (textur_load(cub, &cub->view_mi_face_right1, "textures/alien/view_mi_face_right1.xpm") < 0)
    {
        cub->view_mi_face_right1.img = NULL; cub->view_mi_face_right1.data = NULL; cub->view_mi_face_right1.w=0; cub->view_mi_face_right1.h=0; cub->view_mi_face_right1.bpp=0; cub->view_mi_face_right1.sl=0; cub->view_mi_face_right1.endian=0;
        cub->view_mi_face_right1.mask_cache = NULL; cub->view_mi_face_right1.mask_cache_w = 0; cub->view_mi_face_right1.mask_cache_h = 0;
    }
    if (textur_load(cub, &cub->view_mi_face_right2, "textures/alien/view_mi_face_right2.xpm") < 0)
    {
        cub->view_mi_face_right2.img = NULL; cub->view_mi_face_right2.data = NULL; cub->view_mi_face_right2.w=0; cub->view_mi_face_right2.h=0; cub->view_mi_face_right2.bpp=0; cub->view_mi_face_right2.sl=0; cub->view_mi_face_right2.endian=0;
        cub->view_mi_face_right2.mask_cache = NULL; cub->view_mi_face_right2.mask_cache_w = 0; cub->view_mi_face_right2.mask_cache_h = 0;
    }
    /* minimap inner texture (optional) */
    if (textur_load(cub, &cub->minimap_tex, "textures/tiles/gradient.xpm") < 0)
    {
        cub->minimap_tex.img = NULL;
        cub->minimap_tex.data = NULL;
        cub->minimap_tex.w = 0;
        cub->minimap_tex.h = 0;
        cub->minimap_tex.bpp = 0;
        cub->minimap_tex.sl = 0;
        cub->minimap_tex.endian = 0;
    }
    /* minimap wall tile texture (optional) */
    if (textur_load(cub, &cub->minimap_wall_tex, "textures/tiles/mini_wall_map.xpm") < 0)
    {
        cub->minimap_wall_tex.img = NULL;
        cub->minimap_wall_tex.data = NULL;
        cub->minimap_wall_tex.w = 0;
        cub->minimap_wall_tex.h = 0;
        cub->minimap_wall_tex.bpp = 0;
        cub->minimap_wall_tex.sl = 0;
        cub->minimap_wall_tex.endian = 0;
    }
    if (textur_load(cub, &cub->minimap_player_tex, "textures/HUD/soldier_top.xpm") < 0)
    {
        cub->minimap_player_tex.img = NULL;
        cub->minimap_player_tex.data = NULL;
        cub->minimap_player_tex.w = 0;
        cub->minimap_player_tex.h = 0;
        cub->minimap_player_tex.bpp = 0;
        cub->minimap_player_tex.sl = 0;
        cub->minimap_player_tex.endian = 0;
    }

    /* Load weapon textures */
    /* Hammer frames */
    if (textur_load(cub, &cub->hammer[0], "textures/gun/hammer/hammer_1.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->hammer[1], "textures/gun/hammer/hammer_2.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->hammer[2], "textures/gun/hammer/hammer_3.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->hammer[3], "textures/gun/hammer/hammer_4.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->hammer[4], "textures/gun/hammer/hammer_5.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->hammer[5], "textures/gun/hammer/hammer_6.xpm") < 0)
        return (-1);
    for (int i = 0; i < HAMMER_SPARK_FRAMES; ++i)
    {
        char path[128];

        snprintf(path, sizeof(path),
            "textures/gun/hammer/spark/spark%d.xpm", i + 1);
        if (textur_load(cub, &cub->hammer_spark_tex[i], path) < 0)
            return (-1);
    }

    /* MP textures */
    if (textur_load(cub, &cub->mp_idle, "textures/gun/mp/mp1.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->mp_aim, "textures/gun/mp/mp2.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->mp_fire1, "textures/gun/mp/fire_mp1.xpm") < 0)
        return (-1);
    if (textur_load(cub, &cub->mp_fire2, "textures/gun/mp/fire_mp2.xpm") < 0)
        return (-1);

    /* Blood effect textures (optionnelles) - blood_alien1.xpm à blood_alien5.xpm */
    if (textur_load(cub, &cub->blood_alien[0], "textures/alien/alien_blood/blood_alien1.xpm") < 0)
    {
        /* Initialiser TOUS les éléments à NULL pour éviter les crashes */
        for (int i = 0; i < 6; i++)
        {
            cub->blood_alien[i].img = NULL;
            cub->blood_alien[i].data = NULL;
            cub->blood_alien[i].w = 0;
            cub->blood_alien[i].h = 0;
        }
    }
    else
    {
        /* Charger le reste des frames (2 à 5) si la première existe */
        if (textur_load(cub, &cub->blood_alien[1], "textures/alien/alien_blood/blood_alien2.xpm") < 0 ||
            textur_load(cub, &cub->blood_alien[2], "textures/alien/alien_blood/blood_alien3.xpm") < 0 ||
            textur_load(cub, &cub->blood_alien[3], "textures/alien/alien_blood/blood_alien4.xpm") < 0 ||
            textur_load(cub, &cub->blood_alien[4], "textures/alien/alien_blood/blood_alien5.xpm") < 0)
        {
            /* Si chargement incomplet, tout mettre à NULL */
            for (int i = 0; i < 6; i++)
            {
                if (cub->blood_alien[i].img)
                    mlx_destroy_image(cub->mlx, cub->blood_alien[i].img);
                cub->blood_alien[i].img = NULL;
                cub->blood_alien[i].data = NULL;
                cub->blood_alien[i].w = 0;
                cub->blood_alien[i].h = 0;
            }
        }
        else
        {
            /* Initialiser le 6ème élément non utilisé */
            cub->blood_alien[5].img = NULL;
            cub->blood_alien[5].data = NULL;
            cub->blood_alien[5].w = 0;
            cub->blood_alien[5].h = 0;
        }
    }

    return (0);
}
