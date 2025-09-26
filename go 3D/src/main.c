/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkief <nkief@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:46:16 by adi-marc          #+#    #+#             */
/*   Updated: 2025/09/26 11:17:00 by nkief            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/*
-- TO DO --
1) parser_check_elements 
    -> Vérifier que les path sont bien des textures
    -> Inclure les path des textures dans la structure cub
    -> Vérifier que les F et C sont bien des couleurs RGB
    -> Convertir les couleurs dans le bon format -> inclure les couleurs dans t_color -> t_cub
2) parse_map -- DONE 
    -> Vérifier que la map existe dans le fichier cub->file -- DONE
    -> Dupliquer la map dans la structure cub (skip les espaces) cub->map -- DONE
    -> Vérifier que les seuls elements de la map sont 0 && 1 && (N | E | S | W) -- DONE
    -> cloner cub->map dans un buffer temporaire -> calculer la hauteur et largeur -> ajouter un padding exterieur au buffer -- DONE
    -> Parcourrir chaque 0 de la map pour verifier que ses 4 cotés ne touchent pas un vide -- DONE
3) Initializattion
    -> Trouver les informations du spawn du joueur et les inclure dans t_player -> t_cub
    ...
    
*/


int main(int argc, char **argv)
{
    t_cub   *cub;
    int     result;
    
    cub = malloc(sizeof(t_cub));
    if (!cub)
        return (1);
    init_values(cub);
    /* Initialize all pointers to NULL to avoid garbage values */
    result = ft_cub_parser(argc, argv, cub);
    if (result == 0)
    {
        printf("parser reussi\n");
        /* initialize player from parsed map (find spawn pos & dir) */
        if (!player_init(cub))
        {
            printf("Erreur: impossible d'initialiser le joueur\n");
            free_cub(cub);
            cub = NULL;
            return (1);
        }
        start_game(cub);
    }
    else
    {
        printf("Erreur de parsing\n");
    }
    free_cub(cub);
    cub = NULL;
    return (result);
}
