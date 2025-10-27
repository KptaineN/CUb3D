/* ************************************************************************** */
/*                                                                            */
/* Clean single implementation for alien sprite handling */
#include "../../includes/cub3d.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Minimal global state used by this module */
static int alien_debug_show_raw __attribute__((unused)) = 0;
typedef enum e_mask_mode { MASK_AUTO = 0, MASK_MAGENTA = 1, MASK_BLACK = 2, MASK_ALPHA = 3 } t_mask_mode;
static t_mask_mode alien_mask_mode __attribute__((unused)) = MASK_AUTO;
static int alien_mask_detected __attribute__((unused)) = 0;
static t_mask_mode alien_detected_mode __attribute__((unused)) = MASK_AUTO;
static int alien_bg_r __attribute__((unused)) = 0, alien_bg_g __attribute__((unused)) = 0, alien_bg_b __attribute__((unused)) = 0;
static int alien_replace_bg_color __attribute__((unused)) = -1;

/* Timing configuration for pre-attack staging */
#define ALIEN_PREP_TOTAL_TIME 0.65
#define ALIEN_PREP_CROUCH_WINDOW 0.22
#define ALIEN_POUNCE_MAX_DURATION 0.85
#define ALIEN_ATTACK_SEQUENCE_DURATION 1.0
#define ALIEN_ATTACK_PHASE_SPLIT 0.5

typedef enum e_alien_view_bucket
{
    ALIEN_VIEW_FRONT = 0,
    ALIEN_VIEW_WALK,
    ALIEN_VIEW_MID_LEFT,
    ALIEN_VIEW_MID_RIGHT,
    ALIEN_VIEW_SIDE_LEFT,
    ALIEN_VIEW_SIDE_RIGHT,
    ALIEN_VIEW_BACK
}   t_alien_view_bucket;

static void alien_set_pounce_target(t_alien *a, double tx, double ty);
static void alien_finish_attack(t_cub *cub, t_alien *a, int hit_player);

/* Forward declarations for helpers used in public functions. Implemented as lightweight
 * versions to keep compilation and basic runtime stable. Full-featured implementations
 * can be restored/expanded later. */
static void detect_alien_mask_mode(t_cub *cub) __attribute__((unused));
static void build_alien_mask_map_for_tex(t_cub *cub, t_tur *tex) __attribute__((unused));
void draw_alien_sprite(t_cub *cub);
static void alien_reset_path(t_alien *a);
static void alien_choose_patrol_target(t_cub *cub, t_alien *a);
static void alien_plan_retreat(t_cub *cub, t_alien *a);
static void alien_force_unstuck(t_cub *cub, t_alien *a);
static t_alien_view_bucket alien_select_view_bucket(t_alien *a, double view_front, double view_side, int moving);
static void alien_apply_player_damage(t_cub *cub);
static void alien_prepare_direct_pounce(t_cub *cub, t_alien *a, double target_x, double target_y);
static void alien_enter_search(t_cub *cub, t_alien *a, double center_x, double center_y);

/* Minimal implementations to keep compilation and runtime stable. */
/* Heuristic to detect mask mode for a texture (alpha / magenta / black) */
static void detect_alien_mask_mode(t_cub *cub)
{
    if (!cub || !cub->alien_tex.data) { alien_detected_mode = MASK_MAGENTA; alien_mask_detected = 1; return; }
    t_tur *t = &cub->alien_tex;
    int bytes = t->bpp / 8;
    int cx = t->w / 2, cy = t->h / 2;
    if (t->w <= 0 || t->h <= 0 || bytes <= 0) { alien_detected_mode = MASK_MAGENTA; alien_mask_detected = 1; return; }
    int idx = cy * t->sl + cx * bytes;
    unsigned char *p = (unsigned char *)t->data + idx;
    unsigned char b = p[0], g = (bytes > 1) ? p[1] : 0, r = (bytes > 2) ? p[2] : 0, a = (bytes > 3) ? p[3] : 255;
    if (bytes >= 4 && a < 250) { alien_detected_mode = MASK_ALPHA; }
    else if ((int)r > 200 && (int)b > 200 && (int)g < 50) { alien_detected_mode = MASK_MAGENTA; }
    else if ((int)r < 40 && (int)g < 40 && (int)b < 40) { alien_detected_mode = MASK_BLACK; }
    else { alien_detected_mode = MASK_MAGENTA; }
    alien_mask_detected = 1;
}

/* Build a simple 1-byte mask per texel: 1 = opaque, 0 = transparent. Cached on texture. */
static void build_alien_mask_map_for_tex(t_cub *cub __attribute__((unused)), t_tur *tex)
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
    t_mask_mode mode = alien_mask_mode;
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

static void alien_set_pounce_target(t_alien *a, double tx, double ty)
{
    if (!a || !a->pos || !a->dir)
        return;
    a->pounce_target_x = tx;
    a->pounce_target_y = ty;
    double dx = tx - a->pos->x;
    double dy = ty - a->pos->y;
    double len = sqrt(dx * dx + dy * dy);
    if (len < 1e-6)
    {
        /* fallback on current facing direction when target is extremely close */
        dx = a->dir->x;
        dy = a->dir->y;
        len = sqrt(dx * dx + dy * dy);
        if (len < 1e-6)
            len = 1.0;
    }
    a->pounce_vx = dx / len;
    a->pounce_vy = dy / len;
}

static void alien_finish_attack(t_cub *cub, t_alien *a, int hit_player)
{
    if (!a)
        return;
    a->is_attacking = 0;
    a->pounce_timer = 0.0;
    if (hit_player)
    {
        a->has_attacked = 1;
        a->attack_anim_timer = ALIEN_ATTACK_SEQUENCE_DURATION;
        if (a->attack_cooldown < 4.5)
            a->attack_cooldown = 4.5;
        alien_apply_player_damage(cub);
    }
    else
    {
        a->has_attacked = 0;
        a->attack_anim_timer = 0.0;
        if (a->attack_cooldown < 2.5)
            a->attack_cooldown = 2.5;
    }
    alien_plan_retreat(cub, a);
    a->state = ALIEN_SEARCH;
    a->state_timer = 3.0;
    alien_reset_path(a);
}
/* Trigger a pounce (bondissement) attack for manual testing */
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
static void build_and_report(t_cub *cub, t_tur *tex, const char *name)
{
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
    /* print center pixel RGBA for diagnosis if data available */
    if (tex->data && tex->bpp/8 >= 3)
    {
        int cx = tex->w/2, cy = tex->h/2;
        int offc = cy * tex->sl + cx * (tex->bpp/8);
        unsigned char *pc = (unsigned char *)tex->data + offc;
        unsigned int bc = pc[0]; unsigned int gc = (tex->bpp/8>1)?pc[1]:0; unsigned int rc = (tex->bpp/8>2)?pc[2]:0; unsigned int ac = (tex->bpp/8>3)?pc[3]:255;
    }
}

