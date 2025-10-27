/* ************************************************************************** */
/*                                                                            */
/*   alien_globals.c - Global state variables for alien module               */
/*                                                                            */
/* ************************************************************************** */

#include "alien_internal.h"

/* Global state variables (shared across alien modules) */
t_mask_mode g_alien_mask_mode = MASK_AUTO;
int g_alien_mask_detected = 0;
t_mask_mode g_alien_detected_mode = MASK_AUTO;
int g_alien_bg_r = 0;
int g_alien_bg_g = 0;
int g_alien_bg_b = 0;
int g_alien_replace_bg_color = -1;
