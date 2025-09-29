#include "../../includes/cub3d.h"


void update_player_trig(t_cub *cub)
{
    t_player *player;

    if (!cub || !cub->player)
        return;
    player = cub->player;
    player->cos_angle = cos(player->angle);
    player->sin_angle = sin(player->angle);
    player->direction->x = player->cos_angle;
    player->direction->y = player->sin_angle;
    find_player_plane(cub); // <-- réutilisation
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
    if (!cub->map[map_y] || !cub->map[map_y][map_x])
        return (1); // Treat out of bounds as walls

    // Check if it's a wall
    return (cub->map[map_y][map_x] == '1');
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

    radius = cub->player->radius;

    // Check the four corners of the player's bounding box
    if (is_wall(cub, (int)(x - radius), (int)(y - radius))) // Top-left
        return (0);
    if (is_wall(cub, (int)(x + radius), (int)(y - radius))) // Top-right
        return (0);
    if (is_wall(cub, (int)(x - radius), (int)(y + radius))) // Bottom-left
        return (0);
    if (is_wall(cub, (int)(x + radius), (int)(y + radius))) // Bottom-right
        return (0);

    return (1);
}

static void rotate_player(t_player *player)
{
    double  old_dir_x; 
    double  old_plane_x;

    if (player->left_rotate)
    {
        player->angle -= player->rotation_frame;
        old_dir_x = player->direction->x;
        player->direction->x = player->direction->x * cos(-player->rotation_frame) - player->direction->y * sin(-player->rotation_frame);
        player->direction->y = old_dir_x * sin(-player->rotation_frame) + player->direction->y * cos(-player->rotation_frame);
        old_plane_x = player->plane->x;
        player->plane->x = player->plane->x * cos(-player->rotation_frame) - player->plane->y * sin(-player->rotation_frame);
        player->plane->y = old_plane_x * sin(-player->rotation_frame) + player->plane->y * cos(-player->rotation_frame);
    }
    if (player->right_rotate)
    {
        player->angle += player->rotation_frame;
        old_dir_x = player->direction->x;
        player->direction->x = player->direction->x * cos(player->rotation_frame) - player->direction->y * sin(player->rotation_frame);
        player->direction->y = old_dir_x * sin(player->rotation_frame) + player->direction->y * cos(player->rotation_frame);
        old_plane_x = player->plane->x;
        player->plane->x = player->plane->x * cos(player->rotation_frame) - player->plane->y * sin(player->rotation_frame);
        player->plane->y = old_plane_x * sin(player->rotation_frame) + player->plane->y * cos(player->rotation_frame);
    }
}

static void move_forward_backward(t_cub *cub)
{
    double  new_x;
    double  new_y;

    if (cub->player->key_up)
    {
        new_x = cub->player->position->x + cub->player->direction->x * cub->player->move_frame;
        new_y = cub->player->position->y + cub->player->direction->y * cub->player->move_frame;
        if (is_valid_position(cub, new_x, new_y))
        {
            cub->player->position->x = new_x;
            cub->player->position->y = new_y;
        }
    }
    if (cub->player->key_down)
    {
        new_x = cub->player->position->x - cub->player->direction->x * cub->player->move_frame;
        new_y = cub->player->position->y - cub->player->direction->y * cub->player->move_frame;
        if (is_valid_position(cub, new_x, new_y))
        {
            cub->player->position->x = new_x;
            cub->player->position->y = new_y;
        }
    }
}

static void move_strafe(t_cub *cub)
{
    double  new_x; 
    double  new_y;

    if (cub->player->key_left)
    {
        new_x = cub->player->position->x + cub->player->direction->y * cub->player->move_frame;
        new_y = cub->player->position->y - cub->player->direction->x * cub->player->move_frame;
        if (is_valid_position(cub, new_x, new_y))
        {
            cub->player->position->x = new_x;
            cub->player->position->y = new_y;
        }
    }
    if (cub->player->key_right)
    {
        new_x = cub->player->position->x - cub->player->direction->y * cub->player->move_frame;
        new_y = cub->player->position->y + cub->player->direction->x * cub->player->move_frame;
        if (is_valid_position(cub, new_x, new_y))
        {
            cub->player->position->x = new_x;
            cub->player->position->y = new_y;
        }
    }
}

void move_player(t_cub *cub)
{
    rotate_player(cub->player);
    move_forward_backward(cub);
    move_strafe(cub);
}