/* draw a sprite texture projected on screen; uses tex->mask_cache if present */
static void draw_alien_sprite_texture_with(t_cub *cub, t_tur *tex, int dx0, int dx1, int dy0, int dy1, double transformY)
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
    /* debug: print player and alien positions once occasionally */
    static int dbgcnt = 0;
    if ((dbgcnt++ & 127) == 0)
    {
            cub->player->position->x, cub->player->position->y, cub->player->angle, cub->player->cos_angle, cub->player->sin_angle);
    }
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
    t_alien_view_bucket view_bucket = alien_select_view_bucket(a, view_front, view_side, moving && !a->is_attacking && !a->has_attacked);
    a->view_state = view_bucket;

    const double contact_dist = BLOCK * 0.55;

    /* choose texture based on state, animations and facing */
    t_tur *tex = NULL;
    /* attaque1 / attaque2 only when contact is established */
    if (a->has_attacked && a->attack_anim_timer > 0.0 && raw_target_dist <= contact_dist + BLOCK * 0.1)
    {
        if (cub->alien_attack1_tex.img && cub->alien_attack1_tex.data && a->attack_anim_timer > ALIEN_ATTACK_PHASE_SPLIT)
            tex = &cub->alien_attack1_tex;
        else if (cub->alien_attack2_tex.img && cub->alien_attack2_tex.data)
            tex = &cub->alien_attack2_tex;
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
                /* fallback to front for now (optional mid-face textures not loaded) */
                tex = (cub->alien_tex.img && cub->alien_tex.data) ? &cub->alien_tex : NULL;
                break;
            case ALIEN_VIEW_MID_RIGHT:
                /* fallback to front for now (optional mid-face textures not loaded) */
                tex = (cub->alien_tex.img && cub->alien_tex.data) ? &cub->alien_tex : NULL;
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
    if (tex && tex->img && tex->data)
    {
        static int dbg_count = 0;
        if (dbg_count < 6)
        {
            dbg_count++;
        }
        draw_alien_sprite_texture_with(cub, tex, drawStartX, drawEndX, drawStartY, drawEndY, transformY);
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
        || cub->map[my][mx] == 'D' || cub->map[my][mx] == ' ')
        return 0;
    return 1;
}

static int line_of_sight(t_cub *cub, t_alien *a, double px,double py) __attribute__((unused));
static int line_of_sight(t_cub *cub, t_alien *a, double px,double py)
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
            || cub->map[my][mx]=='D' || cub->map[my][mx]==' ')
            return 0;
    }
    return 1;
}

static void alien_reset_path(t_alien *a)
{
    if (!a) return;
    a->path_length = 0;
    a->path_index = 0;
    a->path_goal_mx = -1;
    a->path_goal_my = -1;
    a->path_recalc_timer = 0.0;
}

static void alien_mark_wall_hit(t_alien *a, int mx, int my)
{
    if (!a) return;
    if (mx == a->wall_hit_mx && my == a->wall_hit_my)
    {
        a->consecutive_hits++;
        return;
    }
    a->wall_hit_prev_mx = a->wall_hit_mx;
    a->wall_hit_prev_my = a->wall_hit_my;
    a->wall_hit_mx = mx;
    a->wall_hit_my = my;
    a->consecutive_hits = 1;
}

static int is_blocking_cell(t_cub *cub, int mx, int my)
{
    if (!cub || !cub->map) return 1;
    if (mx < 0 || my < 0) return 1;
    int map_h = 0;
    while (cub->map[map_h]) map_h++;
    if (my >= map_h) return 1;
    char *row = cub->map[my];
    if (!row) return 1;
    int rowlen = (int)strlen(row);
    if (mx >= rowlen) return 1;
    char c = row[mx];
    if (c == '1' || c == 'P' || c == 'D' || c == ' ')
        return 1;
    return 0;
}

static int alien_position_clear(t_cub *cub, double x, double y, double radius)
{
    static const double offsets[8][2] = {
        {0.0, 0.0}, {1.0, 0.0}, {-1.0, 0.0}, {0.0, 1.0},
        {0.0, -1.0}, {0.7071, 0.7071}, {-0.7071, 0.7071},
        {0.7071, -0.7071}
    };
    if (!cub) return 0;
    for (int i = 0; i < 8; ++i)
    {
        double sx = x + offsets[i][0] * radius;
        double sy = y + offsets[i][1] * radius;
        int mx = (int)(sx / BLOCK);
        int my = (int)(sy / BLOCK);
        if (is_blocking_cell(cub, mx, my))
            return 0;
    }
    return 1;
}

static void alien_plan_retreat(t_cub *cub, t_alien *a)
{
    if (!a)
        return;
    double dirx = -a->pounce_vx;
    double diry = -a->pounce_vy;
    double norm = sqrt(dirx * dirx + diry * diry);
    if (norm < 1e-6)
    {
        dirx = -a->dir->x;
        diry = -a->dir->y;
        norm = sqrt(dirx * dirx + diry * diry);
    }
    if (norm < 1e-6)
    {
        dirx = 0.0;
        diry = -1.0;
        norm = 1.0;
    }
    dirx /= norm;
    diry /= norm;
    double base_x = a->pos->x;
    double base_y = a->pos->y;
    double chosen_x = base_x;
    double chosen_y = base_y;
    double distances[4] = { BLOCK * 4.0, BLOCK * 3.0, BLOCK * 2.0, BLOCK * 1.4 };
    for (int i = 0; i < 4; ++i)
    {
        double tx = base_x + dirx * distances[i];
        double ty = base_y + diry * distances[i];
        if (!cub || alien_position_clear(cub, tx, ty, a->hitbox_radius * 0.8))
        {
            chosen_x = tx;
            chosen_y = ty;
            break;
        }
    }
    a->retreat_x = chosen_x;
    a->retreat_y = chosen_y;
    a->target_x = chosen_x;
    a->target_y = chosen_y;
    a->path_recalc_timer = 0.0;
}

