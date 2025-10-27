#include "../../includes/cub3d.h"


void update_player_trig(t_cub *cub)
{
    t_player *player;

    if (!cub || !cub->player)
        return;
    if (!cub->player->direction || !cub->player->plane)
        return;
    player = cub->player;
    player->cos_angle = cos(player->angle);
    player->sin_angle = sin(player->angle);
    player->direction->x = player->cos_angle;
    player->direction->y = player->sin_angle;
    find_player_plane(cub->player); // <-- réutilisation
}


/**
 * Checks if a specific map coordinate contains a wall
 * @param cub The main game structure
 * @param map_x Integer map x coordinate
 * @param map_y Integer map y coordinate
 * @return 1 if wall, 0 if not wall or out of bounds
 */
static int is_wall(t_cub *cub, int map_x, int map_y)
{
    // Check bounds
    if (map_y < 0 || map_x < 0)
        return (1); // Treat out of bounds as walls
    if (!cub->map[map_y])
        return (1); // Treat out of bounds as walls
    if ((size_t)map_x >= ft_strlen(cub->map[map_y]))
        return (1);
    // Check if it's a wall-like tile
    return (cub->map[map_y][map_x] == '1'
        || cub->map[map_y][map_x] == 'P'
        || cub->map[map_y][map_x] == 'D'
        || cub->map[map_y][map_x] == '6'
        || cub->map[map_y][map_x] == 'B'
        || cub->map[map_y][map_x] == ' ');
}

/**
 * Checks if a position is valid using radius-based collision detection
 * @param cub The main game structure
 * @param x Map x coordinate (center of player)
 * @param y Map y coordinate (center of player)
 * @return 1 if valid, 0 if collision would occur
 */
int is_valid_position(t_cub *cub, double x, double y)
{
    double radius;
    double left;
    double right;
    double top;
    double bottom;

    radius = cub->player->radius; // Rayon du joueur (en pixels ou unités du monde)
    left = (x - radius) / BLOCK;  // 180°   Coordonnée x de la bordure gauche du joueur (convertie en case de la map)
    right = (x + radius) / BLOCK; // 0° Coordonnée x de la bordure droite du joueur (convertie en case de la map)
    top = (y - radius) / BLOCK;   // 90°  Coordonnée y de la bordure supérieure du joueur (convertie en case de la map)
    bottom = (y + radius) / BLOCK;// 270° Coordonnée y de la bordure inférieure du joueur (convertie en case de la map)

    /* Quick bounds check: ensure computed tile indices are inside the map */
    if (!cub || !cub->map)
        return (0);
    /* Reject positions that would place the center far outside the known map (flood protection) */
    {
        int map_h = 0;
        int map_w = 0;
        while (cub->map && cub->map[map_h])
            map_h++;
        if (map_h > 0 && cub->map[0])
            map_w = (int)ft_strlen(cub->map[0]);
        /* convert center to tile coords */
        int cx = (int)floor(x / (double)BLOCK);
        int cy = (int)floor(y / (double)BLOCK);
        if (cy < 0 || cy >= map_h)
            return (0);
        if (cx < 0 || cx >= map_w)
            return (0);
    }

    /* Check the four corners of the player's bounding box */
    if (is_wall(cub, (int)left, (int)top)) // Top-left
        return (0);
    if (is_wall(cub, (int)right, (int)top)) // Top-right
        return (0);
    if (is_wall(cub, (int)left, (int)bottom)) // Bottom-left
        return (0);
    if (is_wall(cub, (int)right, (int)bottom)) // Bottom-right
        return (0);

    return (1);
}

/**
 * Advanced wall sliding system - attempts multiple sliding strategies
 * @param cub The main game structure
 * @param move_x Desired movement on X axis
 * @param move_y Desired movement on Y axis
 * @return 1 if any movement was possible, 0 if completely blocked
 */
