#include "../../includes/cub3d.h"


void update_player_trig(t_cub *cub)
{
    t_player *player;

    if (!cub || !cub->player->direction 
        || !cub->player->plane)
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
    double left;
    double right;
    double top;
    double bottom;

    radius = cub->player->radius;
    left = (x - radius) / BLOCK;
    right = (x + radius) / BLOCK;
    top = (y - radius) / BLOCK;
    bottom = (y + radius) / BLOCK;

    // Check the four corners of the player's bounding box

    if (is_wall(cub, (int)left, (int)top)) // Top-left
        return (1);
    if (is_wall(cub, (int)right, (int)top)) // Top-right
        return (1);
    if (is_wall(cub, (int)left, (int)bottom)) // Bottom-left
        return (1);
    if (is_wall(cub, (int)right, (int)bottom)) // Bottom-right
        return (1);

    return (0);
}

static void rotate_player(t_cub *cub)
{
    //double  old_dir_x; 
    //double  old_plane_x;

    bool        rotated;

    if (!cub || !cub->player)
        return ;
    cub->player = cub->player;
    rotated = false;
    if ( cub->player->left_rotate)
    {
        cub->player->angle -=  cub->player->rotation_frame;
        rotated = true;
    }
    if ( cub->player->right_rotate)
    {
        cub->player->angle +=  cub->player->rotation_frame;
        rotated = true;
    }
    if (rotated)
        update_player_trig(cub);
}

static void move_forward_backward(t_cub *cub)
{
    double  new_x;
    double  new_y;

    if (cub->player->key_up)
    {
        new_x = cub->player->position->x + cub->player->direction->x * cub->player->move_frame;
        new_y = cub->player->position->y + cub->player->direction->y * cub->player->move_frame;
        if (is_valid_position(cub, new_x, new_y) != 0)
        {
            cub->player->position->x = new_x;
            cub->player->position->y = new_y;
        }
    }
    if (cub->player->key_down)
    {
        new_x = cub->player->position->x - cub->player->direction->x * cub->player->move_frame;
        new_y = cub->player->position->y - cub->player->direction->y * cub->player->move_frame;
        if (is_valid_position(cub, new_x, new_y) != 0)
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
        if (is_valid_position(cub, new_x, new_y) != 0)
        {
            cub->player->position->x = new_x;
            cub->player->position->y = new_y;
        }
    }
    if (cub->player->key_right)
    {
        new_x = cub->player->position->x - cub->player->direction->y * cub->player->move_frame;
        new_y = cub->player->position->y + cub->player->direction->x * cub->player->move_frame;
        if (is_valid_position(cub, new_x, new_y) != 0)
        {
            cub->player->position->x = new_x;
            cub->player->position->y = new_y;
        }
    }
}

void move_player(t_cub *cub)
{
    rotate_player(cub);
    move_forward_backward(cub);
    move_strafe(cub);
}


