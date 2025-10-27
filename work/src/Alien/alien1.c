/* ************************************************************************** */
/*                                                                            */
/*   alien1.c - Rendering & Texture management                                */
/*                                                                            */
/* ************************************************************************** */

#include "alien_internal.h"

/* Helper function to put pixel (from drawing.c) */
extern void put_pixel(int x, int y, int color, t_cub *cub);

void detect_alien_mask_mode(t_cub *cub)
{
    if (!cub || !cub->alien_tex.data) { g_alien_detected_mode = MASK_MAGENTA; g_alien_mask_detected = 1; return; }
    t_tur *t = &cub->alien_tex;
    int bytes = t->bpp / 8;
    int cx = t->w / 2, cy = t->h / 2;
    if (t->w <= 0 || t->h <= 0 || bytes <= 0) { g_alien_detected_mode = MASK_MAGENTA; g_alien_mask_detected = 1; return; }
    int idx = cy * t->sl + cx * bytes;
    unsigned char *p = (unsigned char *)t->data + idx;
    unsigned char b = p[0], g = (bytes > 1) ? p[1] : 0, r = (bytes > 2) ? p[2] : 0, a = (bytes > 3) ? p[3] : 255;
    if (bytes >= 4 && a < 250) { g_alien_detected_mode = MASK_ALPHA; }
    else if ((int)r > 200 && (int)b > 200 && (int)g < 50) { g_alien_detected_mode = MASK_MAGENTA; }
    else if ((int)r < 40 && (int)g < 40 && (int)b < 40) { g_alien_detected_mode = MASK_BLACK; }
    else { g_alien_detected_mode = MASK_MAGENTA; }
    g_alien_mask_detected = 1;
}