static int try_wall_slide(t_cub *cub, double move_x, double move_y)
{
    double current_x = cub->player->position->x;
    double current_y = cub->player->position->y;
    double new_x = current_x + move_x;
    double new_y = current_y + move_y;
    
    // 1. Essayer le mouvement complet
    if (is_valid_position(cub, new_x, new_y)) {
        cub->player->position->x = new_x;
        cub->player->position->y = new_y;
        return (1);
    }
    
    // 2. Glissement axial (X seulement)
    if (is_valid_position(cub, new_x, current_y)) {
        cub->player->position->x = new_x;
        return (1);
    }
    
    // 3. Glissement axial (Y seulement)
    if (is_valid_position(cub, current_x, new_y)) {
        cub->player->position->y = new_y;
        return (1);
    }
    
    // 4. Glissement diagonal réduit (multiple tentatives)
    double slide_factors[] = {0.75, 0.5, 0.25, 0.1};
    int num_factors = sizeof(slide_factors) / sizeof(slide_factors[0]);
    
    for (int i = 0; i < num_factors; i++) {
        double factor = slide_factors[i];
        double slide_x = current_x + (move_x * factor);
        double slide_y = current_y + (move_y * factor);
        
        if (is_valid_position(cub, slide_x, slide_y)) {
            cub->player->position->x = slide_x;
            cub->player->position->y = slide_y;
            return (1);
        }
        
        // Essayer aussi les axes séparément avec le facteur réduit
        if (is_valid_position(cub, slide_x, current_y)) {
            cub->player->position->x = slide_x;
            return (1);
        }
        if (is_valid_position(cub, current_x, slide_y)) {
            cub->player->position->y = slide_y;
            return (1);
        }
    }
    
    return (0); // Aucun mouvement possible
}

static void rotate_player(t_cub *cub)
{
    bool        rotated;

    if (!cub || !cub->player)
        return ;
    rotated = false;
    if (cub->player->left_rotate)
    {
        cub->player->angle -= cub->player->rotation_frame;
        rotated = true;
    }
    if (cub->player->right_rotate)
    {
        cub->player->angle += cub->player->rotation_frame;
        rotated = true;
    }
    if (rotated)
        update_player_trig(cub);
}

/* Rotation progressive du demi-tour pour effet de tête qui tourne rapidement */
static void update_half_turn_animation(t_cub *cub)
{
    double angle_diff;
    double abs_diff;
    double turn_step;

    if (!cub || !cub->player || !cub->player->is_turning)
        return ;
    /* calculer la différence d'angle en prenant le chemin le plus court */
    angle_diff = cub->player->turn_target_angle - cub->player->angle;
    /* normaliser la différence entre -PI et PI */
    while (angle_diff > M_PI)
        angle_diff -= 2.0 * M_PI;
    while (angle_diff < -M_PI)
        angle_diff += 2.0 * M_PI;
    abs_diff = fabs(angle_diff);
    /* si on est arrivé (seuil petit), terminer l'animation */
    if (abs_diff < 0.01)
    {
        cub->player->angle = cub->player->turn_target_angle;
        cub->player->is_turning = false;
        update_player_trig(cub);
        return ;
    }
    /* sinon, tourner d'un pas en direction de la cible */
    turn_step = cub->player->turn_speed;
    if (abs_diff < turn_step)
        turn_step = abs_diff; /* ralentir en fin de rotation */
    if (angle_diff > 0.0)
        cub->player->angle += turn_step;
    else
        cub->player->angle -= turn_step;
    /* normaliser l'angle */
    cub->player->angle = fmod(cub->player->angle, 2.0 * M_PI);
    if (cub->player->angle < 0.0)
        cub->player->angle += 2.0 * M_PI;
    update_player_trig(cub);
}