static void alien_force_unstuck(t_cub *cub, t_alien *a)
{
    if (!cub || !a || !cub->player || !cub->player->position)
    {
        alien_choose_patrol_target(cub, a);
        return;
    }
    double px = cub->player->position->x;
    double py = cub->player->position->y;
    double dx = px - a->pos->x;
    double dy = py - a->pos->y;
    double len = sqrt(dx * dx + dy * dy);
    double dirx = (len > 1e-6) ? dx / len : a->dir->x;
    double diry = (len > 1e-6) ? dy / len : a->dir->y;
    double fallback_x = a->pos->x;
    double fallback_y = a->pos->y;
    double distances[4] = { BLOCK * 6.0, BLOCK * 5.0, BLOCK * 4.0, BLOCK * 2.5 };
    int found = 0;
    for (int i = 0; i < 4; ++i)
    {
        double tx = px - dirx * distances[i];
        double ty = py - diry * distances[i];
        if (alien_position_clear(cub, tx, ty, a->hitbox_radius * 0.8))
        {
            fallback_x = tx;
            fallback_y = ty;
            found = 1;
            break;
        }
    }
    if (!found)
        alien_choose_patrol_target(cub, a);
    else
    {
        a->target_x = fallback_x;
        a->target_y = fallback_y;
    }
    a->state = ALIEN_SEARCH;
    a->state_timer = 3.5;
    alien_reset_path(a);
    a->path_recalc_timer = 0.0;
    a->stall_cell_mx = -1;
    a->stall_cell_my = -1;
    a->stall_timer = 0.0;
}

static t_alien_view_bucket alien_select_view_bucket(t_alien *a, double view_front, double view_side, int moving)
{
    const double FRONT_ENTER = 0.78;
    const double FRONT_EXIT = 0.70;
    const double WALK_ENTER = 0.60;
    const double WALK_EXIT = 0.52;
    const double MID_ENTER = 0.38;
    const double MID_EXIT = 0.30;
    const double SIDE_ENTER = 0.68;
    const double SIDE_EXIT = 0.58;
    const double BACK_ENTER = -0.55;
    const double BACK_EXIT = -0.45;

    t_alien_view_bucket current = (t_alien_view_bucket)a->view_state;
    double abs_side = fabs(view_side);

    if (view_front <= BACK_ENTER || (current == ALIEN_VIEW_BACK && view_front <= BACK_EXIT))
        return ALIEN_VIEW_BACK;

    if (moving && view_front >= WALK_ENTER)
        return ALIEN_VIEW_WALK;
    if (current == ALIEN_VIEW_WALK && moving && view_front >= WALK_EXIT)
        return ALIEN_VIEW_WALK;

    if ((abs_side >= SIDE_ENTER && view_front < 0.5) ||
        ((current == ALIEN_VIEW_SIDE_LEFT || current == ALIEN_VIEW_SIDE_RIGHT) && abs_side >= SIDE_EXIT && view_front < 0.55))
        return (view_side > 0.0) ? ALIEN_VIEW_SIDE_LEFT : ALIEN_VIEW_SIDE_RIGHT;

    if ((view_front >= MID_ENTER && view_front < WALK_ENTER) ||
        ((current == ALIEN_VIEW_MID_LEFT || current == ALIEN_VIEW_MID_RIGHT) && view_front >= MID_EXIT && view_front < WALK_ENTER + 0.05))
        return (view_side >= 0.0) ? ALIEN_VIEW_MID_LEFT : ALIEN_VIEW_MID_RIGHT;

    if (view_front >= FRONT_ENTER || (current == ALIEN_VIEW_FRONT && view_front >= FRONT_EXIT))
        return moving ? ALIEN_VIEW_WALK : ALIEN_VIEW_FRONT;

    if (current == ALIEN_VIEW_WALK && moving && view_front >= WALK_EXIT)
        return ALIEN_VIEW_WALK;

    return ALIEN_VIEW_FRONT;
}

static void alien_apply_player_damage(t_cub *cub)
{
    if (!cub || !cub->player)
        return;
    t_player *p = cub->player;
    if (p->is_dead || p->is_invincible)
        return;
    if (p->health > 0)
        p->health--;
    p->damage_level = (p->damage_level < 3) ? p->damage_level + 1 : 3;
    p->damage_flash_timer = 0.45;
    p->is_invincible = 1;
    p->invincibility_timer = 1.2;
    if (p->health <= 0)
    {
        p->health = 0;
        if (!p->is_dead)
        {
            p->is_dead = 1;
            if (p->death_timer <= 0.0)
                p->death_timer = 3.0;
        }
    }
}

static void alien_prepare_direct_pounce(t_cub *cub, t_alien *a, double target_x, double target_y)
{
    (void)cub;
    if (!a)
        return;
    alien_set_pounce_target(a, target_x, target_y);
    a->is_stalking = 0;
    a->is_preparing = 1;
    a->is_attacking = 0;
    a->prepare_timer = ALIEN_PREP_TOTAL_TIME * 0.6;
    if (a->prepare_timer < 0.2)
        a->prepare_timer = 0.2;
    a->state = ALIEN_PREPARING;
    a->state_timer = a->prepare_timer + ALIEN_POUNCE_MAX_DURATION;
    a->stalk_timer = 0.0;
    alien_reset_path(a);
}