/* Build a simple 1-byte mask per texel: 1 = opaque, 0 = transparent. Cached on texture. */
void build_alien_mask_map_for_tex(t_cub *cub __attribute__((unused)), t_tur *tex)
{
    if (!tex || !tex->data) return;
    int w = tex->w, h = tex->h, bytes = tex->bpp / 8;
    if (w <= 0 || h <= 0 || bytes <= 0) return;
    if (tex->mask_cache && tex->mask_cache_w == w && tex->mask_cache_h == h) return;
    if (tex->mask_cache) { free(tex->mask_cache); tex->mask_cache = NULL; }
    unsigned char *mask = malloc((size_t)w * (size_t)h);
    if (!mask) { tex->mask_cache = NULL; tex->mask_cache_w = tex->mask_cache_h = 0; return; }
    /* decide base mode: if AUTO, detect per-texture heuristically using corner sampling
     * (center or alpha can be misleading for XPM/MLX where alpha may be zero). */
    t_mask_mode mode = g_alien_mask_mode;
    int bg_r = 0, bg_g = 0, bg_b = 0;
    if (mode == MASK_AUTO)
    {
        /* sample 4 small corner pixels and average them to get background color */
        int samples = 0;
        int corners[4][2] = { {1,1}, { (w>2)?w-2:0, 1 }, {1, (h>2)?h-2:0 }, { (w>2)?w-2:0, (h>2)?h-2:0 } };
        long sr = 0, sg = 0, sb = 0;
        for (int ci = 0; ci < 4; ++ci)
        {
            int cx = corners[ci][0];
            int cy = corners[ci][1];
            if (cx < 0 || cy < 0 || cx >= w || cy >= h) continue;
            int off = cy * tex->sl + cx * bytes;
            unsigned char *pc = (unsigned char *)tex->data + off;
            unsigned char bc = pc[0]; unsigned char gc = (bytes > 1) ? pc[1] : 0; unsigned char rc = (bytes > 2) ? pc[2] : 0;
            sr += rc; sg += gc; sb += bc; samples++;
        }
        if (samples > 0) { bg_r = (int)(sr / samples); bg_g = (int)(sg / samples); bg_b = (int)(sb / samples); }
        else { bg_r = bg_g = bg_b = 0; }
        /* choose magenta if corner average is magenta-like */
        if (bg_r > 200 && bg_b > 200 && bg_g < 100) mode = MASK_MAGENTA;
        else if (bg_r < 40 && bg_g < 40 && bg_b < 40) mode = MASK_BLACK;
        else mode = MASK_AUTO; /* use color distance to bg below */
    }
    const int MAGENTA_TH2 = 2200; /* squared threshold for approximate magenta */
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            int off = y * tex->sl + x * bytes;
            unsigned char *px = (unsigned char *)tex->data + off;
            unsigned char b = px[0]; unsigned char g = (bytes > 1) ? px[1] : 0; unsigned char r = (bytes > 2) ? px[2] : 0; unsigned char a = (bytes > 3) ? px[3] : 255;
            int opaque = 1;
            if (mode == MASK_ALPHA)
            {
                if (bytes < 4 || a == 0) opaque = 0;
            }
            else if (mode == MASK_MAGENTA)
            {
                int dr = (int)r - 255; int dg = (int)g - 0; int db = (int)b - 255;
                if (dr * dr + dg * dg + db * db <= MAGENTA_TH2) opaque = 0;
            }
            else if (mode == MASK_BLACK)
            {
                if ((int)r < 50 && (int)g < 50 && (int)b < 50) opaque = 0;
            }
            else /* fallback: compare distance to corner-averaged bg color */
            {
                int dr = (int)r - bg_r; int dg = (int)g - bg_g; int db = (int)b - bg_b;
                int dist2 = dr * dr + dg * dg + db * db;
                const int BG_TH2 = 1800; /* tuned threshold */
                if (dist2 <= BG_TH2) opaque = 0;
            }
            mask[y * w + x] = opaque ? 1 : 0;
        }
    }
    tex->mask_cache = mask;
    tex->mask_cache_w = w;
    tex->mask_cache_h = h;

    /* Post-process: remove small magenta fringes by flood-filling transparent areas from corners
     * If mask indicates opaque in corners that are magenta, we convert nearby magenta pixels to transparent.
     * Simple approach: from each corner, BFS through pixels whose chroma is close to magenta and mark transparent.
     */
    /* small BFS stack allocated conservatively */
    int maxq = w * h;
    int *qx = malloc(sizeof(int) * maxq);
    int *qy = malloc(sizeof(int) * maxq);
    if (!qx || !qy) { free(qx); free(qy); return; }
    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (int corner = 0; corner < 4; ++corner)
    {
        int sx = (corner & 1) ? (w - 2) : 1;
        int sy = (corner & 2) ? (h - 2) : 1;
        int qh = 0, qt = 0;
        qx[qh] = sx; qy[qh] = sy; qh++;
        while (qt < qh)
        {
            int cx = qx[qt]; int cy = qy[qt]; qt++;
            if (cx < 0 || cy < 0 || cx >= w || cy >= h) continue;
            if (mask[cy * w + cx] == 0) continue; /* already transparent */
            int off = cy * tex->sl + cx * bytes;
            unsigned char *pc = (unsigned char *)tex->data + off;
            unsigned char b = pc[0], g = (bytes > 1) ? pc[1] : 0, r = (bytes > 2) ? pc[2] : 0;
            int drm = (int)r - 255; int dbm = (int)b - 255; int dg = (int)g - 0;
            int magenta_dist2 = drm*drm + dg*dg + dbm*dbm;
            if (magenta_dist2 <= 3000)
            {
                /* mark transparent and propagate */
                mask[cy * w + cx] = 0;
                for (int d = 0; d < 4; ++d)
                {
                    int nx = cx + dirs[d][0]; int ny = cy + dirs[d][1];
                    if (nx >= 0 && ny >= 0 && nx < w && ny < h && mask[ny * w + nx] != 0)
                    {
                        qx[qh] = nx; qy[qh] = ny; qh++;
                        if (qh >= maxq) break;
                    }
                }
            }
        }
    }
    free(qx); free(qy);
}

void alien_trigger_pounce(t_cub *cub)
{
    if (!cub || !cub->alien) return;
    t_alien *a = cub->alien;
    /* enter stalking mode first: show crouch-walk frames (w1/w2/w3) while stalking */
    a->state = ALIEN_STALKING;
    a->is_stalking = 1;
    a->stalk_timer = 1.2; /* stalk for 1.2s before preparing */
    alien_reset_path(a);
    /* compute pounce direction towards player and set speed */
    if (cub->player && cub->player->position)
        alien_set_pounce_target(a, cub->player->position->x, cub->player->position->y);
    else
        alien_set_pounce_target(a, a->pos->x + a->dir->x * BLOCK, a->pos->y + a->dir->y * BLOCK);
    a->pounce_speed = 300.0; /* pixels per second */
    a->pounce_timer = 0.0;
    /* set attack animation timer for when lunging */
    a->is_attacking = 0;
    a->attack_anim_timer = 0.0;
    a->chase_runup_timer = 0.0;
}

