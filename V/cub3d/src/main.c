/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:46:16 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/27 15:11:06 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/*

-- TO DO --
1) parser_check_elements -- DONE
    -> Vérifier que les path sont bien des textures -- DONE
    -> Inclure les path des textures dans la structure cub -- DONE
    -> Vérifier que les F et C sont bien des couleurs RGB -- DONE
    -> Convertir les couleurs dans le bon format -> inclure les couleurs dans t_color -> t_cub -- DONE
2) parse_map -- DONE 
    -> Vérifier que la map existe dans le fichier cub->file -- DONE
    -> Dupliquer la map dans la structure cub (skip les espaces) cub->map -- DONE
    -> Vérifier que les seuls elements de la map sont 0 && 1 && (N | E | S | W) -- DONE
    -> cloner cub->map dans un buffer temporaire -> calculer la hauteur et largeur -> ajouter un padding exterieur au buffer -- DONE
    -> Parcourrir chaque 0 de la map pour verifier que ses 4 cotés ne touchent pas un vide -- DONE
3) Initializattion
    -> Trouver les informations du spawn du joueur et les inclure dans t_player -> t_cub
    ...
     

    REMINDER : CHANGER LE SPAWN DU JOUEUR AU MEME TEMPS DE START_GAME SUR LA MAP ET LE METTRE A '0'
*/

int main(int argc, char **argv)
{
    t_cub   *cub;
    
    cub = malloc(sizeof(t_cub));
    if (!cub)
        return (1);
    init_values(cub);
    if (!ft_cub_parser(argc, argv, cub))
    {
        if (player_init(cub))
        {
            if (start_game(cub))
            {
                return (0);
            }
        }
        return (1);        /* Valid map but player initialization failed */
    }
    return (1);            /* Invalid map */
}