static void alien_enter_search(t_cub *cub, t_alien *a, double center_x, double center_y)
{
    if (!a)
        return;
    alien_reset_path(a);
    a->state = ALIEN_SEARCH;
    a->state_timer = 4.0;
    a->is_stalking = 0;
    a->is_preparing = 0;
    a->is_attacking = 0;
    a->chase_runup_timer = 0.0;
    double target_x = center_x;
    double target_y = center_y;
    double radius_min = BLOCK * 0.8;
    double radius_max = BLOCK * 3.5;
    for (int attempt = 0; attempt < 12; ++attempt)
    {
        double angle = ((double)rand() / (double)RAND_MAX) * 2.0 * M_PI;
        double radius = radius_min + ((double)rand() / (double)RAND_MAX) * (radius_max - radius_min);
        double tx = center_x + cos(angle) * radius;
        double ty = center_y + sin(angle) * radius;
        if (!cub || alien_position_clear(cub, tx, ty, a->hitbox_radius))
        {
            target_x = tx;
            target_y = ty;
            break;
        }
    }
    a->target_x = target_x;
    a->target_y = target_y;
    a->path_recalc_timer = 0.0;
}
static int alien_try_move(t_cub *cub, t_alien *a, double vx, double vy, double speed, double dt, int smooth)
{
    if (!cub || !a || !a->pos) return 0;
    double step = speed * dt;
    if (step <= 0.0) return 0;
    double mvx = vx;
    double mvy = vy;
    if (smooth)
    {
        double blend = fmin(1.0, dt * 6.0);
        a->smooth_vx = (1.0 - blend) * a->smooth_vx + blend * vx;
        a->smooth_vy = (1.0 - blend) * a->smooth_vy + blend * vy;
        double mag = sqrt(a->smooth_vx * a->smooth_vx + a->smooth_vy * a->smooth_vy);
        if (mag > 1e-6)
        {
            mvx = a->smooth_vx / mag;
            mvy = a->smooth_vy / mag;
        }
        a->smooth_speed = (1.0 - blend) * a->smooth_speed + blend * step;
        step = a->smooth_speed;
    }
    else
    {
        a->smooth_vx = vx;
        a->smooth_vy = vy;
        a->smooth_speed = step;
    }
    double nx = a->pos->x + mvx * step;
    double ny = a->pos->y + mvy * step;
    if (alien_position_clear(cub, nx, ny, a->hitbox_radius))
    {
        a->pos->x = nx;
        a->pos->y = ny;
        a->is_walking = 1;
        return 1;
    }
    int moved = 0;
    if (alien_position_clear(cub, a->pos->x + mvx * step, a->pos->y, a->hitbox_radius))
    {
        a->pos->x += mvx * step;
        moved = 1;
    }
    if (alien_position_clear(cub, a->pos->x, a->pos->y + mvy * step, a->hitbox_radius))
    {
        a->pos->y += mvy * step;
        moved = 1;
    }
    if (!moved)
    {
        int mx = (int)(nx / BLOCK);
        int my = (int)(ny / BLOCK);
        alien_mark_wall_hit(a, mx, my);
    }
    else
    {
        a->is_walking = 1;
    }
    return moved;
}

static void alien_face_target(t_alien *a, double tx, double ty, double lerp_speed, double dt)
{
    if (!a || !a->pos || !a->dir) return;
    double dx = tx - a->pos->x;
    double dy = ty - a->pos->y;
    double len = sqrt(dx * dx + dy * dy);
    if (len < 1e-6) return;
    double target_x = dx / len;
    double target_y = dy / len;
    double blend = lerp_speed * dt;
    if (blend > 1.0) blend = 1.0;
    a->dir->x = (1.0 - blend) * a->dir->x + blend * target_x;
    a->dir->y = (1.0 - blend) * a->dir->y + blend * target_y;
    double norm = sqrt(a->dir->x * a->dir->x + a->dir->y * a->dir->y);
    if (norm > 1e-6)
    {
        a->dir->x /= norm;
        a->dir->y /= norm;
    }
}

static void alien_update_anim(t_alien *a, int threshold)
{
    if (!a) return;
    if (++a->anim_timer >= threshold)
    {
        a->anim_timer = 0;
        a->anim_frame = (a->anim_frame + 1) % 4;
    }
}

static void alien_choose_patrol_target(t_cub *cub, t_alien *a)
{
    if (!cub || !cub->map || !a) return;
    int map_h = 0;
    int map_w = 0;
    for (int y = 0; cub->map[y]; ++y)
    {
        int len = (int)strlen(cub->map[y]);
        if (len > map_w) map_w = len;
        map_h++;
    }
    if (map_h == 0 || map_w == 0) return;
    double best_x = a->pos->x;
    double best_y = a->pos->y;
    double best_score = -1e9;
    int found = 0;
    double ideal_dist = BLOCK * 6.0;
    for (int attempt = 0; attempt < 64; ++attempt)
    {
        int mx = rand() % map_w;
        int my = rand() % map_h;
        if (is_blocking_cell(cub, mx, my))
            continue;
        double px = mx * BLOCK + BLOCK * 0.5;
        double py = my * BLOCK + BLOCK * 0.5;
        double dx = px - a->pos->x;
        double dy = py - a->pos->y;
        double dist = sqrt(dx * dx + dy * dy);
        if (dist < BLOCK * 2.0 || dist > BLOCK * 12.0)
            continue;
        double norm = (dist > 1e-6) ? dist : 1.0;
        double dirx = dx / norm;
        double diry = dy / norm;
        double alignment = a->dir->x * dirx + a->dir->y * diry;
        double dist_score = 1.0 - fabs(dist - ideal_dist) / ideal_dist;
        if (dist_score < -1.0) dist_score = -1.0;
        double score = alignment * 0.7 + dist_score * 0.3;
        if (!found || score > best_score)
        {
            best_score = score;
            best_x = px;
            best_y = py;
            found = 1;
        }
    }
    if (!found)
    {
        int fallback_mx = (int)(a->pos->x / BLOCK);
        int fallback_my = (int)(a->pos->y / BLOCK);
        for (int dy = -3; dy <= 3 && !found; ++dy)
        {
            for (int dx = -3; dx <= 3 && !found; ++dx)
            {
                int mx = fallback_mx + dx;
                int my = fallback_my + dy;
                if (is_blocking_cell(cub, mx, my))
                    continue;
                best_x = mx * BLOCK + BLOCK * 0.5;
                best_y = my * BLOCK + BLOCK * 0.5;
                found = 1;
            }
        }
    }
    a->target_x = best_x;
    a->target_y = best_y;
    alien_reset_path(a);
}

static int adjust_goal_if_blocked(t_cub *cub, int *mx, int *my)
{
    if (!cub || !mx || !my) return 0;
    if (!is_blocking_cell(cub, *mx, *my)) return 1;
    static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (int i = 0; i < 4; ++i)
    {
        int nx = *mx + dirs[i][0];
        int ny = *my + dirs[i][1];
        if (!is_blocking_cell(cub, nx, ny))
        {
            *mx = nx;
            *my = ny;
            return 1;
        }
    }
    return 0;
}