/* helper: build mask and report transparent pixel count for debugging */
void build_and_report(t_cub *cub, t_tur *tex, const char *name)
{
    (void)name; /* Unused parameter - debug function disabled */
    if (!tex) return;
    if (!tex->data)
    {
        return;
    }
    build_alien_mask_map_for_tex(cub, tex);
    if (!tex->mask_cache)
    {
        return;
    }
    int w = tex->mask_cache_w, h = tex->mask_cache_h;
    long total = (long)w * (long)h;
    long trans = 0;
    for (long i = 0; i < total; ++i) if (tex->mask_cache[i] == 0) ++trans;
    /* Debug output disabled for performance */
}

/* draw a sprite texture projected on screen; uses tex->mask_cache if present */
void draw_alien_sprite_texture_with(t_cub *cub, t_tur *tex, int dx0, int dx1, int dy0, int dy1, double transformY)
{
    if (!cub || !tex || !tex->data) return;
    int w = tex->w, h = tex->h, bytes = tex->bpp / 8;
    if (w <= 0 || h <= 0 || bytes <= 0) return;
    /* ensure mask exists */
    if (!tex->mask_cache) build_alien_mask_map_for_tex(cub, tex);
    int spanx = dx1 - dx0 + 1;
    int spany = dy1 - dy0 + 1;
    for (int sx = dx0; sx <= dx1; ++sx)
    {
        if (sx < 0 || sx >= WIDTH) continue;
        double z = cub->zbuffer[sx];
        if (transformY >= z - 1e-6) continue; /* occluded by wall */
        double tx = (double)(sx - dx0) / (double)spanx;
        int tex_x = (int)(tx * w);
    if (tex_x < 0) tex_x = 0;
    if (tex_x >= w) tex_x = w - 1;
        for (int sy = dy0; sy <= dy1; ++sy)
        {
            if (sy < 0 || sy >= HEIGHT) continue;
            double ty = (double)(sy - dy0) / (double)spany;
            int tex_y = (int)(ty * h);
            if (tex_y < 0) tex_y = 0;
            if (tex_y >= h) tex_y = h - 1;
            if (tex->mask_cache && tex->mask_cache_w == w && tex->mask_cache_h == h && tex->mask_cache[tex_y * w + tex_x] == 0) continue;
            int tidx = tex_y * tex->sl + tex_x * bytes;
            int color = *(int *)(tex->data + tidx);
            put_pixel(sx, sy, color, cub);
        }
    }
}