void move_player(t_cub *cub)
{
    double move_x = 0.0;
    double move_y = 0.0;
    
    /* === GESTION DE L'INVINCIBILITÉ === */
    if (cub->player->invincibility_timer > 0.0)
    {
        cub->player->invincibility_timer -= (1.0 / 60.0); /* décrémentation basée sur 60 FPS */
        if (cub->player->invincibility_timer <= 0.0)
        {
            cub->player->is_invincible = 0;
            cub->player->invincibility_timer = 0.0;
        }
    }
    
    /* === GESTION DU FLASH DE DÉGÂTS === */
    if (cub->player->damage_flash_timer > 0.0)
    {
        cub->player->damage_flash_timer -= (1.0 / 60.0);
        if (cub->player->damage_flash_timer <= 0.0)
        {
            cub->player->damage_flash_timer = 0.0;
        }
    }
    
    /* === GESTION DU GAME OVER === */
    if (cub->player->is_dead)
    {
        cub->player->death_timer -= (1.0 / 60.0);
        
        if (cub->player->death_timer <= 0.0)
        {
            /* RESTART : Réinitialiser le joueur */
            cub->player->health = cub->player->max_health;
            cub->player->is_dead = 0;
            cub->player->death_timer = 0.0;
            cub->player->damage_level = 0;
            cub->player->damage_flash_timer = 0.0;
            cub->player->is_invincible = 0;
            cub->player->invincibility_timer = 0.0;
            
            /* Réinitialiser la position du joueur au spawn */
            find_player_pos(cub);
            find_player_dir(cub);
        }
        
        /* Bloquer les mouvements pendant le game over */
        return;
    }
    
    /* appliquer la rotation progressive du demi-tour */
    update_half_turn_animation(cub);
    
    rotate_player(cub);
    
    // Calculer le vecteur de déplacement total
    if (cub->player->key_up) {
        move_x += cub->player->direction->x;
        move_y += cub->player->direction->y;
    }
    if (cub->player->key_down) {
        move_x -= cub->player->direction->x;
        move_y -= cub->player->direction->y;
    }
    if (cub->player->key_left) {
        move_x += cub->player->direction->y;
        move_y -= cub->player->direction->x;
    }
    if (cub->player->key_right) {
        move_x -= cub->player->direction->y;
        move_y += cub->player->direction->x;
    }
    
    double len;
    double target_speed;
    double lerp_factor;

    /* déterminer si le joueur est en mouvement (input présent) */
    /* utiliser la fonction utilitaire centralisée */
    bool moving = player_is_moving(cub->player);

    /* déterminer vitesse cible selon l'état du joueur mais seulement si moving */
    if (!moving)
        target_speed = cub->player->walk_speed;
    else if (cub->player->is_crouching)
        target_speed = cub->player->crouch_speed;
    else if (cub->player->is_running)
        target_speed = cub->player->run_speed;
    else
        target_speed = cub->player->walk_speed;

    /* interpolation douce de la vitesse effective et des paramètres visuels */
    lerp_factor = 0.10; /* smoothing factor per frame (réduit pour effet plus discret) */
    cub->player->move_frame = cub->player->move_frame + (target_speed - cub->player->move_frame) * lerp_factor;

    /* FOV smoothing: n'appliquer l'augmentation que si moving */
    {
        double target_fov = cub->player->base_fov + (moving && cub->player->is_running ? cub->player->run_fov_delta : 0.0);
        double prev = cub->player->current_fov;
        cub->player->current_fov = cub->player->current_fov + (target_fov - cub->player->current_fov) * lerp_factor;
        if (fabs(cub->player->current_fov - prev) > 1e-3)
            find_player_plane(cub->player);
    }

    /* eye height smoothing: n'appliquer l'accroupissement que si moving */
    {
        double target_eye;
        if (moving && cub->player->is_crouching)
            target_eye = cub->player->crouch_eye_height_pixels;
        else
            target_eye = cub->player->eye_height_base;
        cub->player->eye_height_current = cub->player->eye_height_current + (target_eye - cub->player->eye_height_current) * lerp_factor;
    }

    len = sqrt(move_x * move_x + move_y * move_y);
    if (len > 0.0)
    {
        move_x /= len;
        move_y /= len;
        move_x *= cub->player->move_frame;
        move_y *= cub->player->move_frame;
        try_wall_slide(cub, move_x, move_y);
    }
}
