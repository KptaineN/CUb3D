#!/bin/bash

# Fix alien1.c: remove static from exported functions and fix global vars
sed -i 's/static void detect_alien_mask_mode/void detect_alien_mask_mode/g' alien1.c
sed -i 's/static void build_alien_mask_map_for_tex/void build_alien_mask_map_for_tex/g' alien1.c
sed -i 's/static void build_and_report/void build_and_report/g' alien1.c
sed -i 's/static void draw_alien_sprite_texture_with/void draw_alien_sprite_texture_with/g' alien1.c
sed -i 's/static t_alien_view_bucket alien_select_view_bucket/t_alien_view_bucket alien_select_view_bucket/g' alien1.c

# Fix global variable references
sed -i 's/alien_mask_mode/g_alien_mask_mode/g' alien1.c alien2.c alien3.c
sed -i 's/alien_mask_detected/g_alien_mask_detected/g' alien1.c alien2.c alien3.c
sed -i 's/alien_detected_mode/g_alien_detected_mode/g' alien1.c alien2.c alien3.c
sed -i 's/alien_bg_r/g_alien_bg_r/g' alien1.c alien2.c alien3.c
sed -i 's/alien_bg_g/g_alien_bg_g/g' alien1.c alien2.c alien3.c
sed -i 's/alien_bg_b/g_alien_bg_b/g' alien1.c alien2.c alien3.c
sed -i 's/alien_replace_bg_color/g_alien_replace_bg_color/g' alien1.c alien2.c alien3.c
sed -i 's/alien_debug_show_raw/g_alien_debug_show_raw/g' alien1.c alien2.c alien3.c

# Fix alien2.c: remove static from exported functions
sed -i 's/static void alien_reset_path/void alien_reset_path/g' alien2.c
sed -i 's/static void alien_mark_wall_hit/void alien_mark_wall_hit/g' alien2.c
sed -i 's/static void alien_plan_retreat/void alien_plan_retreat/g' alien2.c
sed -i 's/static void alien_force_unstuck/void alien_force_unstuck/g' alien2.c
sed -i 's/static void alien_choose_patrol_target/void alien_choose_patrol_target/g' alien2.c
sed -i 's/static void alien_enter_search/void alien_enter_search/g' alien2.c
sed -i 's/static int alien_player_visible/int alien_player_visible/g' alien2.c

# Fix alien3.c: remove static from exported functions
sed -i 's/static void alien_set_pounce_target/void alien_set_pounce_target/g' alien3.c
sed -i 's/static void alien_finish_attack/void alien_finish_attack/g' alien3.c
sed -i 's/static void alien_apply_player_damage/void alien_apply_player_damage/g' alien3.c
sed -i 's/static void alien_prepare_direct_pounce/void alien_prepare_direct_pounce/g' alien3.c

echo "Files fixed"