static int alien_build_path(t_cub *cub, t_alien *a, double target_x, double target_y)
{
    if (!cub || !a || !a->pos) return 0;
    int map_h = 0;
    int map_w = 0;
    for (int y = 0; cub->map && cub->map[y]; ++y)
    {
        int len = (int)strlen(cub->map[y]);
        if (len > map_w) map_w = len;
        map_h++;
    }
    if (map_h == 0 || map_w == 0) return 0;
    int start_mx = (int)(a->pos->x / BLOCK);
    int start_my = (int)(a->pos->y / BLOCK);
    int goal_mx = (int)(target_x / BLOCK);
    int goal_my = (int)(target_y / BLOCK);
    if (goal_mx < 0) goal_mx = 0;
    if (goal_my < 0) goal_my = 0;
    if (goal_mx >= map_w) goal_mx = map_w - 1;
    if (goal_my >= map_h) goal_my = map_h - 1;
    if (!adjust_goal_if_blocked(cub, &goal_mx, &goal_my))
        return 0;
    if (start_mx == goal_mx && start_my == goal_my)
    {
        alien_reset_path(a);
        return 1;
    }
    if (a->path_length > 0 && a->path_goal_mx == goal_mx && a->path_goal_my == goal_my && a->path_recalc_timer > 0.0)
        return 1;
    int total = map_w * map_h;
    int *queue_x = malloc(sizeof(int) * total);
    int *queue_y = malloc(sizeof(int) * total);
    int *prev = malloc(sizeof(int) * total);
    char *visited = malloc((size_t)total);
    if (!queue_x || !queue_y || !prev || !visited)
    {
        free(queue_x); free(queue_y); free(prev); free(visited);
        return 0;
    }
    for (int i = 0; i < total; ++i)
    {
        prev[i] = -1;
        visited[i] = 0;
    }
    int head = 0, tail = 0;
    int start_idx = start_my * map_w + start_mx;
    queue_x[tail] = start_mx;
    queue_y[tail] = start_my;
    visited[start_idx] = 1;
    tail++;
    int found = 0;
    static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    while (head < tail)
    {
        int cx = queue_x[head];
        int cy = queue_y[head];
        int cidx = cy * map_w + cx;
        head++;
        if (cx == goal_mx && cy == goal_my)
        {
            found = 1;
            break;
        }
        for (int d = 0; d < 4; ++d)
        {
            int nx = cx + dirs[d][0];
            int ny = cy + dirs[d][1];
            if (nx < 0 || ny < 0 || nx >= map_w || ny >= map_h)
                continue;
            int nidx = ny * map_w + nx;
            if (visited[nidx])
                continue;
            if (is_blocking_cell(cub, nx, ny))
                continue;
            visited[nidx] = 1;
            prev[nidx] = cidx;
            queue_x[tail] = nx;
            queue_y[tail] = ny;
            tail++;
        }
    }
    int goal_idx = goal_my * map_w + goal_mx;
    if (!found || prev[goal_idx] == -1)
    {
        free(queue_x); free(queue_y); free(prev); free(visited);
        return 0;
    }
    int tmp_x[ALIEN_MAX_PATH];
    int tmp_y[ALIEN_MAX_PATH];
    int count = 0;
    int trace = goal_idx;
    while (trace != start_idx && trace >= 0 && count < ALIEN_MAX_PATH)
    {
        int cx = trace % map_w;
        int cy = trace / map_w;
        tmp_x[count] = cx;
        tmp_y[count] = cy;
        trace = prev[trace];
        count++;
    }
    free(queue_x); free(queue_y); free(prev); free(visited);
    if (trace != start_idx && count == ALIEN_MAX_PATH)
    {
        alien_reset_path(a);
        return 0;
    }
    a->path_length = count;
    a->path_index = 0;
    for (int i = 0; i < count; ++i)
    {
        int idx = count - 1 - i;
        a->path_points[i].x = tmp_x[idx] * BLOCK + BLOCK * 0.5;
        a->path_points[i].y = tmp_y[idx] * BLOCK + BLOCK * 0.5;
    }
    a->path_goal_mx = goal_mx;
    a->path_goal_my = goal_my;
    a->path_recalc_timer = 0.2;
    return 1;
}

static int alien_follow_path(t_cub *cub, t_alien *a, double speed, double dt)
{
    if (!a || a->path_length <= 0 || a->path_index >= a->path_length)
        return 1;
    double tolerance = fmax(6.0, speed * dt * 1.5);
    while (a->path_index < a->path_length)
    {
        t_cord target = a->path_points[a->path_index];
        double dx = target.x - a->pos->x;
        double dy = target.y - a->pos->y;
        double dist = sqrt(dx * dx + dy * dy);
        if (dist < tolerance)
        {
            a->path_index++;
            continue;
        }
        if (dist < 1e-6)
        {
            a->path_index++;
            continue;
        }
        double vx = dx / dist;
        double vy = dy / dist;
        alien_face_target(a, target.x, target.y, 8.0, dt);
        if (!alien_try_move(cub, a, vx, vy, speed, dt, 1))
        {
            alien_reset_path(a);
        }
        return 0;
    }
    return 1;
}

static int alien_player_visible(t_cub *cub, t_alien *a, double *out_dist)
{
    if (!cub || !a || !cub->player || !cub->player->position) return 0;
    double px = cub->player->position->x;
    double py = cub->player->position->y;
    double dx = px - a->pos->x;
    double dy = py - a->pos->y;
    double dist = sqrt(dx * dx + dy * dy);
    if (out_dist) *out_dist = dist;
    if (dist > a->view_distance)
        return 0;
    double len = dist;
    if (len < 1e-6)
        return 1;
    double ndx = dx / len;
    double ndy = dy / len;
    double dot = a->dir->x * ndx + a->dir->y * ndy;
    double cos_fov = cos(a->fov_deg * (M_PI / 180.0));
    if (dot < cos_fov)
        return 0;
    if (!line_of_sight(cub, a, px, py))
        return 0;
    return 1;
}

/* simplified movement handlers omitted for brevity: keep existing implementations in other files if present */

