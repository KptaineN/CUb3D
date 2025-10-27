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

#include <math.h>

/*
** Met à jour l'angle selon les flags et recalcule cos/sin si besoin.
** Angle normalisé dans [0, 2π).
*/
void	handle_player_rotation(t_cub *cub, bool *rotated)
{
	t_player	*player;

	if (!cub || !cub->player)
		return ;
	player = cub->player;
	*rotated = false;
	if (player->left_rotate)
	{
		player->angle -= player->rotation_frame;
		*rotated = true;
	}
	if (player->right_rotate)
	{
		player->angle += player->rotation_frame;
		*rotated = true;
	}
	if (*rotated)
	{
		player->angle = fmod(player->angle, 2.0 * M_PI);
		if (player->angle < 0.0)
			player->angle += 2.0 * M_PI;
		update_player_trig(cub);
	}
}

/*
** Déplacement SANS collision :
** - up/down : avance/recul le long de (cos, sin)
** - left/right : strafe le long de (-sin, +cos)
*/
void	move_player(t_cub *cub)
{
	t_player	*player;
	double		step;
	bool		rotated;

	if (!cub || !cub->player)
		return ;
	player = cub->player;
	handle_player_rotation(cub, &rotated);
	step = (double)player->move_frame;
	if (player->key_up)
	{
		player->position->x += player->cos_angle * step;
		player->position->y += player->sin_angle * step;
	}
	if (player->key_down)
	{
		player->position->x -= player->cos_angle * step;
		player->position->y -= player->sin_angle * step;
	}
	if (player->key_right)
	{
		player->position->x += -player->sin_angle * step;
		player->position->y +=  player->cos_angle * step;
	}
	if (player->key_left)
	{
		player->position->x +=  player->sin_angle * step;
		player->position->y += -player->cos_angle * step;
	}
}


