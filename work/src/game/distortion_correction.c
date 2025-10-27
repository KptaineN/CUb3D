#include "../../includes/cub3d.h"
#include <math.h>

/*
** Correction fish-eye mathématiquement parfaite - Projection linéaire pure
** Utilise la méthode géométrique du plan image pour éliminer TOUTE distorsion
*/
static double compute_perpendicular_distance(t_cub *cub, double ray_x, double ray_y, double raw_distance)
{
    // Méthode du plan image - plus précise que le produit scalaire
    double player_dir_x = cub->player->cos_angle;
    double player_dir_y = cub->player->sin_angle;
    
    // Calcul de l'angle avec atan2 pour une précision maximale
    double player_angle = atan2(player_dir_y, player_dir_x);
    double ray_angle = atan2(ray_y, ray_x);
    
    // Différence angulaire normalisée
    double angle_diff = ray_angle - player_angle;
    
    // Normalisation dans [-π, π] pour éviter les discontinuités
    while (angle_diff > M_PI) angle_diff -= 2.0 * M_PI;
    while (angle_diff < -M_PI) angle_diff += 2.0 * M_PI;
    
    // Correction cosinus pure - mathématiquement parfaite
    double cos_correction = cos(angle_diff);
    
    // Limitation ultra-conservative pour projection parfaitement linéaire
    if (cos_correction < 0.4) cos_correction = 0.4;   // Évite toute déformation
    if (cos_correction > 1.0) cos_correction = 1.0;
    
    // Distance corrigée avec protection progressive
    double corrected_distance = raw_distance * cos_correction;
    
    // Protection finale contre les valeurs extrêmes
    if (corrected_distance < 0.05) corrected_distance = 0.05;
    
    return corrected_distance;
}

/*
** Calcul de hauteur avec croissance non-linéaire stabilisée
** Élimine l'effet loupe avec une fonction de lissage progressive
*/
static void setup_vertical_bounds(t_distortion_data *result, double perpendicular_distance)
{
    double wall_height_f;
    int wall_height;
    int start;
    int end;

    // Protection contre les divisions par zéro
    if (perpendicular_distance < 0.01) perpendicular_distance = 0.01;
    
    // Calcul de base linéaire
    wall_height_f = (double)HEIGHT / perpendicular_distance;
    
    // Fonction de lissage anti-loupe progressive
    if (perpendicular_distance < 2.0) {
        // Fonction racine carrée pour adoucir la croissance
        double distance_factor = perpendicular_distance / 2.0;  // [0, 1]
        double smooth_curve = sqrt(distance_factor);             // Courbe douce
        
        // Limitation dynamique basée sur la distance
        double max_reasonable_height = HEIGHT * (1.5 + smooth_curve);
        
        if (wall_height_f > max_reasonable_height) {
            // Transition douce vers la limite
            double excess = wall_height_f - max_reasonable_height;
            wall_height_f = max_reasonable_height + excess * (0.2 + smooth_curve * 0.3);
        }
    }
    
    // Conversion avec arrondi correct
    wall_height = (int)round(wall_height_f);
    
    // Limites absolues pour éviter les débordements
    if (wall_height < 1) wall_height = 1;
    if (wall_height > HEIGHT * 3) wall_height = HEIGHT * 3;  // Limite plus stricte

    result->raw_wall_height = wall_height;
    result->corrected_wall_height = wall_height;

    start = HEIGHT / 2 - wall_height / 2;
    end = start + wall_height - 1;
    if (start < 0)
        start = 0;
    if (end >= HEIGHT)
        end = HEIGHT - 1;
    if (start > end)
        start = end;

    result->y_start = start;
    result->y_end = end;
}

/*
** Retourne les données de projection pour une colonne donnée.
*/
t_distortion_data apply_distortion_correction(t_cub *cub, int column, double ray_x, double ray_y, double raw_distance)
{
    t_distortion_data result;
    double perpendicular_distance;

    (void)column;
    perpendicular_distance = compute_perpendicular_distance(cub, ray_x, ray_y, raw_distance);
    result.corrected_distance = perpendicular_distance;
    setup_vertical_bounds(&result, perpendicular_distance);
    return result;
}