void alien_update(t_cub *cub,double dt)
{
    if (!cub || !cub->alien || !cub->player) return;
    t_alien *a = cub->alien;
    if (!a->pos || !a->dir || !cub->player->position) return;
    if (dt > 0.2) dt = 0.2;
    if (dt <= 0.0) dt = 1.0 / 60.0;

    a->is_walking = 0;
    if (dt > 0.0) {
        a->smooth_speed *= 0.96;
        a->smooth_vx *= 0.94;
        a->smooth_vy *= 0.94;
    }
    if (a->attack_cooldown > 0.0)
    {
        a->attack_cooldown -= dt;
        if (a->attack_cooldown < 0.0)
            a->attack_cooldown = 0.0;
    }
    if (a->state_timer > 0.0)
    {
        a->state_timer -= dt;
        if (a->state_timer < 0.0)
            a->state_timer = 0.0;
    }
    if (a->path_recalc_timer > 0.0)
    {
        a->path_recalc_timer -= dt;
        if (a->path_recalc_timer < 0.0)
            a->path_recalc_timer = 0.0;
    }
    if (a->crouch_timer > 0.0)
    {
        a->crouch_timer -= dt;
        if (a->crouch_timer <= 0.0)
        {
            a->crouch_timer = 0.0;
            a->is_crouching = 0;
        }
    }
    if (a->attack_anim_timer > 0.0)
    {
        a->attack_anim_timer -= dt;
        if (a->attack_anim_timer <= 0.0)
        {
            a->attack_anim_timer = 0.0;
            a->has_attacked = 0;
        }
    }
    else if (a->has_attacked && a->attack_cooldown <= 0.0)
        a->has_attacked = 0;

    double prev_x = a->pos->x;
    double prev_y = a->pos->y;

    double px = cub->player->position->x;
    double py = cub->player->position->y;
    double player_dist = 0.0;
    int player_visible = alien_player_visible(cub, a, &player_dist);
    if (player_dist <= 0.0)
    {
        double dx = px - a->pos->x;
        double dy = py - a->pos->y;
        player_dist = sqrt(dx * dx + dy * dy);
    }
    int attack_pipeline = (a->is_preparing || a->is_stalking || a->is_attacking);
    double audible_range = fmin(a->view_distance * 0.75, BLOCK * 6.0);
    int player_audible = (!player_visible && player_dist <= audible_range);
    if (a->state != ALIEN_CHASE)
        a->chase_runup_timer = 0.0;

    if (!attack_pipeline)
    {
        if (player_visible)
        {
            a->last_seen_x = px;
            a->last_seen_y = py;
            a->last_sensed_x = px;
            a->last_sensed_y = py;
            a->state = ALIEN_CHASE;
            a->state_timer = 1.2;
            alien_reset_path(a);
        }
        else if (player_audible)
        {
            a->last_sensed_x = px;
            a->last_sensed_y = py;
            if (!a->is_stalking && !a->is_preparing && !a->is_attacking)
            {
                a->state = ALIEN_STALKING;
                a->is_stalking = 1;
                a->stalk_timer = 1.1;
                alien_reset_path(a);
            }
        }
        else if (a->state == ALIEN_CHASE && a->state_timer <= 0.0)
        {
            double sx = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_x : a->pos->x;
            double sy = (a->last_seen_y != 0.0 || a->last_seen_x != 0.0) ? a->last_seen_y : a->pos->y;
            alien_enter_search(cub, a, sx, sy);
        }
    }

    if (a->is_preparing)
    {
        double focus_x = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_x : px;
        double focus_y = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_y : py;
        alien_set_pounce_target(a, focus_x, focus_y);
        alien_face_target(a, focus_x, focus_y, 14.0, dt);
        a->prepare_timer -= dt;
        if (a->prepare_timer <= 0.0)
        {
            a->is_preparing = 0;
            a->is_attacking = 1;
            a->pounce_timer = ALIEN_POUNCE_MAX_DURATION;
            alien_face_target(a, a->pounce_target_x, a->pounce_target_y, 25.0, dt);
            a->state = ALIEN_CHASE;
        }
        return;
    }

    if (a->is_stalking)
    {
        double stalk_target_x = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_x : px;
        double stalk_target_y = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_y : py;
        if (!alien_build_path(cub, a, stalk_target_x, stalk_target_y))
        {
            double dx = stalk_target_x - a->pos->x;
            double dy = stalk_target_y - a->pos->y;
            double len = sqrt(dx * dx + dy * dy);
            if (len > 1e-3)
            {
                double vx = dx / len;
                double vy = dy / len;
                alien_try_move(cub, a, vx, vy, a->speed_patrol * 0.7, dt, 1);
                alien_face_target(a, stalk_target_x, stalk_target_y, 10.0, dt);
            }
        }
        else
        {
            alien_follow_path(cub, a, a->speed_patrol * 0.7, dt);
        }
        alien_update_anim(a, 6);
        a->stalk_timer -= dt;
        if (a->stalk_timer <= 0.0)
        {
            a->is_stalking = 0;
            a->is_preparing = 1;
            a->prepare_timer = ALIEN_PREP_TOTAL_TIME;
            a->state = ALIEN_PREPARING;
        }
        return;
    }

    if (a->is_attacking)
    {
        const double contact_dist = BLOCK * 0.55;
        const double target_hit_radius = fmax(contact_dist, BLOCK * 0.35);
        double dist_to_target = sqrt((a->pounce_target_x - a->pos->x) * (a->pounce_target_x - a->pos->x)
                + (a->pounce_target_y - a->pos->y) * (a->pounce_target_y - a->pos->y));
        if (player_dist <= contact_dist)
        {
            alien_finish_attack(cub, a, 1);
            return;
        }
        if (dist_to_target <= target_hit_radius)
        {
            alien_finish_attack(cub, a, 0);
            return;
        }
        alien_face_target(a, a->pounce_target_x, a->pounce_target_y, 22.0, dt);
        int moved = alien_try_move(cub, a, a->pounce_vx, a->pounce_vy, a->pounce_speed, dt, 0);
        if (!moved)
        {
            dist_to_target = sqrt((a->pounce_target_x - a->pos->x) * (a->pounce_target_x - a->pos->x)
                    + (a->pounce_target_y - a->pos->y) * (a->pounce_target_y - a->pos->y));
            if (player_dist <= contact_dist + BLOCK * 0.15 || dist_to_target <= target_hit_radius)
                alien_finish_attack(cub, a, player_dist <= contact_dist + BLOCK * 0.15);
            else
            {
                a->is_attacking = 0;
                a->pounce_timer = 0.0;
                if (a->attack_cooldown < 2.5)
                    a->attack_cooldown = 2.5;
                a->state = ALIEN_SEARCH;
                a->state_timer = 2.5;
                alien_reset_path(a);
            }
            return;
        }
        dist_to_target = sqrt((a->pounce_target_x - a->pos->x) * (a->pounce_target_x - a->pos->x)
                + (a->pounce_target_y - a->pos->y) * (a->pounce_target_y - a->pos->y));
        if (player_dist <= contact_dist || dist_to_target <= target_hit_radius)
        {
            alien_finish_attack(cub, a, player_dist <= contact_dist);
            return;
        }
        if (a->pounce_timer > 0.0)
        {
            a->pounce_timer -= dt;
            if (a->pounce_timer <= 0.0)
            {
                a->pounce_timer = 0.0;
                a->is_attacking = 0;
                if (a->attack_cooldown < 2.5)
                    a->attack_cooldown = 2.5;
                a->state = ALIEN_SEARCH;
                a->state_timer = 2.5;
                alien_reset_path(a);
            }
        }
        return;
    }

    switch (a->state)
    {
        case ALIEN_PATROL:
        default:
        {
            if ((a->target_x == 0.0 && a->target_y == 0.0) ||
                hypot(a->target_x - a->pos->x, a->target_y - a->pos->y) < BLOCK * 0.5)
            {
                alien_choose_patrol_target(cub, a);
            }
            if (!alien_build_path(cub, a, a->target_x, a->target_y))
            {
                double dx = a->target_x - a->pos->x;
                double dy = a->target_y - a->pos->y;
                double len = sqrt(dx * dx + dy * dy);
                if (len > 1e-3)
                {
                    double vx = dx / len;
                    double vy = dy / len;
                    alien_try_move(cub, a, vx, vy, a->speed_patrol, dt, 1);
                    alien_face_target(a, a->target_x, a->target_y, 6.0, dt);
                }
            }
            else
            {
                if (alien_follow_path(cub, a, a->speed_patrol, dt))
                    alien_choose_patrol_target(cub, a);
            }
            alien_update_anim(a, 10);
            break;
        }
        case ALIEN_SEARCH:
        {
            double goal_x = a->target_x;
            double goal_y = a->target_y;
            if (a->has_attacked && (fabs(a->retreat_x) > 1.0 || fabs(a->retreat_y) > 1.0))
            {
                goal_x = a->retreat_x;
                goal_y = a->retreat_y;
            }
            if (!alien_build_path(cub, a, goal_x, goal_y))
        {
            double dx = goal_x - a->pos->x;
            double dy = goal_y - a->pos->y;
            double len = sqrt(dx * dx + dy * dy);
            if (len > 1e-3)
            {
                double vx = dx / len;
                double vy = dy / len;
                alien_try_move(cub, a, vx, vy, a->speed_patrol * 0.6, dt, 1);
                alien_face_target(a, goal_x, goal_y, 7.0, dt);
            }
        }
        else
        {
            alien_follow_path(cub, a, a->speed_patrol * 0.65, dt);
        }
        alien_update_anim(a, 8);
        double dist_goal = hypot(goal_x - a->pos->x, goal_y - a->pos->y);
        if (dist_goal < BLOCK * 0.5 || a->state_timer <= 0.0)
        {
            double sx = (a->last_seen_x != 0.0 || a->last_seen_y != 0.0) ? a->last_seen_x : goal_x;
            double sy = (a->last_seen_y != 0.0 || a->last_seen_x != 0.0) ? a->last_seen_y : goal_y;
            alien_enter_search(cub, a, sx, sy);
        }
        break;
    }
        case ALIEN_CHASE:
        {
            if (player_visible)
            {
                double dx = px - a->pos->x;
                double dy = py - a->pos->y;
                double len = sqrt(dx * dx + dy * dy);
                if (len > 1e-3)
                {
                    double vx = dx / len;
                    double vy = dy / len;
                    if (!alien_try_move(cub, a, vx, vy, a->speed_chase, dt, 1))
                    {
                        if (alien_build_path(cub, a, px, py))
                            alien_follow_path(cub, a, a->speed_chase * 0.95, dt);
                    }
                    alien_face_target(a, px, py, 12.0, dt);
                    alien_update_anim(a, 5);
                    if (!a->is_preparing && !a->is_attacking && !a->is_stalking)
                    {
                        if (a->is_walking && player_dist <= BLOCK * 1.5)
                            a->chase_runup_timer += dt;
                        else
                        {
                            a->chase_runup_timer -= dt * 0.5;
                            if (a->chase_runup_timer < 0.0)
                                a->chase_runup_timer = 0.0;
                        }
                        if (a->chase_runup_timer >= 0.8 && player_dist <= BLOCK * 1.4 && a->attack_cooldown <= 0.0)
                        {
                            alien_prepare_direct_pounce(cub, a, px, py);
                            a->chase_runup_timer = 0.0;
                            return;
                        }
                    }
                    else
                        a->chase_runup_timer = 0.0;
                }
            }
            else
            {
                if (alien_build_path(cub, a, a->last_seen_x, a->last_seen_y))
                {
                    alien_follow_path(cub, a, a->speed_chase * 0.9, dt);
                    alien_update_anim(a, 6);
                }
                else
                {
                    a->chase_runup_timer = 0.0;
                    a->state = ALIEN_SEARCH;
                    a->state_timer = 2.5;
                    a->target_x = a->last_seen_x;
                    a->target_y = a->last_seen_y;
                }
            }
            break;
        }
        case ALIEN_PREPARING:
            break;
    }

    double moved = sqrt((a->pos->x - prev_x) * (a->pos->x - prev_x) +
                        (a->pos->y - prev_y) * (a->pos->y - prev_y));
    if (moved < 1.0)
        a->stuck_timer += dt;
    else
        a->stuck_timer = 0.0;
    if (a->stuck_timer > 1.5)
    {
        a->stuck_timer = 0.0;
        a->is_crouching = 1;
        a->crouch_timer = 1.0;
        alien_reset_path(a);
        alien_choose_patrol_target(cub, a);
    }
    int cell_mx = (int)(a->pos->x / BLOCK);
    int cell_my = (int)(a->pos->y / BLOCK);
    if (moved < 1.0)
    {
        if (cell_mx == a->stall_cell_mx && cell_my == a->stall_cell_my)
            a->stall_timer += dt;
        else
        {
            a->stall_cell_mx = cell_mx;
            a->stall_cell_my = cell_my;
            a->stall_timer = dt;
        }
    }
    else
    {
        a->stall_cell_mx = cell_mx;
        a->stall_cell_my = cell_my;
        a->stall_timer = 0.0;
    }
    if (a->stall_timer >= 4.0)
    {
        alien_force_unstuck(cub, a);
        a->stall_timer = 0.0;
    }
    a->prev_pos_x = a->pos->x;
    a->prev_pos_y = a->pos->y;
}

