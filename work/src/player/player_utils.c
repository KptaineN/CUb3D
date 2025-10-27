#include "../../includes/cub3d.h"

/* Retourne true si une touche de déplacement est active */
bool player_is_moving(t_player *p)
{
    if (!p)
        return (false);
    return (p->key_up || p->key_down || p->key_left || p->key_right);
}