/* Top-level sprite drawing: project alien and pick texture according to state/facing */
void draw_alien_sprite(t_cub *cub)
{
    if (!cub || !cub->alien || !cub->player) return;
    t_alien *a = cub->alien;
    if (!a->pos || !a->dir || !cub->player->position) return;
    
    /* compute sprite position in map units (same units as zbuffer/perp distances) */
    double spriteX = (a->pos->x - cub->player->position->x) / (double)BLOCK;
    double spriteY = (a->pos->y - cub->player->position->y) / (double)BLOCK;
    double dirX = cub->player->direction->x;
    double dirY = cub->player->direction->y;
    double planeX = cub->player->plane->x;
    double planeY = cub->player->plane->y;
    double invDet = 1.0 / (planeX * dirY - dirX * planeY);
    double transformX = invDet * (dirY * spriteX - dirX * spriteY);
    double transformY = invDet * (-planeY * spriteX + planeX * spriteY);
    if (transformY <= 0.0001) return; /* behind camera */
    int spriteScreenX = (int)((WIDTH / 2.0) * (1.0 + transformX / transformY));
    int spriteHeight = abs((int)(HEIGHT / transformY));
    if (spriteHeight <= 0) spriteHeight = 1;
    int drawStartY = -spriteHeight / 2 + HEIGHT / 2; if (drawStartY < 0) drawStartY = 0;
    int drawEndY = spriteHeight / 2 + HEIGHT / 2; if (drawEndY >= HEIGHT) drawEndY = HEIGHT - 1;
    int spriteWidth = spriteHeight;
    int drawStartX = -spriteWidth / 2 + spriteScreenX; if (drawStartX < 0) drawStartX = 0;
    int drawEndX = spriteWidth / 2 + spriteScreenX; if (drawEndX >= WIDTH) drawEndX = WIDTH - 1;

    double target_dx = cub->player->position->x - a->pos->x;
    double target_dy = cub->player->position->y - a->pos->y;
    double raw_target_dist = sqrt(target_dx * target_dx + target_dy * target_dy);
    double norm_dist = raw_target_dist;
    if (norm_dist < 1e-6) norm_dist = 1.0;
    double dir_to_player_x = target_dx / norm_dist;
    double dir_to_player_y = target_dy / norm_dist;
    double view_front = a->dir->x * dir_to_player_x + a->dir->y * dir_to_player_y;
    double view_side = a->dir->x * dir_to_player_y - a->dir->y * dir_to_player_x;
    double move_mag = sqrt(a->smooth_vx * a->smooth_vx + a->smooth_vy * a->smooth_vy);
    int moving = a->is_walking || move_mag > 0.1;
    
    /* Si l'alien est bloqué (stuck/stall > 0.5s), figer la texture sur view_mi_face */
    t_alien_view_bucket view_bucket;
    if (a->stuck_timer > 0.5 || a->stall_timer > 0.5)
    {
        /* Bloquer sur view_mi_face selon la position relative au joueur */
        double abs_side = fabs(view_side);
        
        /* Si majoritairement de côté, afficher MID */
        if (abs_side > 0.15 || (view_front >= 0.30 && view_front < 0.85))
        {
            view_bucket = (view_side > 0.0) ? ALIEN_VIEW_MID_LEFT : ALIEN_VIEW_MID_RIGHT;
        }
        /* Sinon front ou back selon l'orientation */
        else if (view_front >= 0.65)
            view_bucket = ALIEN_VIEW_FRONT;
        else if (view_front < -0.40)
            view_bucket = ALIEN_VIEW_BACK;
        else
            view_bucket = (view_side > 0.0) ? ALIEN_VIEW_MID_LEFT : ALIEN_VIEW_MID_RIGHT;
    }
    else
    {
        /* Comportement normal: sélection dynamique */
        view_bucket = alien_select_view_bucket(a, view_front, view_side, moving && !a->is_attacking && !a->has_attacked);
    }
    
    a->view_state = view_bucket;

    /* choose texture based on state, animations and facing */
    t_tur *tex = NULL;
    
    /* PRIORITÉ ABSOLUE: attaque1 / attaque2 pendant toute la séquence d'attaque sur le player */
    if (a->has_attacked && a->attack_anim_timer > 0.0)
    {
        /* Afficher attaque1 dans la première moitié, attaque2 dans la seconde */
        if (cub->alien_attack1_tex.img && cub->alien_attack1_tex.data && a->attack_anim_timer > ALIEN_ATTACK_PHASE_SPLIT)
            tex = &cub->alien_attack1_tex;
        else if (cub->alien_attack2_tex.img && cub->alien_attack2_tex.data)
            tex = &cub->alien_attack2_tex;
        
        /* FORCER: ne pas continuer si on a une texture d'attaque */
        if (tex)
            goto render_sprite; /* Sauter toute la logique normale */
    }
    
    /* Après une attaque réussie mais timer écoulé: afficher view_mi_face intelligemment */
    if (!tex && a->attack_anim_timer <= 0.0 && a->attack_cooldown > 0.0)
    {
        /* Calcul simplifié: utiliser position relative au joueur */
        int use_left = (view_side > 0.0);
        
        /* Déterminer si l'alien bouge */
        double move_speed = sqrt(a->smooth_vx * a->smooth_vx + a->smooth_vy * a->smooth_vy);
        int is_moving = (a->is_walking || move_speed > 0.08);
        
        /* Frame 1 = statique, Frame 2 = en mouvement */
        if (use_left)
        {
            t_tur *mid_tex = is_moving ? &cub->view_mi_face_left2 : &cub->view_mi_face_left1;
            if (mid_tex && mid_tex->img && mid_tex->data)
                tex = mid_tex;
        }
        else
        {
            t_tur *mid_tex = is_moving ? &cub->view_mi_face_right2 : &cub->view_mi_face_right1;
            if (mid_tex && mid_tex->img && mid_tex->data)
                tex = mid_tex;
        }
    }
    
    /* bondissement frames when leaping */
    if (!tex && a->is_attacking && cub->alien_bond_tex.img && cub->alien_bond_tex.data)
        tex = &cub->alien_bond_tex;
    /* preparation pose: stand up first, then crouch right before jumping */
    if (!tex && a->is_preparing)
    {
        if (cub->alien_tex.img && cub->alien_tex.data && a->prepare_timer > ALIEN_PREP_CROUCH_WINDOW)
            tex = &cub->alien_tex;
        else if (cub->alien_attack_tex.img && cub->alien_attack_tex.data)
            tex = &cub->alien_attack_tex;
    }
    /* stalking: flash just_before at entry, then crouch-walk frames */
    if (!tex && a->is_stalking)
    {
        if (a->stalk_timer > 0.8 && cub->alien_attack_tex.img && cub->alien_attack_tex.data)
            tex = &cub->alien_attack_tex;
    }

    if (!tex && !a->is_preparing && view_bucket == ALIEN_VIEW_WALK && !a->has_attacked && !a->is_attacking)
    {
        static const int crouch_pattern[4] = {0, 1, 2, 1};
        int seq = crouch_pattern[a->anim_frame & 3];
        t_tur *frame = (seq == 0) ? &cub->alien_w1_tex : (seq == 1) ? &cub->alien_w2_tex : &cub->alien_w3_tex;
        if (frame && frame->img && frame->data)
            tex = frame;
        else
            view_bucket = ALIEN_VIEW_FRONT;
    }

    if (!tex)
    {
        switch (view_bucket)
        {
            case ALIEN_VIEW_FRONT:
            case ALIEN_VIEW_WALK:
                tex = (cub->alien_tex.img && cub->alien_tex.data) ? &cub->alien_tex : NULL;
                break;
            case ALIEN_VIEW_MID_LEFT:
                /* Utiliser view_mi_face_left avec animation */
                if (a->is_walking || moving)
                {
                    int frame_idx = (a->anim_frame & 1);
                    t_tur *mid_tex = (frame_idx == 0) ? &cub->view_mi_face_left1 : &cub->view_mi_face_left2;
                    if (mid_tex && mid_tex->img && mid_tex->data)
                        tex = mid_tex;
                }
                else
                {
                    /* Statique: utiliser frame 1 */
                    if (cub->view_mi_face_left1.img && cub->view_mi_face_left1.data)
                        tex = &cub->view_mi_face_left1;
                }
                /* Fallback vers front si textures manquantes */
                if (!tex && cub->alien_tex.img && cub->alien_tex.data)
                    tex = &cub->alien_tex;
                break;
            case ALIEN_VIEW_MID_RIGHT:
                /* Utiliser view_mi_face_right avec animation */
                if (a->is_walking || moving)
                {
                    int frame_idx = (a->anim_frame & 1);
                    t_tur *mid_tex = (frame_idx == 0) ? &cub->view_mi_face_right1 : &cub->view_mi_face_right2;
                    if (mid_tex && mid_tex->img && mid_tex->data)
                        tex = mid_tex;
                }
                else
                {
                    /* Statique: utiliser frame 1 */
                    if (cub->view_mi_face_right1.img && cub->view_mi_face_right1.data)
                        tex = &cub->view_mi_face_right1;
                }
                /* Fallback vers front si textures manquantes */
                if (!tex && cub->alien_tex.img && cub->alien_tex.data)
                    tex = &cub->alien_tex;
                break;
            case ALIEN_VIEW_SIDE_LEFT:
                if (cub->alien_look_left_tex.img && cub->alien_look_left_tex.data)
                    tex = &cub->alien_look_left_tex;
                break;
            case ALIEN_VIEW_SIDE_RIGHT:
                if (cub->alien_look_right_tex.img && cub->alien_look_right_tex.data)
                    tex = &cub->alien_look_right_tex;
                break;
            case ALIEN_VIEW_BACK:
                if (cub->alien_back_tex.img && cub->alien_back_tex.data)
                    tex = &cub->alien_back_tex;
                break;
        }
        if (!tex && cub->alien_tex.img && cub->alien_tex.data)
            tex = &cub->alien_tex;
    }

    /* default front if nothing else picked */
    if (!tex)
        tex = &cub->alien_tex;
    
render_sprite: /* Label pour sauter la logique normale lors de l'attaque */
    if (tex && tex->img && tex->data)
    {
        /* Draw the alien sprite texture */
        draw_alien_sprite_texture_with(cub, tex, drawStartX, drawEndX, drawStartY, drawEndY, transformY);
    }
    
    /* EFFET DE SANG: Dessiner le sprite de sang par-dessus l'alien si touché */
    /* AMÉLIORATION: Réduire la taille de 40% (scale = 0.6) */
    if (a->show_blood && a->blood_frame >= 0 && a->blood_frame < 5)
    {
        t_tur *blood_tex = &cub->blood_alien[a->blood_frame];
        if (blood_tex && blood_tex->img && blood_tex->data)
        {
            /* Réduire la taille de 40% (multiplier par 0.6) */
            int blood_height = (int)(spriteHeight * 0.6);
            int blood_drawStartY = -blood_height / 2 + HEIGHT / 2;
            int blood_drawEndY = blood_height / 2 + HEIGHT / 2;
            
            int blood_width = (int)(blood_height * 0.6); /* Garder le ratio */
            int blood_drawStartX = -blood_width / 2 + spriteScreenX;
            int blood_drawEndX = blood_width / 2 + spriteScreenX;
            
            /* Clipping */
            if (blood_drawStartY < 0) blood_drawStartY = 0;
            if (blood_drawEndY >= HEIGHT) blood_drawEndY = HEIGHT - 1;
            if (blood_drawStartX < 0) blood_drawStartX = 0;
            if (blood_drawEndX >= WIDTH) blood_drawEndX = WIDTH - 1;
            
            /* Dessiner le sang à taille réduite avec transparence magenta */
            draw_alien_sprite_texture_with(cub, blood_tex, blood_drawStartX, blood_drawEndX, blood_drawStartY, blood_drawEndY, transformY);
        }
    }
}