/* public helpers */
void alien_cycle_mask_mode(t_cub *cub)
{
    (void)cub;
    if (alien_mask_mode == MASK_AUTO) alien_mask_mode = MASK_MAGENTA;
    else if (alien_mask_mode == MASK_MAGENTA) alien_mask_mode = MASK_BLACK;
    else if (alien_mask_mode == MASK_BLACK) alien_mask_mode = MASK_ALPHA;
    else alien_mask_mode = MASK_AUTO;
    alien_mask_detected = 0;
}

void alien_detect_mask(t_cub *cub)
{
    if (!cub)
        return;
    detect_alien_mask_mode(cub);
    build_alien_mask_map_for_tex(cub, &cub->alien_tex);
}

void alien_toggle_force_transparent(t_cub *cub)
{
    (void)cub; static int enabled=0; enabled = !enabled; alien_replace_bg_color = enabled ? 0x000000 : -1;
}

void alien_build_mask(t_cub *cub)
{
    if (!cub) return;
    /* ensure global detection at least once */
    if (!alien_mask_detected) detect_alien_mask_mode(cub);
    /* build masks for all alien-related textures if they exist */
    build_alien_mask_map_for_tex(cub, &cub->alien_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_back_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_w1_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_w2_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_w3_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_attack_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_bond_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_attack1_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_attack2_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_look_right_tex);
    build_alien_mask_map_for_tex(cub, &cub->alien_look_left_tex);
    /* view_mi_face textures not currently loaded - skip mask build */

    /* debug report counts only for primary alien texture to avoid log flood */
    build_and_report(cub, &cub->alien_tex, "alien_tex");
}

