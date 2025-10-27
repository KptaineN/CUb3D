#!/bin/bash

# Remove static from functions that should be public in alien2.c
sed -i 's/^static void alien_apply_player_damage/void alien_apply_player_damage/g' alien2.c
sed -i 's/^static void alien_prepare_direct_pounce/void alien_prepare_direct_pounce/g' alien2.c
sed -i 's/^static t_alien_view_bucket alien_select_view_bucket/t_alien_view_bucket alien_select_view_bucket/g' alien2.c
sed -i 's/^static void alien_update_anim/void alien_update_anim/g' alien2.c
sed -i 's/^static int alien_build_path/int alien_build_path/g' alien2.c
sed -i 's/^static int alien_follow_path/int alien_follow_path/g' alien2.c
sed -i 's/^static void alien_face_target/void alien_face_target/g' alien2.c
sed -i 's/^static int alien_try_move/int alien_try_move/g' alien2.c

echo "Removed static from alien2.c functions"