static int alien_can_move(t_cub *cub, double x, double y) __attribute__((unused));
static int alien_can_move(t_cub *cub, double x, double y)
{
    if (!cub || !cub->map) return 0;
    int mx = (int)(x / BLOCK);
    int my = (int)(y / BLOCK);
    int map_h = 0; while (cub->map[map_h]) map_h++;
    if (mx < 0 || my < 0 || my >= map_h) return 0;
    if (!cub->map[my]) return 0;
    int rowlen = (int)strlen(cub->map[my]);
    if (mx >= rowlen) return 0;
    if (cub->map[my][mx] == '1' || cub->map[my][mx] == 'P'
        || cub->map[my][mx] == 'D' || cub->map[my][mx] == ' '
        || cub->map[my][mx] == 'B')
        return 0;
    return 1;
}

int line_of_sight(t_cub *cub, t_alien *a, double px,double py)
{
    if (!cub || !a || !a->pos || !cub->map) return 0;
    double dx = px - a->pos->x, dy = py - a->pos->y;
    double dist = sqrt(dx*dx+dy*dy);
    if (dist < 1e-6) return 1;
    int steps = (int)(dist / (double)(BLOCK/4)) + 1;
    for (int i=1;i<=steps;++i)
    {
        double t = (double)i/(double)steps; double sx = a->pos->x + dx*t; double sy = a->pos->y + dy*t;
        int mx = (int)(sx / BLOCK), my = (int)(sy / BLOCK);
        int map_h = 0; while (cub->map[map_h]) map_h++;
        if (mx<0 || my<0 || my>=map_h) return 0;
        if (!cub->map[my] || mx >= (int)strlen(cub->map[my])) return 0;
        if (cub->map[my][mx]=='1' || cub->map[my][mx]=='P'
            || cub->map[my][mx]=='D' || cub->map[my][mx]==' '
            || cub->map[my][mx]=='B')
            return 0;
    }
    return 1;
}