int alien_init(t_cub *cub)
{
    if (!cub) return 0;
    if (cub->alien) return 1; /* already initialized */
    t_alien *a = (t_alien*)malloc(sizeof(t_alien));
    if (!a) return 0;
    memset(a, 0, sizeof(t_alien));
    a->pos = (t_cord*)malloc(sizeof(t_cord));
    a->dir = (t_cord*)malloc(sizeof(t_cord));
    if (!a->pos || !a->dir) { free(a->pos); free(a->dir); free(a); return 0; }
    /* place at map spawn if available, otherwise default to near origin */
    if (cub->alien_spawn_x >= 0 && cub->alien_spawn_y >= 0)
    {
        a->pos->x = (double)cub->alien_spawn_x * BLOCK + BLOCK / 2.0;
        a->pos->y = (double)cub->alien_spawn_y * BLOCK + BLOCK / 2.0;
    }
    else
    {
        a->pos->x = BLOCK*2 + BLOCK/2; a->pos->y = BLOCK*2 + BLOCK/2;
    }
    a->dir->x = 0.0; a->dir->y = -1.0;
    a->pounce_vx = 0.0;
    a->pounce_vy = -1.0;
    a->pounce_speed = 300.0;
    a->pounce_target_x = a->pos->x;
    a->pounce_target_y = a->pos->y - BLOCK;
    a->pounce_timer = 0.0;
    a->speed_patrol = 40.0; a->speed_chase = 80.0;
    a->hitbox_radius = 12.0;
    a->fov_deg = 60.0; a->view_distance = BLOCK*8;
    a->state = ALIEN_PATROL;
    a->target_x = a->pos->x;
    a->target_y = a->pos->y;
    a->wall_hit_mx = -1;
    a->wall_hit_my = -1;
    a->wall_hit_prev_mx = -1;
    a->wall_hit_prev_my = -1;
    a->path_goal_mx = -1;
    a->path_goal_my = -1;
    a->path_length = 0;
    a->path_index = 0;
    a->path_recalc_timer = 0.0;
    a->stuck_timer = 0.0;
    a->prev_pos_x = a->pos->x;
    a->prev_pos_y = a->pos->y;
    a->retreat_x = a->pos->x;
    a->retreat_y = a->pos->y;
    a->attack_cooldown = 0.0;
    a->is_walking = 0;
    a->smooth_vx = 0.0;
    a->smooth_vy = 0.0;
    a->smooth_speed = 0.0;
    a->stall_timer = 0.0;
    a->stall_cell_mx = -1;
    a->stall_cell_my = -1;
    a->chase_runup_timer = 0.0;
    a->view_state = ALIEN_VIEW_FRONT;
    alien_reset_path(a);
    alien_choose_patrol_target(cub, a);
    cub->alien = a;
    /* ensure masks exist for visibility */
    alien_build_mask(cub);
    return 1;
}

void alien_free(t_cub *cub)
{
    if (!cub || !cub->alien) return;
    t_alien *a = cub->alien;
    if (a->pos) free(a->pos);
    if (a->dir) free(a->dir);
    free(a);
    cub->alien = NULL;
}

void alien_toggle_debug(t_cub *cub __attribute__((unused)))
{
    alien_debug_show_raw = !alien_debug_show_raw;
}

void alien_toggle_replace_bg(t_cub *cub __attribute__((unused)))
{
    (void)cub;
    if (alien_replace_bg_color == -1) alien_replace_bg_color = 0x000000;
    else alien_replace_bg_color = -1;
}

void alien_dump_corners(t_cub *cub)
{
    int w = cub->alien_tex.w, h = cub->alien_tex.h, bytes = cub->alien_tex.bpp/8;
    int corners[4][2] = {{1,1},{w-2,1},{1,h-2},{w-2,h-2}};
    for (int i=0;i<4;++i)
    {
        int sx = corners[i][0], sy = corners[i][1];
        int idx = sy * cub->alien_tex.sl + sx * bytes;
        unsigned char *p = (unsigned char*)cub->alien_tex.data + idx;
    }
}
            
