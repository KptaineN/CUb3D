#!/bin/bash

# Backup original
cp alien.c alien_backup.c

# Extract line ranges to separate files
# Note: sed -n 'X,Yp' prints lines X to Y

# alien1.c: Rendering & Textures (lines 1-3 header, 56-498)
echo "/* ************************************************************************** */
/*                                                                            */
/*   alien1.c - Rendering & Texture management                                */
/*                                                                            */
/* ************************************************************************** */

#include \"alien_internal.h\"

/* Helper function to put pixel (from drawing.c) */
extern void put_pixel(int x, int y, int color, t_cub *cub);
" > alien1.c
sed -n '57,498p' alien_backup.c >> alien1.c

# alien2.c: AI & Pathfinding (lines 500-1126)
echo "/* ************************************************************************** */
/*                                                                            */
/*   alien2.c - AI & Pathfinding logic                                        */
/*                                                                            */
/* ************************************************************************** */

#include \"alien_internal.h\"
" > alien2.c
sed -n '500,1126p' alien_backup.c >> alien2.c

# alien3.c: Combat & Physics (lines 192-240, 693-860, 1513-1660)
echo "/* ************************************************************************** */
/*                                                                            */
/*   alien3.c - Combat mechanics & Physics                                    */
/*                                                                            */
/* ************************************************************************** */

#include \"alien_internal.h\"
" > alien3.c
sed -n '192,240p' alien_backup.c >> alien3.c
sed -n '693,860p' alien_backup.c >> alien3.c
sed -n '1513,1660p' alien_backup.c >> alien3.c

echo "Files created: alien1.c alien2.c alien3.c"
